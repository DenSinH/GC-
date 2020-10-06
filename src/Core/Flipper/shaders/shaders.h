#ifndef GC__SHADER_H
#define GC__SHADER_H

// fragmentShaderSource (from fragment.glsl, lines 2 to 272)
const char* fragmentShaderSource = 
"#version 430 core\n"  // l:1
"\n"  // l:2
"layout (std430, binding = 6) readonly buffer BP_SSBO\n"  // l:3
"{\n"  // l:4
"    uint BP_regs[0x100];\n"  // l:5
"};\n"  // l:6
"\n"  // l:7
"layout (std430, binding = 7) readonly buffer texture_SSBO\n"  // l:8
"{\n"  // l:9
"    uint _tex_data_size;  // not actually needed in the shader\n"  // l:10
"    uint texture_data[];\n"  // l:11
"};\n"  // l:12
"\n"  // l:13
"in vec4 vertexColor;\n"  // l:14
"flat in uint textureData;       // custom bitfield for texture data\n"  // l:15
"/*\n"  // l:16
"bit : meaning\n"  // l:17
"1-3 : texture index (TEX0, ..., TEX7)\n"  // l:18
"0   : texture present\n"  // l:19
"*/\n"  // l:20
"\n"  // l:21
"flat in uint textureOffset;     // start of texture into texture_data\n"  // l:22
"in highp vec2 texCoord;  // interpolated texture coordinate\n"  // l:23
"\n"  // l:24
"/*\n"  // l:25
"Idea: parse tex coords in vertex.glsl, pass them and they will be interpolated to the proper values here\n"  // l:26
"*/\n"  // l:27
"\n"  // l:28
"out vec4 FragColor;\n"  // l:29
"\n"  // l:30
"// shift per pixel\n"  // l:31
"const uint tex_fmt_shft[16] = {\n"  // l:32
"    0, // I4 ?\n"  // l:33
"    1, // I8 ?\n"  // l:34
"    0, // IA4 ?\n"  // l:35
"    1, // IA8 ?\n"  // l:36
"    2, // RGB565\n"  // l:37
"    2, // RGB5A3\n"  // l:38
"    3, // RGBA8\n"  // l:39
"    0, // unused\n"  // l:40
"    0, // CI4 ?\n"  // l:41
"    1, // CI8 ?\n"  // l:42
"    0, // CIA4 ?\n"  // l:43
"    0, 0, 0, // unused\n"  // l:44
"    1, // CMP ?\n"  // l:45
"    0  // unused\n"  // l:46
"};\n"  // l:47
"\n"  // l:48
"const uint tex_fmt_block_width_shft[16] = {\n"  // l:49
"    3, // I4\n"  // l:50
"    3, // I8\n"  // l:51
"    3, // IA4\n"  // l:52
"    2, // IA8\n"  // l:53
"    2, // RGB565\n"  // l:54
"    2, // RGB5A3\n"  // l:55
"    2, // RGBA8\n"  // l:56
"    0, // unused\n"  // l:57
"    3, // CI4\n"  // l:58
"    3, // CI8\n"  // l:59
"    0, // CIA4\n"  // l:60
"    0, 0, 0, // unused\n"  // l:61
"    3, // CMP\n"  // l:62
"    0  // unused\n"  // l:63
"};\n"  // l:64
"\n"  // l:65
"const uint tex_fmt_block_height_shft[16] = {\n"  // l:66
"    3, // I4\n"  // l:67
"    2, // I8\n"  // l:68
"    2, // IA4\n"  // l:69
"    2, // IA8\n"  // l:70
"    2, // RGB565\n"  // l:71
"    2, // RGB5A3\n"  // l:72
"    2, // RGBA8\n"  // l:73
"    0, // unused\n"  // l:74
"    3, // CI4\n"  // l:75
"    2, // CI8\n"  // l:76
"    0, // CIA4\n"  // l:77
"    0, 0, 0, // unused\n"  // l:78
"    3, // CMP\n"  // l:79
"    0  // unused\n"  // l:80
"};\n"  // l:81
"\n"  // l:82
"uint utemp;\n"  // l:83
"int itemp;\n"  // l:84
"\n"  // l:85
"uint index_from_pos(uint x, uint y, uint width, uint height, uint block_width_shft, uint block_height_shft) {\n"  // l:86
"    uint tile_x = x >> block_width_shft;\n"  // l:87
"    uint tile_y = y >> block_height_shft;\n"  // l:88
"    x &= (1u << block_width_shft)  - 1u;  // in-tile\n"  // l:89
"    y &= (1u << block_height_shft) - 1u;  // in-tile\n"  // l:90
"\n"  // l:91
"    return (tile_y * (width >> block_width_shft) + tile_x) * (1 << (block_width_shft + block_height_shft)) /* (tileW * tileH) */\n"  // l:92
"            + (y << block_width_shft) + x;\n"  // l:93
"}\n"  // l:94
"\n"  // l:95
"void main()\n"  // l:96
"{\n"  // l:97
"    if ((textureData & 1u) == 0) {\n"  // l:98
"        // no texture present\n"  // l:99
"        FragColor = vec4(vertexColor);\n"  // l:100
"    }\n"  // l:101
"    else {\n"  // l:102
"        uint tex_index = bitfieldExtract(textureData, 1, 3);\n"  // l:103
"\n"  // l:104
"        uint setimage0_i;  // holds width/height data\n"  // l:105
"        if (tex_index < 4) {\n"  // l:106
"            setimage0_i = BP_regs[0x88 + tex_index];\n"  // l:107
"        }\n"  // l:108
"        else {\n"  // l:109
"            setimage0_i = BP_regs[0xa8 + tex_index - 4];\n"  // l:110
"        }\n"  // l:111
"\n"  // l:112
"        uint texture_color_format = bitfieldExtract(setimage0_i, 20, 4);\n"  // l:113
"        uint width = bitfieldExtract(setimage0_i, 0, 10) + 1;\n"  // l:114
"        uint height = bitfieldExtract(setimage0_i, 10, 10) + 1;\n"  // l:115
"\n"  // l:116
"        // todo: wrap_s/t\n"  // l:117
"        highp vec2 wrappedCoord = clamp(texCoord, 0.0, 1.0);\n"  // l:118
"        uint textureIndex = textureOffset;\n"  // l:119
"\n"  // l:120
"        // always use nearest interpolation\n"  // l:121
"        /*\n"  // l:122
"        Textures are put in blocks:\n"  // l:123
"        https://fgiesen.wordpress.com/2011/01/17/texture-tiling-and-swizzling/\n"  // l:124
"        */\n"  // l:125
"        uint x = uint(wrappedCoord.x * width);\n"  // l:126
"        uint y = uint(wrappedCoord.y * height);\n"  // l:127
"\n"  // l:128
"        uint offset_into_texture;\n"  // l:129
"        uint data;\n"  // l:130
"        if (texture_color_format != 6) {\n"  // l:131
"            offset_into_texture = index_from_pos(x, y,\n"  // l:132
"                width, height,\n"  // l:133
"                tex_fmt_block_width_shft[texture_color_format], tex_fmt_block_height_shft[texture_color_format]\n"  // l:134
"            );\n"  // l:135
"            offset_into_texture <<= tex_fmt_shft[texture_color_format];\n"  // l:136
"            offset_into_texture >>= 1;\n"  // l:137
"\n"  // l:138
"            textureIndex += offset_into_texture;\n"  // l:139
"\n"  // l:140
"            // load data at the location. The color formats are all nicely aligned (either by 4, 2, 1 or 0.5 byte, so we\n"  // l:141
"            // can parse that part before parsing the actual color.\n"  // l:142
"            data = texture_data[textureIndex >> 2];  // stored bytes as uints\n"  // l:143
"\n"  // l:144
"            switch (tex_fmt_shft[texture_color_format]) {\n"  // l:145
"                case 0:  // nibble\n"  // l:146
"                case 1:  // byte\n"  // l:147
"                    utemp = bitfieldExtract(offset_into_texture, 0, 2);  // todo: nibble-sized offset for 0\n"  // l:148
"                    data = bitfieldExtract(data, int(utemp * 8), 8);\n"  // l:149
"                    break;\n"  // l:150
"                case 2:  // halfword\n"  // l:151
"                    utemp = offset_into_texture & 0x2u;  // misalignment (always halfword-aligned)\n"  // l:152
"                    data = bitfieldExtract(data, int(utemp * 8), 16);  // get right halfword\n"  // l:153
"                    utemp = data >> 8;  // top byte\n"  // l:154
"                    data = (data & 0xffu) << 8;  // swap bottom byte\n"  // l:155
"                    data = data | utemp;  // convert to LE\n"  // l:156
"                    break;\n"  // l:157
"                case 3:  // word\n"  // l:158
"                    utemp = data >> 16;  // top halfword\n"  // l:159
"                    data = (data & 0xffffu) << 16;  // swap bottom halfword\n"  // l:160
"                    data |= utemp;\n"  // l:161
"                    // we now did [0123] -> [2301], need [3210]\n"  // l:162
"\n"  // l:163
"                    utemp = data & 0x00ff00ffu;  // [.3.1]\n"  // l:164
"                    utemp <<= 8;                 // [3.1.]\n"  // l:165
"                    data >>= 8;                  // [.230]\n"  // l:166
"                    data &= 0x00ff00ffu;         // [.2.0]\n"  // l:167
"                    data |= utemp;               // [3210]\n"  // l:168
"                    break;\n"  // l:169
"                default:\n"  // l:170
"                    break;\n"  // l:171
"            }\n"  // l:172
"        }\n"  // l:173
"        else {\n"  // l:174
"            // data separated into 2 blocks:\n"  // l:175
"            /*\n"  // l:176
"            The RGBA32 format (or RGBA8 as it is sometimes known) is used to store 24 bit depth True Color\n"  // l:177
"            (1 byte per color), with an 8 bit alpha channel.\n"  // l:178
"            Although the pixel data does follow the block order as seen in other formats,\n"  // l:179
"            the data is separated into 2 groups. A and R are encoded in this order in the first group,\n"  // l:180
"            and G and B in the second group.\n"  // l:181
"            So one block in this format (4x4 pixels), as 64 bytes, appears in this order:\n"  // l:182
"\n"  // l:183
"            ARARARARARARARAR (<- 16 bytes)\n"  // l:184
"            ARARARARARARARAR\n"  // l:185
"            GBGBGBGBGBGBGBGB\n"  // l:186
"            GBGBGBGBGBGBGBGB\n"  // l:187
"            */\n"  // l:188
"            // blocks is still 4x4 pixels\n"  // l:189
"            uint tile_x = x >> 2;\n"  // l:190
"            uint tile_y = y >> 2;\n"  // l:191
"            uint i_x = x & 3u;  // in-tile\n"  // l:192
"            uint i_y = y & 3u;  // in-tile\n"  // l:193
"\n"  // l:194
"            // shift by 2 here for 4 bytes per full color in block\n"  // l:195
"            uint block_offset = (tile_y * (width >> 2) + tile_x) * 16 << 2;\n"  // l:196
"\n"  // l:197
"            // within the block though, all colors are in 2 separate blocks\n"  // l:198
"            uint AR_offset = ((i_y << 2) + i_x) << 1;\n"  // l:199
"            uint AR = texture_data[(block_offset + AR_offset) >> 2];  // stored as uints\n"  // l:200
"\n"  // l:201
"            // GB is half a block further\n"  // l:202
"            uint GB_offset = AR_offset + 32;\n"  // l:203
"            uint GB = texture_data[(block_offset + GB_offset) >> 2];  // stored as uints\n"  // l:204
"\n"  // l:205
"            if ((AR_offset & 2u) != 0) {\n"  // l:206
"                // misaligned reads\n"  // l:207
"                AR >>= 16;\n"  // l:208
"                GB >>= 16;\n"  // l:209
"            }\n"  // l:210
"            AR &= 0xffffu;\n"  // l:211
"            GB &= 0xffffu;\n"  // l:212
"\n"  // l:213
"            data = (AR << 16) | GB;\n"  // l:214
"        }\n"  // l:215
"\n"  // l:216
"        vec4 color = vec4(0, 0, 0, 1.0);\n"  // l:217
"\n"  // l:218
"        switch (texture_color_format) {\n"  // l:219
"            // todo: proper parsing (all modes)\n"  // l:220
"            case 1:\n"  // l:221
"                color = vec4(data, data, data, 0xff);\n"  // l:222
"                color *= 2 * vertexColor;\n"  // l:223
"                color /= 255.0;\n"  // l:224
"                break;\n"  // l:225
"\n"  // l:226
"            case 5:\n"  // l:227
"                if ((data & 0x8000u) != 0) {\n"  // l:228
"                    // \"normal\" RGB555\n"  // l:229
"                    color.x = bitfieldExtract(data, 10, 5);\n"  // l:230
"                    color.y = bitfieldExtract(data, 5, 5);\n"  // l:231
"                    color.z = bitfieldExtract(data, 0, 5);\n"  // l:232
"                    color.w = 32.0;\n"  // l:233
"                    color /= 32.0;\n"  // l:234
"                }\n"  // l:235
"                else {\n"  // l:236
"                    // RGB4A3\n"  // l:237
"                    color.x = bitfieldExtract(data, 8, 4);\n"  // l:238
"                    color.y = bitfieldExtract(data, 4, 4);\n"  // l:239
"                    color.z = bitfieldExtract(data, 0, 4);\n"  // l:240
"                    color.w = bitfieldExtract(data, 12, 3);\n"  // l:241
"                    color.w *= 2;  // one bit less\n"  // l:242
"                    color /= 16.0;\n"  // l:243
"                }\n"  // l:244
"                break;\n"  // l:245
"            case 6:\n"  // l:246
"                color = unpackUnorm4x8(data).yxwz;  // big endian caused this to be RABG\n"  // l:247
"                break;\n"  // l:248
"            default:\n"  // l:249
"                color = vec4(1.0, 0.0, 1.0, 1.0);\n"  // l:250
"                break;\n"  // l:251
"        }\n"  // l:252
"\n"  // l:253
"        // debugging stuff:\n"  // l:254
"//        if (height_min_1 == 0x7f) {\n"  // l:255
"//            FragColor = vec4(0.0, 1.0, 0.0, 1.0);\n"  // l:256
"//        }\n"  // l:257
"//        else {\n"  // l:258
"//            FragColor = vec4(0.0, 0.0, 1.0, 1.0);\n"  // l:259
"//        }\n"  // l:260
"\n"  // l:261
"//        if (color.w < 0.1) {\n"  // l:262
"//            discard;\n"  // l:263
"//        }\n"  // l:264
"\n"  // l:265
"        FragColor = color; // vec4(color.xyz, 0.5);\n"  // l:266
"        // FragColor = vec4(float(offset_into_texture) / (height_min_1 * width_min_1 * 2), 0.0, 0.0, 1.0);\n"  // l:267
"    }\n"  // l:268
"}\n"  // l:269
"\n"  // l:270
;


