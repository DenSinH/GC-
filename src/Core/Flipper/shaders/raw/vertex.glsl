// BEGIN vertexShaderSource

#version 430 core

#undef DEBUG

const uint draw_arg_POS = ++draw_arg_POS++;

uniform uint VCD_lo, VCD_hi;
uniform uint VAT_A, VAT_B, VAT_C;
uniform uint MATIDX_REG_A, MATIDX_REG_B;

// here, the array is read as little endian
const int extract_offset[4] = { 0, 8, 16, 24 };

layout (std430, binding = 3) readonly buffer command_SSBO
{
    uint vertices;
    uint _command;
    uint vertex_stride;
    int arg_offsets[21];
    int data_offsets[21];
    uint array_strides[21];
    uint _cmd_data_size;  // data_size: I don't actually need this in the shader
    uint args[0x1140 >> 2];     // todo: generalize this
    uint data[];
};

out vec4 vertexColor;

flat out uint textureData;
flat out uint textureOffset;
out highp vec2 texCoord;

/*
 * I can't pass arrays to functions, so I'll have to make due writing separate read handlers for args/data
 * */
#define read8(array) uint read8_ ## array ##(uint address) { return  bitfieldExtract(array[(address) >> 2], extract_offset[(address) & 3], 8); }
#define read8s(array) int read8s_ ## array ##(uint address) { return  bitfieldExtract(int(array[(address) >> 2]), extract_offset[(address) & 3], 8); }

// bitfieldExtract sign extends the data for us when we pass signed ints!
#define read16(array) uint read16_ ## array ##(uint address) { \
    uint msb = read8_ ## array ##(address); \
    uint lsb = read8_ ## array ##(address + 1); \
    return (msb << 8) | lsb; \
}

#define read16s(array) int read16s_ ## array ##(uint address) { \
    int msb = read8s_ ## array ##(address); \
    uint lsb = read8_ ## array ##(address + 1); \
    return (msb << 8) | int(lsb); \
}

#define read32(array) uint read32_ ## array ##(uint address) { \
    uint msh = read16_ ## array ##(address); \
    uint lsh = read16_ ## array ##(address + 2); \
    return (msh << 16) | lsh; \
}

#define read32s(array) int read32s_ ## array ##(uint address) { \
    int msh = read16s_ ## array ##(address); \
    uint lsh = read16_ ## array ##(address + 2); \
    return (msh << 16) | int(lsh); \
}

read8(data)
read8s(data)
read8(args)
read8s(args)

read16(data)
read16s(data)
read16(args)
read16s(args)

read32(data)
read32s(data)
read32(args)
read32s(args)

// temporary stuff
int stemp;
uint utemp;

// defined in trafo.glsl
vec4 transform_pos(vec3 position, uint posidx);
vec3 transform_tex(vec3 texcoord, uint texidx);

vec3 load_position(bool from_data, uint FMT, uint offset)
{
    // always load 3 coodinates, decide on whether to use the last one later
    vec3 position;
    if (from_data) {
        switch (FMT) {
            case 0:  // u8
                position.x = read8_data(offset);
                position.y = read8_data(offset + 1);
                position.z = read8_data(offset + 2);
                break;
            case 1:  // s8
                position.x = read8s_data(offset);
                position.y = read8s_data(offset + 1);
                position.z = read8s_data(offset + 2);
                break;
            case 2:  // u16
                position.x = read16_data(offset);
                position.y = read16_data(offset + 2);
                position.z = read16_data(offset + 4);
                break;
            case 3:  // s16
                position.x = read16s_data(offset);
                position.y = read16s_data(offset + 2);
                position.z = read16s_data(offset + 4);
                break;
            case 4:
                utemp = read32_data(offset);
                position.x = uintBitsToFloat(utemp);
                utemp = read32_data(offset + 4);
                position.y = uintBitsToFloat(utemp);
                utemp = read32_data(offset + 8);
                position.z = uintBitsToFloat(utemp);
                break;
            default:
                // invalid format
                position = vec3(0, 0, 0);
                break;
        }
        return position;
    }
    else {
        // read data from args
        // always load 3 coodinates, decide on whether to use the last one later
        switch (FMT) {
            case 0:  // u8
                position.x = read8_args(offset);
                position.y = read8_args(offset + 1);
                position.z = read8_args(offset + 2);
                break;
            case 1:  // s8
                position.x = read8s_args(offset);
                position.y = read8s_args(offset + 1);
                position.z = read8s_args(offset + 2);
                break;
            case 2:  // u16
                position.x = read16_args(offset);
                position.y = read16_args(offset + 2);
                position.z = read16_args(offset + 4);
                break;
            case 3:  // s16
                position.x = read16s_args(offset);
                position.y = read16s_args(offset + 2);
                position.z = read16s_args(offset + 4);
                break;
            case 4:
                utemp = read32_args(offset);
                position.x = uintBitsToFloat(utemp);
                utemp = read32_args(offset + 4);
                position.y = uintBitsToFloat(utemp);
                utemp = read32_args(offset + 8);
                position.z = uintBitsToFloat(utemp);
                break;
            default:
                // invalid format
                position = vec3(0, 0, 0);
                break;
        }
        return position;
    }
}

