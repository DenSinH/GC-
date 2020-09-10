// BEGIN vertexShaderSource

#version 430 core
const uint draw_arg_POS = %draw_arg_POS%;

uniform uint VCD_lo, VCD_hi;
uniform uint VAT_A;
uniform uint VAT_B;
uniform uint VAT_C;

// here, the array is read as little endian
const int extract_offset[4] = { 0, 8, 16, 24 };

#define READ8(array, address) (bitfieldExtract(array[address >> 2], extract_offset[(address) & 3], 8));
#define READ8s(array, address) (bitfieldExtract(int(array[address >> 2]), extract_offset[(address) & 3], 8));

// layout corresponds to that in GX_constants.h
layout (location = 0) in uint index;

layout (std430, binding = 3) buffer ssbo
{
    uint vertices;
    uint vertex_stride;
    int arg_offsets[21];
    int data_offsets[12];
    uint _;  // data_size: I don't actually need this in the shader
    uint args[0x1140 >> 2];     // todo: generalize this
    uint data[];
};

out vec4 vertexColor;

void main()
{
    uint draw_arg;
    uint arg_offset;
    uint data_offset;

    int x, y, z;
    {
        /* determine vertex position */
        // todo: different cases (right now only i8 indexed rgba8888
        // initial values
        draw_arg = %draw_arg_POS%;
        arg_offset = arg_offsets[draw_arg];
        arg_offset += gl_VertexID * vertex_stride;
        // todo: signed offset, 16bit offset
        uint vertex_index = READ8(args, arg_offset);

        data_offset = data_offsets[draw_arg - draw_arg_POS];

        // todo: generalize strides
        x = READ8s(data, data_offset + (vertex_index * 6));
        x <<= 8;
        x |= READ8s(data, data_offset + (vertex_index * 6) + 1);
        x <<= 16;
        x >>= 16;

        y = READ8s(data, data_offset + (vertex_index * 6) + 2);
        y <<= 8;
        y |= READ8s(data, data_offset + (vertex_index * 6) + 3);
        y <<= 16;
        y >>= 16;

        z = READ8s(data, data_offset + (vertex_index * 6) + 4);
        z <<= 8;
        z |= READ8s(data, data_offset + (vertex_index * 6) + 5);
        z <<= 16;
        z >>= 16;

        gl_Position = vec4(x / 32.0, y / 32.0, z / 32.0, 1.0);
//        switch (gl_VertexID) {
//            case 4:
//                gl_Position = vec4(-0.5, -0.5, 0.0, 1.0);
//                break;
//            case 5:
//                gl_Position = vec4(0.5, -0.5, 0.0, 1.0);
//                break;
//            case 6:
//                gl_Position = vec4(0.0, 0.5, 0.0, 1.0);
//                break;
//            case 7:
//                gl_Position = vec4(0.5, 0.5, 0.0, 1.0);
//                break;
//            default:
//                gl_Position = vec4(x / 32.0, y / 32.0, z / 32.0, 1.0);
//                break;
//        }
    }

    {
        /* determine vertex color */
        // todo: different cases (right now only i8 indexed rgba8888
        // initial values
        draw_arg = %draw_arg_CLR0%;
        arg_offset = arg_offsets[draw_arg];
        arg_offset += gl_VertexID * vertex_stride;
        // todo: signed offset
        uint color_index = READ8(args, arg_offset);

        data_offset = data_offsets[draw_arg - draw_arg_POS];

        uint r, g, b;
        // todo: generalize strides
        r = READ8(data, data_offset + (color_index << 2));
        g = READ8(data, data_offset + (color_index << 2) + 1);
        b = READ8(data, data_offset + (color_index << 2) + 2);

        vertexColor = vec4(r / 255.0, g / 255.0, b / 255.0, 1.0);
//        if (z == 0) {
//            vertexColor = vec4(0.0, 1.0, 0.0, 1.0);
//        }
//        else {
//            vertexColor = vec4(1.0, 0.0, 0.0, 1.0);
//        }
    }
}

// END vertexShaderSource