// framebufferFragmentShaderSource (from framebuffer_fragment.glsl, lines 2 to 49)
const char* framebufferFragmentShaderSource = 
"#version 430\n"  // l:1
"\n"  // l:2
"varying in vec2 texCoord;\n"  // l:3
"\n"  // l:4
"out vec4 FragColor;\n"  // l:5
"\n"  // l:6
"uniform uint efb_width;\n"  // l:7
"uniform sampler2D efb;\n"  // l:8
"/*\n"  // l:9
" * This is a fragment shader to handle the \"overlay\" caused from writes to the XFB\n"  // l:10
" * The idea is to discard certain fragments based on the\n"  // l:11
" * */\n"  // l:12
"\n"  // l:13
"void main() {\n"  // l:14
"    vec4 pixel_pair = texture(efb, texCoord);\n"  // l:15
"    float Cb = pixel_pair.y;\n"  // l:16
"    float Cr = pixel_pair.w;\n"  // l:17
"    float Y;\n"  // l:18
"\n"  // l:19
"    if ((uint(texCoord.x * efb_width) & 1u) == 0) {\n"  // l:20
"        // left pixel\n"  // l:21
"        Y = pixel_pair.x;\n"  // l:22
"    }\n"  // l:23
"    else {\n"  // l:24
"        // right pixel\n"  // l:25
"        Y = pixel_pair.z;\n"  // l:26
"    }\n"  // l:27
"\n"  // l:28
"    if (all(lessThan(\n"  // l:29
"        abs(vec3(\n"  // l:30
"            Y - (float(89) / 255.0),\n"  // l:31
"            Cb - (float(164) / 255.0),\n"  // l:32
"            Cr - (float(137) / 255.0)\n"  // l:33
"        )),\n"  // l:34
"        vec3(1.5 / 255.0, 1.5 / 255.0, 1.5 / 255.0)\n"  // l:35
"    ))) {\n"  // l:36
"        discard;  // impossible color\n"  // l:37
"    }\n"  // l:38
"\n"  // l:39
"    FragColor = vec4(\n"  // l:40
"        Y + 1.317 * (Cr - 0.5),\n"  // l:41
"        Y - 0.698 * (Cr - 0.5) - 0.336 * (Cb - 0.5),\n"  // l:42
"        Y + 1.732 * (Cr - 0.5),\n"  // l:43
"        1.0\n"  // l:44
"    );\n"  // l:45
"}\n"  // l:46
"\n"  // l:47
;