void main()
{
    // used for all types of parameters
    uint arg_offset;
    uint data_offset;

    // placed here mostly for debugging purposes
    vec3 position;

    if (arg_offsets[++draw_arg_POS++] >= 0)
    {
        /* determine vertex position */

        // initial values
        arg_offset = arg_offsets[++draw_arg_POS++];
        arg_offset += gl_VertexID * vertex_stride;

        uint POSVCD  = bitfieldExtract(VCD_lo, 9, 2);
        bool POSCNT  = bitfieldExtract(VAT_A, 0, 1) != 0;
        uint POSFMT  = bitfieldExtract(VAT_A, 1, 3);
        uint POSSHFT = bitfieldExtract(VAT_A, 4, 5);

        if (POSVCD > 1) {
            // indirect data
            data_offset = data_offsets[++draw_arg_POS++];

            // determine the "GC vertex index"
            int vertex_index;
            if (POSVCD == 2) {
                vertex_index = read8s_args(arg_offset);
            }
            else {
                vertex_index = read16s_args(arg_offset);
            }

            data_offset += vertex_index * array_strides[++draw_arg_POS++ - draw_arg_POS];

            position = load_position(true, POSFMT, data_offset);
        }
        else {
            position = load_position(false, POSFMT, arg_offset);
        }

        // todo: POSSHFT
        if (!POSCNT) {
            // 2D
            position.z = 0;  // todo: what is this value supposed to be?
        }

        uint posidx;
        if (arg_offsets[++draw_arg_PNMTXIDX++] >= 0) {
            // position matrix index value passed (always direct)
            posidx = read8_args(arg_offsets[++draw_arg_PNMTXIDX++]);
        }
        else {
            posidx = bitfieldExtract(MATIDX_REG_A, 0, 6);
        }

        gl_Position = transform_pos(position, posidx);
#ifdef DEBUG
        switch (gl_VertexID) {
            case 0:
                gl_Position = vec4(-0.5, -0.5, 0.0, 1.0);
                break;
            case 1:
                gl_Position = vec4(0.0, -0.5, 0.0, 1.0);
                break;
            case 2:
                gl_Position = vec4(0.5, 0.5, 0.0, 1.0);
                break;
            case 3:
                gl_Position = vec4(0.0, 0.5, 0.0, 1.0);
                break;
        }
#endif
    }

    if (arg_offsets[++draw_arg_CLR0++] >= 0)
    {
        /* determine vertex color 0 */
        // todo: different cases (right now only i8 indexed rgba8888
        // initial values
        arg_offset = arg_offsets[++draw_arg_CLR0++];
        arg_offset += gl_VertexID * vertex_stride;

        uint COL0VCD  = bitfieldExtract(VCD_lo, 13, 2);
        bool COL0CNT  = bitfieldExtract(VAT_A, 13, 1) != 0;
        uint COL0FMT  = bitfieldExtract(VAT_A, 14, 3);

        vec4 color;

        if (COL0VCD > 1) {
            // indirect data
            data_offset = data_offsets[++draw_arg_CLR0++];

            int color_index;
            if (COL0VCD == 2) {
                color_index = read8s_args(arg_offset);
            }
            else {
                color_index = read16s_args(arg_offset);
            }

            data_offset += color_index * array_strides[++draw_arg_CLR0++ - draw_arg_POS];

            // always get 4 color value, determine actual value later
            switch (COL0FMT) {
                case 0:  // 16bit rgb565
                    utemp = read16_data(data_offset);
                    color.x = bitfieldExtract(utemp, 0, 5);
                    color.y = bitfieldExtract(utemp, 4, 6);
                    color.z = bitfieldExtract(utemp, 10, 5);
                    color /= 32.0;
                    color.y *= 0.5; // extra bit
                    color.w = 1.0;
                    break;
                case 1:  // 24bit rgb888
                    utemp = read32_data(data_offset);
                    color = unpackUnorm4x8(utemp).wzyx;  // BE to LE
                    color.w = 1.0;  // 3 colors
                    // already normalized
                    break;
                case 2:  // 32bit rgb888x
                    utemp = data[data_offset >> 2];  // data is 4 aligned
                    color = unpackUnorm4x8(utemp);
                    color.w = 1.0;  // 3 colors
                    // already normalized
                    break;
                case 3:  // 16bit rgba4444
                    utemp = read16_data(data_offset);
                    color.x = bitfieldExtract(utemp, 0, 4);
                    color.y = bitfieldExtract(utemp, 3, 4);
                    color.z = bitfieldExtract(utemp, 7, 4);
                    color.w = bitfieldExtract(utemp, 11, 4);
                    color /= 16.0;  // normalize
                    break;
                case 4:  // 24bit rgba6666
                    utemp = read32_data(data_offset);
                    color.x = bitfieldExtract(utemp, 0, 6);
                    color.y = bitfieldExtract(utemp, 5, 6);
                    color.z = bitfieldExtract(utemp, 11, 6);
                    color.w = bitfieldExtract(utemp, 17, 6);
                    color /= 64.0;  // normalize
                    break;
                case 5:  // 32bit rgba8888
                    utemp = data[data_offset >> 2];  // data is 4 aligned
                    color = unpackUnorm4x8(utemp);
                    // already normalized
                    break;
                default:
                    break;
            }
        }
        else {
            switch (COL0FMT) {
                case 0:  // 16bit rgb565
                    utemp = read16_args(arg_offset);
                    color.x = bitfieldExtract(utemp, 0, 5);
                    color.y = bitfieldExtract(utemp, 4, 6);
                    color.z = bitfieldExtract(utemp, 10, 5);
                    color /= 32.0;
                    color.y *= 0.5;  // extra bit
                    color.w = 1.0;
                    break;
                case 1:  // 24bit rgb888
                case 2:  // 32bit rgb888x
                    utemp = read32_args(arg_offset);
                    color = unpackUnorm4x8(utemp).wzyx;  // BE to LE
                    color.w = 1.0;  // 3 colors
                    // already normalized
                    break;
                case 3:  // 16bit rgba4444
                    utemp = read16_args(arg_offset);
                    color.x = bitfieldExtract(utemp, 0, 4);
                    color.y = bitfieldExtract(utemp, 3, 4);
                    color.z = bitfieldExtract(utemp, 7, 4);
                    color.w = bitfieldExtract(utemp, 11, 4);
                    color /= 16.0;  // normalize
                    break;
                case 4:  // 24bit rgba6666
                    utemp = read32_args(arg_offset);
                    color.x = bitfieldExtract(utemp, 0, 6);
                    color.y = bitfieldExtract(utemp, 5, 6);
                    color.z = bitfieldExtract(utemp, 11, 6);
                    color.w = bitfieldExtract(utemp, 17, 6);
                    color /= 64.0;  // normalize
                    break;
                case 5:  // 32bit rgba8888
                    utemp = read32_args(arg_offset);
                    color = unpackUnorm4x8(utemp).wzyx;  // BE to LE
                    // already normalized
                    break;
                default:
                    break;
            }
        }

        // todo: I am not sure if COLCNT really does anything, but just to be sure
        vertexColor = color;
        if (!COL0CNT) color.w = 1.0;
#ifdef DEBUG
        if (position.y > 29) {
            vertexColor = vec4(0.0, 1.0, 0.0, 1.0);
        }
        else {
            vertexColor = vec4(1.0, 0.0, 0.0, 1.0);
        }
#endif
    }

    /* load texture data */
    textureData = 0;
    for (int i = 0; i < 8; i++) {
        if (arg_offsets[++draw_arg_TEX0++ + i] >= 0) {
            // load texture specific data
            textureData = 1 | (uint(i) << 1);
            textureOffset = data_offsets[i];

            // load texture coordinate (same as position basically)
            arg_offset = arg_offsets[++draw_arg_TEX0++ + i];
            arg_offset += gl_VertexID * vertex_stride;

            uint TEXVCD = bitfieldExtract(VCD_hi, 2 * i, 2);
            bool TEXCNT;
            uint TEXFMT;
            uint TEXSHFT;
            switch (i) {
                case 0:
                    // TEX0 is in VAT_A
                    TEXCNT = bitfieldExtract(VAT_A, 21, 1) != 0;
                    TEXFMT = bitfieldExtract(VAT_A, 22, 3);
                    TEXSHFT = bitfieldExtract(VAT_A, 25, 5);
                    break;
                case 1:
                case 2:
                case 3:
                    // TEX1-3 are fully in VAT_B
                    TEXCNT = bitfieldExtract(VAT_B, 9 * (i - 1), 1) != 0;
                    TEXFMT = bitfieldExtract(VAT_B, 9 * (i - 1) + 1, 3);
                    TEXSHFT = bitfieldExtract(VAT_B, 9 * (i - 1) + 4, 5);
                    break;
                case 4:
                    // TEX4 is partly in VAT_B, partly in VAT_C
                    TEXCNT = bitfieldExtract(VAT_B, 27, 1) != 0;
                    TEXFMT = bitfieldExtract(VAT_B, 28, 3);
                    TEXSHFT = bitfieldExtract(VAT_C, 0, 5);
                    break;
                case 5:
                case 6:
                case 7:
                    // TEX5-7 are fully in VAT_C
                    TEXCNT = bitfieldExtract(VAT_C, 5 + 9 * (i - 5), 1) != 0;
                    TEXFMT = bitfieldExtract(VAT_C, 5 + 9 * (i - 5) + 1, 3);
                    TEXSHFT = bitfieldExtract(VAT_C, 5 + 9 * (i - 5) + 4, 5);
                default:
                    // invalid texture format
                    break;
            }

            vec3 read_tex_coord;

            if (TEXVCD > 1) {
                // indirect data
                data_offset = data_offsets[++draw_arg_TEX0++ + i];

                // determine the GC texture coordiante index
                int tex_coord_index;
                if (TEXVCD == 2) {
                    tex_coord_index = read8s_args(arg_offset);
                }
                else {
                    tex_coord_index = read16s_args(arg_offset);
                }

                data_offset += tex_coord_index * array_strides[++draw_arg_TEX0++ + i - draw_arg_POS];

                read_tex_coord = load_position(true, TEXFMT, data_offset);
            }
            else {
                read_tex_coord = load_position(false, TEXFMT, arg_offset);
            }

            read_tex_coord.z = 0;  // 2D at most
            if (!TEXCNT) {
                read_tex_coord.y = 0;  // 1D
            }

            uint texidx;
            if (arg_offsets[++draw_arg_TEX0MTXIDX++ + i] >= 0) {
                // texture matrix index value passed (always direct)
                texidx = read8_args(arg_offsets[++draw_arg_TEX0MTXIDX++ + i]);
            }
            else {
                if (i < 4)  {
                    // TEX0-3 in MATIDX_REG_A
                    texidx = bitfieldExtract(MATIDX_REG_A, 6 * (i + 1), 6);
                }
                else {
                    // TEX4-7 in MATIDX_REG_B
                    texidx = bitfieldExtract(MATIDX_REG_B, 6 * (i - 4), 6);
                }
            }

            texCoord = transform_tex(read_tex_coord, texidx).xy;
            break;
        }
    }
}

// END vertexShaderSource