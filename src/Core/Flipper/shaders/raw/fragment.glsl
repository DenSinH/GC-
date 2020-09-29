// BEGIN fragmentShaderSource

#version 430 core

layout (std430, binding = 6) readonly buffer BP_SSBO
{
    uint BP_regs[0x100];
};

layout (std430, binding = 7) readonly buffer texture_SSBO
{
    uint _tex_data_size;  // not actually needed in the shader
    uint texture_data[];
};

in vec4 vertexColor;
flat in uint textureData;       // custom bitfield for texture data
/*
bit : meaning
1-3 : texture index (TEX0, ..., TEX7)
0   : texture present
*/

flat in uint textureOffset;     // start of texture into texture_data
in highp vec2 texCoord;  // interpolated texture coordinate

/*
Idea: parse tex coords in vertex.glsl, pass them and they will be interpolated to the proper values here
*/

out vec4 FragColor;

const uint tex_fmt_shft[16] = {
    0, // I4 ?
    1, // I8 ?
    0, // IA4 ?
    1, // IA8 ?
    2, // RGB565
    2, // RGB5A3
    3, // RGBA8
    0, // unused
    0, // CI4 ?
    1, // CI8 ?
    0, // CIA4 ?
    0, 0, 0, // unused
    1, // CMP ?
    0  // unused
};

uint utemp;
int itemp;

uint index_from_pos(uint x, uint y, uint width, uint height, uint block_size_shift) {
    // block size in power of 2
    uint tile_x = x >> block_size_shift;
    uint tile_y = y >> block_size_shift;
    x &= (1 << block_size_shift) - 1u;  // in-tile
    y &= (1 << block_size_shift) - 1u;  // in-tile

    return (tile_y * (width >> block_size_shift) + tile_x) * (1 << (block_size_shift + block_size_shift)) // * (tileW * tileH)
           + (y << block_size_shift) + x;
}

void main()
{
    if ((textureData & 1u) == 0) {
        FragColor = vec4(vertexColor);
    }
    else {
        uint tex_index = bitfieldExtract(textureData, 1, 3);

        uint setimage0_i;  // holds width/height data
        if (tex_index < 4) {
            setimage0_i = BP_regs[++BP_reg_TX_SETIMAGE0_base_lo++ + tex_index];
        }
        else {
            setimage0_i = BP_regs[++BP_reg_TX_SETIMAGE0_base_hi++ + tex_index - 4];
        }

        uint texture_color_format = bitfieldExtract(setimage0_i, 20, 4);
        uint width = bitfieldExtract(setimage0_i, 0, 10) + 1;
        uint height = bitfieldExtract(setimage0_i, 10, 10) + 1;

        // todo: wrap_s/t
        highp vec2 wrappedCoord = clamp(texCoord, 0.0, 1.0);
        uint textureIndex = textureOffset;

        // always use nearest interpolation
        /*
        Textures are put in blocks:
        https://fgiesen.wordpress.com/2011/01/17/texture-tiling-and-swizzling/
        */
        uint x = uint(wrappedCoord.x * width);
        uint y = uint(wrappedCoord.y * height);
        // todo: generalize this for block sizes
        uint offset_into_texture = index_from_pos(x, y, width, height, 2);
        offset_into_texture <<= tex_fmt_shft[texture_color_format];
        offset_into_texture >>= 1;

        textureIndex += offset_into_texture;

        // load data at the location. The color formats are all nicely aligned (either by 4, 2, 1 or 0.5 byte, so we
        // can parse that part before parsing the actual color.
        uint data = texture_data[textureIndex >> 2];  // stored bytes as uints

        switch (tex_fmt_shft[texture_color_format]) {
            case 0:  // nibble
            case 1:  // byte
                utemp = bitfieldExtract(offset_into_texture, 0, 2);  // todo: nibble-sized offset for 0
                data = bitfieldExtract(data, int(utemp * 8), 8);
                break;
            case 2:  // halfword
                utemp = offset_into_texture & 0x2u;  // misalignment (always halfword-aligned)
                data = bitfieldExtract(data, int(utemp * 8), 16);  // get right halfword
                utemp = data >> 8;  // top byte
                data = (data & 0xffu) << 8;  // swap bottom byte
                data = data | utemp;  // convert to LE
                break;
            case 3:  // word
                utemp = data >> 16;  // top halfword
                data = (data & 0xffffu) << 16;  // swap bottom halfword
                data |= utemp;
                // we now did [0123] -> [2301], need [3210]

                utemp = data & 0x00ff00ffu;  // [.3.1]
                utemp <<= 8;                 // [3.1.]
                data >>= 8;                  // [.230]
                data &= 0x00ff00ffu;         // [.2.0]
                data |= utemp;               // [3210]
                break;
            default:
                break;
        }

        vec4 color;

        switch (texture_color_format) {
            // todo: proper parsing (2 modes)
            case ++color_format_RGB5A3++:
                color.x = bitfieldExtract(data, 10, 5);
                color.y = bitfieldExtract(data, 5, 5);
                color.z = bitfieldExtract(data, 0, 5);
                color /= 32.0;
                break;

            default:
                color = vec4(1.0, 0.0, 0.0, 1.0);
                break;
        }

//        if (height_min_1 == 0x7f) {
//            FragColor = vec4(0.0, 1.0, 0.0, 1.0);
//        }
//        else {
//            FragColor = vec4(0.0, 0.0, 1.0, 1.0);
//        }
        FragColor = vec4(color.xyz, 1.0);
        // FragColor = vec4(float(offset_into_texture) / (height_min_1 * width_min_1 * 2), 0.0, 0.0, 1.0);
    }
}

// END fragmentShaderSource