// framebufferVertexShaderSource (from framebuffer_vertex.glsl, lines 2 to 16)
const char* framebufferVertexShaderSource = 
"#version 430\n"  // l:1
"\n"  // l:2
"layout (location = 0) in vec2 position;\n"  // l:3
"layout (location = 1) in vec2 vtx_texCoord;\n"  // l:4
"\n"  // l:5
"out vec2 texCoord;\n"  // l:6
"\n"  // l:7
"void main() {\n"  // l:8
"    gl_Position = vec4(position, 0.0, 1.0);\n"  // l:9
"\n"  // l:10
"    // flip vertically\n"  // l:11
"    texCoord = vec2(vtx_texCoord.x, 1.0 - vtx_texCoord.y);\n"  // l:12
"}\n"  // l:13
"\n"  // l:14
;


// transformationShaderSource (from trafo.glsl, lines 2 to 91)
const char* transformationShaderSource = 
"#version 430 core\n"  // l:1
"\n"  // l:2
"#undef DEBUG\n"  // l:3
"\n"  // l:4
"layout (std430, binding = 4) readonly buffer XF_SSBO\n"  // l:5
"{\n"  // l:6
"    float XF_A[0x100];\n"  // l:7
"    float XF_B[0x100];\n"  // l:8
"    float XF_C[0x100];\n"  // l:9
"    float XF_D[0x100];\n"  // l:10
"    float XF_regs[0x58];  // some regs are not floats, but most are\n"  // l:11
"};\n"  // l:12
"\n"  // l:13
"vec4 transform_pos(vec3 position, uint posidx) {\n"  // l:14
"    // this seems to be what libOGC makes from projection matrices\n"  // l:15
"    mat4 projection;\n"  // l:16
"    if (floatBitsToUint(XF_regs[0x26]) == 0) {\n"  // l:17
"        // perspective matrix\n"  // l:18
"        projection = mat4(\n"  // l:19
"            XF_regs[0x20], 0, 0, 0,  // first column\n"  // l:20
"            0, XF_regs[0x22], 0, 0,  // second column\n"  // l:21
"            XF_regs[0x21], XF_regs[0x23], XF_regs[0x24], -1,  // third column\n"  // l:22
"            0, 0, XF_regs[0x25], 0   // fourth column\n"  // l:23
"        );\n"  // l:24
"    }\n"  // l:25
"    else {\n"  // l:26
"        // orthographic projection matrix\n"  // l:27
"        projection = mat4(\n"  // l:28
"            XF_regs[0x20], 0, 0, 0,  // first column\n"  // l:29
"            0, XF_regs[0x22], 0, 0,  // second column\n"  // l:30
"            0, 0, XF_regs[0x24], 0, // third column\n"  // l:31
"            XF_regs[0x21], XF_regs[0x23], XF_regs[0x25], 1  // fourth column\n"  // l:32
"        );\n"  // l:33
"    }\n"  // l:34
"\n"  // l:35
"    // matrices can be accessed per row\n"  // l:36
"    uint posmtx_base = posidx << 2;\n"  // l:37
"    mat4 modelview = mat4(\n"  // l:38
"        XF_A[posmtx_base + 0], XF_A[posmtx_base + 4], XF_A[posmtx_base + 8], 0,\n"  // l:39
"        XF_A[posmtx_base + 1], XF_A[posmtx_base + 5], XF_A[posmtx_base + 9], 0,\n"  // l:40
"        XF_A[posmtx_base + 2], XF_A[posmtx_base + 6], XF_A[posmtx_base + 10], 0,\n"  // l:41
"        XF_A[posmtx_base + 3], XF_A[posmtx_base + 7], XF_A[posmtx_base + 11], 1\n"  // l:42
"    );\n"  // l:43
"\n"  // l:44
"    vec4 pos = vec4(position, 1);\n"  // l:45
"    pos = projection * modelview * pos;\n"  // l:46
"    pos /= pos.w;\n"  // l:47
"    pos.z = 1 + pos.z;  // openGL is weird\n"  // l:48
"\n"  // l:49
"#ifdef DEBUG\n"  // l:50
"    if (false) {\n"  // l:51
"        switch (gl_VertexID) {\n"  // l:52
"            case 0:\n"  // l:53
"                pos = vec4(-1.0, -0.9, 0.0, 1.0);  // bottom left\n"  // l:54
"                break;\n"  // l:55
"            case 1:\n"  // l:56
"                pos = vec4(pos.x, pos.y, pos.z, 1.0);  // top right\n"  // l:57
"                break;\n"  // l:58
"            case 2:\n"  // l:59
"                pos = vec4(-0.9, -1.0, 0.0, 1.0);  // bottom right\n"  // l:60
"                break;\n"  // l:61
"            case 3:\n"  // l:62
"                pos = vec4(-1.0, -1.0, 0.0, 1.0);  // top left\n"  // l:63
"                break;\n"  // l:64
"            default:\n"  // l:65
"                break;\n"  // l:66
"        }\n"  // l:67
"    }\n"  // l:68
"#endif\n"  // l:69
"\n"  // l:70
"    return pos;\n"  // l:71
"}\n"  // l:72
"\n"  // l:73
"vec3 transform_tex(vec3 texcoord, uint texidx) {\n"  // l:74
"    // matrices can be accessed per row\n"  // l:75
"    uint texmtx_base = texidx << 2;\n"  // l:76
"    mat3 xf_mat = mat3(\n"  // l:77
"        XF_A[texmtx_base + 0], XF_A[texmtx_base + 4], XF_A[texmtx_base + 8],\n"  // l:78
"        XF_A[texmtx_base + 1], XF_A[texmtx_base + 5], XF_A[texmtx_base + 9],\n"  // l:79
"        XF_A[texmtx_base + 2], XF_A[texmtx_base + 6], XF_A[texmtx_base + 10]\n"  // l:80
"    );\n"  // l:81
"\n"  // l:82
"    vec3 coord = vec3(texcoord.xy, 1);\n"  // l:83
"    coord = xf_mat * coord;\n"  // l:84
"    coord /= coord.z;\n"  // l:85
"\n"  // l:86
"    return coord;\n"  // l:87
"}\n"  // l:88
"\n"  // l:89
;


// vertexShaderSource (from vertex.glsl, lines 2 to 482)
const char* vertexShaderSource = 
"#version 430 core\n"  // l:1
"\n"  // l:2
"#undef DEBUG\n"  // l:3
"\n"  // l:4
"const uint draw_arg_POS = 9;\n"  // l:5
"\n"  // l:6
"uniform uint VCD_lo, VCD_hi;\n"  // l:7
"uniform uint VAT_A, VAT_B, VAT_C;\n"  // l:8
"uniform uint MATIDX_REG_A, MATIDX_REG_B;\n"  // l:9
"\n"  // l:10
"// here, the array is read as little endian\n"  // l:11
"const int extract_offset[4] = { 0, 8, 16, 24 };\n"  // l:12
"\n"  // l:13
"layout (std430, binding = 3) readonly buffer command_SSBO\n"  // l:14
"{\n"  // l:15
"    uint vertex_stride;\n"  // l:16
"    int arg_offsets[21];\n"  // l:17
"    int data_offsets[21];\n"  // l:18
"    uint array_strides[21];\n"  // l:19
"    uint args[0x80000 >> 2];\n"  // l:20
"    uint data[];\n"  // l:21
"};\n"  // l:22
"\n"  // l:23
"out vec4 vertexColor;\n"  // l:24
"// signal that the vertex that was sent was invalid\n"  // l:25
"// this is used for vertices that are not supposed to connect for triangle strips/fans and linestrips\n"  // l:26
"// we signal this by filling args with 0xff for that vertex\n"  // l:27
"flat out uint invalidVertex;\n"  // l:28
"\n"  // l:29
"flat out uint textureData;\n"  // l:30
"flat out uint textureOffset;\n"  // l:31
"out highp vec2 texCoord;\n"  // l:32
"\n"  // l:33
"/*\n"  // l:34
" * I can't pass arrays to functions, so I'll have to make due writing separate read handlers for args/data\n"  // l:35
" * */\n"  // l:36
"#define read8(array) uint read8_ ## array ##(uint address) { return  bitfieldExtract(array[(address) >> 2], extract_offset[(address) & 3], 8); }\n"  // l:37
"#define read8s(array) int read8s_ ## array ##(uint address) { return  bitfieldExtract(int(array[(address) >> 2]), extract_offset[(address) & 3], 8); }\n"  // l:38
"\n"  // l:39
"// bitfieldExtract sign extends the data for us when we pass signed ints!\n"  // l:40
"#define read16(array) uint read16_ ## array ##(uint address) { \\\n"  // l:41
"    uint msb = read8_ ## array ##(address); \\\n"  // l:42
"    uint lsb = read8_ ## array ##(address + 1); \\\n"  // l:43
"    return (msb << 8) | lsb; \\\n"  // l:44
"}\n"  // l:45
"\n"  // l:46
"#define read16s(array) int read16s_ ## array ##(uint address) { \\\n"  // l:47
"    int msb = read8s_ ## array ##(address); \\\n"  // l:48
"    uint lsb = read8_ ## array ##(address + 1); \\\n"  // l:49
"    return (msb << 8) | int(lsb); \\\n"  // l:50
"}\n"  // l:51
"\n"  // l:52
"#define read32(array) uint read32_ ## array ##(uint address) { \\\n"  // l:53
"    uint msh = read16_ ## array ##(address); \\\n"  // l:54
"    uint lsh = read16_ ## array ##(address + 2); \\\n"  // l:55
"    return (msh << 16) | lsh; \\\n"  // l:56
"}\n"  // l:57
"\n"  // l:58
"#define read32s(array) int read32s_ ## array ##(uint address) { \\\n"  // l:59
"    int msh = read16s_ ## array ##(address); \\\n"  // l:60
"    uint lsh = read16_ ## array ##(address + 2); \\\n"  // l:61
"    return (msh << 16) | int(lsh); \\\n"  // l:62
"}\n"  // l:63
"\n"  // l:64
"read8(data)\n"  // l:65
"read8s(data)\n"  // l:66
"read8(args)\n"  // l:67
"read8s(args)\n"  // l:68
"\n"  // l:69
"read16(data)\n"  // l:70
"read16s(data)\n"  // l:71
"read16(args)\n"  // l:72
"read16s(args)\n"  // l:73
"\n"  // l:74
"read32(data)\n"  // l:75
"read32s(data)\n"  // l:76
"read32(args)\n"  // l:77
"read32s(args)\n"  // l:78
"\n"  // l:79
"// temporary stuff\n"  // l:80
"int stemp;\n"  // l:81
"uint utemp;\n"  // l:82
"\n"  // l:83
"// defined in trafo.glsl\n"  // l:84
"vec4 transform_pos(vec3 position, uint posidx);\n"  // l:85
"vec3 transform_tex(vec3 texcoord, uint texidx);\n"  // l:86
"\n"  // l:87
"vec3 load_position(bool from_data, uint FMT, uint offset)\n"  // l:88
"{\n"  // l:89
"    // always load 3 coodinates, decide on whether to use the last one later\n"  // l:90
"    vec3 position;\n"  // l:91
"    if (from_data) {\n"  // l:92
"        switch (FMT) {\n"  // l:93
"            case 0:  // u8\n"  // l:94
"                position.x = read8_data(offset);\n"  // l:95
"                position.y = read8_data(offset + 1);\n"  // l:96
"                position.z = read8_data(offset + 2);\n"  // l:97
"                break;\n"  // l:98
"            case 1:  // s8\n"  // l:99
"                position.x = read8s_data(offset);\n"  // l:100
"                position.y = read8s_data(offset + 1);\n"  // l:101
"                position.z = read8s_data(offset + 2);\n"  // l:102
"                break;\n"  // l:103
"            case 2:  // u16\n"  // l:104
"                position.x = read16_data(offset);\n"  // l:105
"                position.y = read16_data(offset + 2);\n"  // l:106
"                position.z = read16_data(offset + 4);\n"  // l:107
"                break;\n"  // l:108
"            case 3:  // s16\n"  // l:109
"                position.x = read16s_data(offset);\n"  // l:110
"                position.y = read16s_data(offset + 2);\n"  // l:111
"                position.z = read16s_data(offset + 4);\n"  // l:112
"                break;\n"  // l:113
"            case 4:\n"  // l:114
"                utemp = read32_data(offset);\n"  // l:115
"                position.x = uintBitsToFloat(utemp);\n"  // l:116
"                utemp = read32_data(offset + 4);\n"  // l:117
"                position.y = uintBitsToFloat(utemp);\n"  // l:118
"                utemp = read32_data(offset + 8);\n"  // l:119
"                position.z = uintBitsToFloat(utemp);\n"  // l:120
"                break;\n"  // l:121
"            default:\n"  // l:122
"                // invalid format\n"  // l:123
"                position = vec3(0, 0, 0);\n"  // l:124
"                break;\n"  // l:125
"        }\n"  // l:126
"        return position;\n"  // l:127
"    }\n"  // l:128
"    else {\n"  // l:129
"        // read data from args\n"  // l:130
"        // always load 3 coodinates, decide on whether to use the last one later\n"  // l:131
"        switch (FMT) {\n"  // l:132
"            case 0:  // u8\n"  // l:133
"                position.x = read8_args(offset);\n"  // l:134
"                position.y = read8_args(offset + 1);\n"  // l:135
"                position.z = read8_args(offset + 2);\n"  // l:136
"                break;\n"  // l:137
"            case 1:  // s8\n"  // l:138
"                position.x = read8s_args(offset);\n"  // l:139
"                position.y = read8s_args(offset + 1);\n"  // l:140
"                position.z = read8s_args(offset + 2);\n"  // l:141
"                break;\n"  // l:142
"            case 2:  // u16\n"  // l:143
"                position.x = read16_args(offset);\n"  // l:144
"                position.y = read16_args(offset + 2);\n"  // l:145
"                position.z = read16_args(offset + 4);\n"  // l:146
"                break;\n"  // l:147
"            case 3:  // s16\n"  // l:148
"                position.x = read16s_args(offset);\n"  // l:149
"                position.y = read16s_args(offset + 2);\n"  // l:150
"                position.z = read16s_args(offset + 4);\n"  // l:151
"                break;\n"  // l:152
"            case 4:\n"  // l:153
"                utemp = read32_args(offset);\n"  // l:154
"                position.x = uintBitsToFloat(utemp);\n"  // l:155
"                utemp = read32_args(offset + 4);\n"  // l:156
"                position.y = uintBitsToFloat(utemp);\n"  // l:157
"                utemp = read32_args(offset + 8);\n"  // l:158
"                position.z = uintBitsToFloat(utemp);\n"  // l:159
"                break;\n"  // l:160
"            default:\n"  // l:161
"                // invalid format\n"  // l:162
"                position = vec3(0, 0, 0);\n"  // l:163
"                break;\n"  // l:164
"        }\n"  // l:165
"        return position;\n"  // l:166
"    }\n"  // l:167
"}\n"  // l:168
"\n"  // l:169
"void main()\n"  // l:170
"{\n"  // l:171
"    // used for all types of parameters\n"  // l:172
"    uint arg_offset;\n"  // l:173
"    uint data_offset;\n"  // l:174
"\n"  // l:175
"    // placed here mostly for debugging purposes\n"  // l:176
"    vec3 position;\n"  // l:177
"\n"  // l:178
"    if (arg_offsets[9] >= 0)\n"  // l:179
"    {\n"  // l:180
"        /* determine vertex position */\n"  // l:181
"\n"  // l:182
"        // initial values\n"  // l:183
"        arg_offset = arg_offsets[9];\n"  // l:184
"        arg_offset += gl_VertexID * vertex_stride;\n"  // l:185
"\n"  // l:186
"        uint POSVCD  = bitfieldExtract(VCD_lo, 9, 2);\n"  // l:187
"        bool POSCNT  = bitfieldExtract(VAT_A, 0, 1) != 0;\n"  // l:188
"        uint POSFMT  = bitfieldExtract(VAT_A, 1, 3);\n"  // l:189
"        uint POSSHFT = bitfieldExtract(VAT_A, 4, 5);\n"  // l:190
"\n"  // l:191
"        if (POSVCD > 1) {\n"  // l:192
"            // indirect data\n"  // l:193
"            data_offset = data_offsets[9];\n"  // l:194
"\n"  // l:195
"            // determine the \"GC vertex index\"\n"  // l:196
"            int vertex_index;\n"  // l:197
"            if (POSVCD == 2) {\n"  // l:198
"                vertex_index = read8s_args(arg_offset);\n"  // l:199
"            }\n"  // l:200
"            else {\n"  // l:201
"                vertex_index = read16s_args(arg_offset);\n"  // l:202
"            }\n"  // l:203
"\n"  // l:204
"            data_offset += vertex_index * array_strides[9 - draw_arg_POS];\n"  // l:205
"\n"  // l:206
"            position = load_position(true, POSFMT, data_offset);\n"  // l:207
"        }\n"  // l:208
"        else {\n"  // l:209
"            position = load_position(false, POSFMT, arg_offset);\n"  // l:210
"        }\n"  // l:211
"\n"  // l:212
"        // todo: POSSHFT\n"  // l:213
"        if (!POSCNT) {\n"  // l:214
"            // 2D\n"  // l:215
"            position.z = 0;  // todo: what is this value supposed to be?\n"  // l:216
"        }\n"  // l:217
"\n"  // l:218
"        uint posidx;\n"  // l:219
"        if (arg_offsets[0] >= 0) {\n"  // l:220
"            // position matrix index value passed (always direct)\n"  // l:221
"            posidx = read8_args(arg_offsets[0]);\n"  // l:222
"        }\n"  // l:223
"        else {\n"  // l:224
"            posidx = bitfieldExtract(MATIDX_REG_A, 0, 6);\n"  // l:225
"        }\n"  // l:226
"\n"  // l:227
"        gl_Position = transform_pos(position, posidx);\n"  // l:228
"#ifdef DEBUG\n"  // l:229
"//        if (gl_Position.z < 0) {\n"  // l:230
"//            switch (gl_VertexID) {\n"  // l:231
"//                case 0:\n"  // l:232
"//                    gl_Position = vec4(-0.5, -0.5, 0.0, 1.0);\n"  // l:233
"//                    break;\n"  // l:234
"//                case 1:\n"  // l:235
"//                    gl_Position = vec4(0.0, -0.5, 0.0, 1.0);\n"  // l:236
"//                    break;\n"  // l:237
"//                case 2:\n"  // l:238
"//                    gl_Position = vec4(0.5, 0.5, 0.0, 1.0);\n"  // l:239
"//                    break;\n"  // l:240
"//                case 3:\n"  // l:241
"//                    gl_Position = vec4(0.0, 0.5, 0.0, 1.0);\n"  // l:242
"//                    break;\n"  // l:243
"//            }\n"  // l:244
"//        }\n"  // l:245
"#endif\n"  // l:246
"    }\n"  // l:247
"\n"  // l:248
"    if (arg_offsets[11] >= 0)\n"  // l:249
"    {\n"  // l:250
"        /* determine vertex color 0 */\n"  // l:251
"        // todo: different cases (right now only i8 indexed rgba8888\n"  // l:252
"        // initial values\n"  // l:253
"        arg_offset = arg_offsets[11];\n"  // l:254
"        arg_offset += gl_VertexID * vertex_stride;\n"  // l:255
"\n"  // l:256
"        uint COL0VCD  = bitfieldExtract(VCD_lo, 13, 2);\n"  // l:257
"        bool COL0CNT  = bitfieldExtract(VAT_A, 13, 1) != 0;\n"  // l:258
"        uint COL0FMT  = bitfieldExtract(VAT_A, 14, 3);\n"  // l:259
"\n"  // l:260
"        vec4 color;\n"  // l:261
"\n"  // l:262
"        if (COL0VCD > 1) {\n"  // l:263
"            // indirect data\n"  // l:264
"            data_offset = data_offsets[11];\n"  // l:265
"\n"  // l:266
"            int color_index;\n"  // l:267
"            if (COL0VCD == 2) {\n"  // l:268
"                color_index = read8s_args(arg_offset);\n"  // l:269
"            }\n"  // l:270
"            else {\n"  // l:271
"                color_index = read16s_args(arg_offset);\n"  // l:272
"            }\n"  // l:273
"\n"  // l:274
"            data_offset += color_index * array_strides[11 - draw_arg_POS];\n"  // l:275
"\n"  // l:276
"            // always get 4 color value, determine actual value later\n"  // l:277
"            switch (COL0FMT) {\n"  // l:278
"                case 0:  // 16bit rgb565\n"  // l:279
"                    utemp = read16_data(data_offset);\n"  // l:280
"                    color.x = bitfieldExtract(utemp, 0, 5);\n"  // l:281
"                    color.y = bitfieldExtract(utemp, 4, 6);\n"  // l:282
"                    color.z = bitfieldExtract(utemp, 10, 5);\n"  // l:283
"                    color /= 32.0;\n"  // l:284
"                    color.y *= 0.5; // extra bit\n"  // l:285
"                    color.w = 1.0;\n"  // l:286
"                    break;\n"  // l:287
"                case 1:  // 24bit rgb888\n"  // l:288
"                    utemp = read32_data(data_offset);\n"  // l:289
"                    color = unpackUnorm4x8(utemp).wzyx;  // BE to LE\n"  // l:290
"                    color.w = 1.0;  // 3 colors\n"  // l:291
"                    // already normalized\n"  // l:292
"                    break;\n"  // l:293
"                case 2:  // 32bit rgb888x\n"  // l:294
"                    utemp = data[data_offset >> 2];  // data is 4 aligned\n"  // l:295
"                    color = unpackUnorm4x8(utemp);\n"  // l:296
"                    color.w = 1.0;  // 3 colors\n"  // l:297
"                    // already normalized\n"  // l:298
"                    break;\n"  // l:299
"                case 3:  // 16bit rgba4444\n"  // l:300
"                    utemp = read16_data(data_offset);\n"  // l:301
"                    color.x = bitfieldExtract(utemp, 0, 4);\n"  // l:302
"                    color.y = bitfieldExtract(utemp, 3, 4);\n"  // l:303
"                    color.z = bitfieldExtract(utemp, 7, 4);\n"  // l:304
"                    color.w = bitfieldExtract(utemp, 11, 4);\n"  // l:305
"                    color /= 16.0;  // normalize\n"  // l:306
"                    break;\n"  // l:307
"                case 4:  // 24bit rgba6666\n"  // l:308
"                    utemp = read32_data(data_offset);\n"  // l:309
"                    color.x = bitfieldExtract(utemp, 0, 6);\n"  // l:310
"                    color.y = bitfieldExtract(utemp, 5, 6);\n"  // l:311
"                    color.z = bitfieldExtract(utemp, 11, 6);\n"  // l:312
"                    color.w = bitfieldExtract(utemp, 17, 6);\n"  // l:313
"                    color /= 64.0;  // normalize\n"  // l:314
"                    break;\n"  // l:315
"                case 5:  // 32bit rgba8888\n"  // l:316
"                    utemp = data[data_offset >> 2];  // data is 4 aligned\n"  // l:317
"                    color = unpackUnorm4x8(utemp);\n"  // l:318
"                    // already normalized\n"  // l:319
"                    break;\n"  // l:320
"                default:\n"  // l:321
"                    break;\n"  // l:322
"            }\n"  // l:323
"        }\n"  // l:324
"        else {\n"  // l:325
"            switch (COL0FMT) {\n"  // l:326
"                case 0:  // 16bit rgb565\n"  // l:327
"                    utemp = read16_args(arg_offset);\n"  // l:328
"                    color.x = bitfieldExtract(utemp, 0, 5);\n"  // l:329
"                    color.y = bitfieldExtract(utemp, 4, 6);\n"  // l:330
"                    color.z = bitfieldExtract(utemp, 10, 5);\n"  // l:331
"                    color /= 32.0;\n"  // l:332
"                    color.y *= 0.5;  // extra bit\n"  // l:333
"                    color.w = 1.0;\n"  // l:334
"                    break;\n"  // l:335
"                case 1:  // 24bit rgb888\n"  // l:336
"                case 2:  // 32bit rgb888x\n"  // l:337
"                    utemp = read32_args(arg_offset);\n"  // l:338
"                    color = unpackUnorm4x8(utemp).wzyx;  // BE to LE\n"  // l:339
"                    color.w = 1.0;  // 3 colors\n"  // l:340
"                    // already normalized\n"  // l:341
"                    break;\n"  // l:342
"                case 3:  // 16bit rgba4444\n"  // l:343
"                    utemp = read16_args(arg_offset);\n"  // l:344
"                    color.x = bitfieldExtract(utemp, 0, 4);\n"  // l:345
"                    color.y = bitfieldExtract(utemp, 3, 4);\n"  // l:346
"                    color.z = bitfieldExtract(utemp, 7, 4);\n"  // l:347
"                    color.w = bitfieldExtract(utemp, 11, 4);\n"  // l:348
"                    color /= 16.0;  // normalize\n"  // l:349
"                    break;\n"  // l:350
"                case 4:  // 24bit rgba6666\n"  // l:351
"                    utemp = read32_args(arg_offset);\n"  // l:352
"                    color.x = bitfieldExtract(utemp, 0, 6);\n"  // l:353
"                    color.y = bitfieldExtract(utemp, 5, 6);\n"  // l:354
"                    color.z = bitfieldExtract(utemp, 11, 6);\n"  // l:355
"                    color.w = bitfieldExtract(utemp, 17, 6);\n"  // l:356
"                    color /= 64.0;  // normalize\n"  // l:357
"                    break;\n"  // l:358
"                case 5:  // 32bit rgba8888\n"  // l:359
"                    utemp = read32_args(arg_offset);\n"  // l:360
"                    color = unpackUnorm4x8(utemp).wzyx;  // BE to LE\n"  // l:361
"                    // already normalized\n"  // l:362
"                    break;\n"  // l:363
"                default:\n"  // l:364
"                    break;\n"  // l:365
"            }\n"  // l:366
"        }\n"  // l:367
"\n"  // l:368
"        // todo: I am not sure if COLCNT really does anything, but just to be sure\n"  // l:369
"        vertexColor = color;\n"  // l:370
"        if (!COL0CNT) color.w = 1.0;\n"  // l:371
"#ifdef DEBUG\n"  // l:372
"//        if (position.y > 29) {\n"  // l:373
"//            vertexColor = vec4(0.0, 1.0, 0.0, 1.0);\n"  // l:374
"//        }\n"  // l:375
"//        else {\n"  // l:376
"//            vertexColor = vec4(1.0, 0.0, 0.0, 1.0);\n"  // l:377
"//        }\n"  // l:378
"#endif\n"  // l:379
"    }\n"  // l:380
"\n"  // l:381
"    /* load texture data */\n"  // l:382
"    textureData = 0;\n"  // l:383
"    for (int i = 0; i < 8; i++) {\n"  // l:384
"        if (arg_offsets[13 + i] >= 0) {\n"  // l:385
"            // load texture specific data\n"  // l:386
"            textureData = 1u | (uint(i) << 1);\n"  // l:387
"            textureOffset = data_offsets[i];\n"  // l:388
"\n"  // l:389
"            // load texture coordinate (same as position basically)\n"  // l:390
"            arg_offset = arg_offsets[13 + i];\n"  // l:391
"            arg_offset += gl_VertexID * vertex_stride;\n"  // l:392
"\n"  // l:393
"            uint TEXVCD = bitfieldExtract(VCD_hi, 2 * i, 2);\n"  // l:394
"            bool TEXCNT;\n"  // l:395
"            uint TEXFMT;\n"  // l:396
"            uint TEXSHFT;\n"  // l:397
"            switch (i) {\n"  // l:398
"                case 0:\n"  // l:399
"                    // TEX0 is in VAT_A\n"  // l:400
"                    TEXCNT = bitfieldExtract(VAT_A, 21, 1) != 0;\n"  // l:401
"                    TEXFMT = bitfieldExtract(VAT_A, 22, 3);\n"  // l:402
"                    TEXSHFT = bitfieldExtract(VAT_A, 25, 5);\n"  // l:403
"                    break;\n"  // l:404
"                case 1:\n"  // l:405
"                case 2:\n"  // l:406
"                case 3:\n"  // l:407
"                    // TEX1-3 are fully in VAT_B\n"  // l:408
"                    TEXCNT = bitfieldExtract(VAT_B, 9 * (i - 1), 1) != 0;\n"  // l:409
"                    TEXFMT = bitfieldExtract(VAT_B, 9 * (i - 1) + 1, 3);\n"  // l:410
"                    TEXSHFT = bitfieldExtract(VAT_B, 9 * (i - 1) + 4, 5);\n"  // l:411
"                    break;\n"  // l:412
"                case 4:\n"  // l:413
"                    // TEX4 is partly in VAT_B, partly in VAT_C\n"  // l:414
"                    TEXCNT = bitfieldExtract(VAT_B, 27, 1) != 0;\n"  // l:415
"                    TEXFMT = bitfieldExtract(VAT_B, 28, 3);\n"  // l:416
"                    TEXSHFT = bitfieldExtract(VAT_C, 0, 5);\n"  // l:417
"                    break;\n"  // l:418
"                case 5:\n"  // l:419
"                case 6:\n"  // l:420
"                case 7:\n"  // l:421
"                    // TEX5-7 are fully in VAT_C\n"  // l:422
"                    TEXCNT = bitfieldExtract(VAT_C, 5 + 9 * (i - 5), 1) != 0;\n"  // l:423
"                    TEXFMT = bitfieldExtract(VAT_C, 5 + 9 * (i - 5) + 1, 3);\n"  // l:424
"                    TEXSHFT = bitfieldExtract(VAT_C, 5 + 9 * (i - 5) + 4, 5);\n"  // l:425
"                default:\n"  // l:426
"                    // invalid texture format\n"  // l:427
"                    break;\n"  // l:428
"            }\n"  // l:429
"\n"  // l:430
"            vec3 read_tex_coord;\n"  // l:431
"\n"  // l:432
"            if (TEXVCD > 1) {\n"  // l:433
"                // indirect data\n"  // l:434
"                data_offset = data_offsets[13 + i];\n"  // l:435
"\n"  // l:436
"                // determine the GC texture coordiante index\n"  // l:437
"                int tex_coord_index;\n"  // l:438
"                if (TEXVCD == 2) {\n"  // l:439
"                    tex_coord_index = read8s_args(arg_offset);\n"  // l:440
"                }\n"  // l:441
"                else {\n"  // l:442
"                    tex_coord_index = read16s_args(arg_offset);\n"  // l:443
"                }\n"  // l:444
"\n"  // l:445
"                data_offset += tex_coord_index * array_strides[13 + i - draw_arg_POS];\n"  // l:446
"\n"  // l:447
"                read_tex_coord = load_position(true, TEXFMT, data_offset);\n"  // l:448
"            }\n"  // l:449
"            else {\n"  // l:450
"                read_tex_coord = load_position(false, TEXFMT, arg_offset);\n"  // l:451
"            }\n"  // l:452
"\n"  // l:453
"            read_tex_coord.z = 0;  // 2D at most\n"  // l:454
"            if (!TEXCNT) {\n"  // l:455
"                read_tex_coord.y = 0;  // 1D\n"  // l:456
"            }\n"  // l:457
"\n"  // l:458
"            uint texidx;\n"  // l:459
"            if (arg_offsets[1 + i] >= 0) {\n"  // l:460
"                // texture matrix index value passed (always direct)\n"  // l:461
"                texidx = read8_args(arg_offsets[1 + i]);\n"  // l:462
"            }\n"  // l:463
"            else {\n"  // l:464
"                if (i < 4)  {\n"  // l:465
"                    // TEX0-3 in MATIDX_REG_A\n"  // l:466
"                    texidx = bitfieldExtract(MATIDX_REG_A, 6 * (i + 1), 6);\n"  // l:467
"                }\n"  // l:468
"                else {\n"  // l:469
"                    // TEX4-7 in MATIDX_REG_B\n"  // l:470
"                    texidx = bitfieldExtract(MATIDX_REG_B, 6 * (i - 4), 6);\n"  // l:471
"                }\n"  // l:472
"            }\n"  // l:473
"\n"  // l:474
"            texCoord = transform_tex(read_tex_coord, texidx).xy;\n"  // l:475
"            break;\n"  // l:476
"        }\n"  // l:477
"    }\n"  // l:478
"}\n"  // l:479
"\n"  // l:480
;

#endif  // GC__SHADER_H