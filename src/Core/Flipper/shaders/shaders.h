#ifndef GC__SHADER_H
#define GC__SHADER_H

// fragmentShaderSource (from fragment.glsl, lines 2 to 227)
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
"const uint tex_fmt_shft[16] = {\n"  // l:31
"    0, // I4 ?\n"  // l:32
"    1, // I8 ?\n"  // l:33
"    0, // IA4 ?\n"  // l:34
"    1, // IA8 ?\n"  // l:35
"    2, // RGB565\n"  // l:36
"    2, // RGB5A3\n"  // l:37
"    3, // RGBA8\n"  // l:38
"    0, // unused\n"  // l:39
"    0, // CI4 ?\n"  // l:40
"    1, // CI8 ?\n"  // l:41
"    0, // CIA4 ?\n"  // l:42
"    0, 0, 0, // unused\n"  // l:43
"    1, // CMP ?\n"  // l:44
"    0  // unused\n"  // l:45
"};\n"  // l:46
"\n"  // l:47
"uint utemp;\n"  // l:48
"int itemp;\n"  // l:49
"\n"  // l:50
"uint index_from_pos(uint x, uint y, uint width, uint height) {\n"  // l:51
"    // blocks are always 4x4 pixels\n"  // l:52
"    uint tile_x = x >> 2;\n"  // l:53
"    uint tile_y = y >> 2;\n"  // l:54
"    x &= 3u;  // in-tile\n"  // l:55
"    y &= 3u;  // in-tile\n"  // l:56
"\n"  // l:57
"    return (tile_y * (width >> 2) + tile_x) * 16 /* 16 = (tileW * tileH) */ + (y << 2) + x;\n"  // l:58
"}\n"  // l:59
"\n"  // l:60
"void main()\n"  // l:61
"{\n"  // l:62
"    if ((textureData & 1u) == 0) {\n"  // l:63
"        // no texture present\n"  // l:64
"        FragColor = vec4(vertexColor);\n"  // l:65
"    }\n"  // l:66
"    else {\n"  // l:67
"        uint tex_index = bitfieldExtract(textureData, 1, 3);\n"  // l:68
"\n"  // l:69
"        uint setimage0_i;  // holds width/height data\n"  // l:70
"        if (tex_index < 4) {\n"  // l:71
"            setimage0_i = BP_regs[0x88 + tex_index];\n"  // l:72
"        }\n"  // l:73
"        else {\n"  // l:74
"            setimage0_i = BP_regs[0xa8 + tex_index - 4];\n"  // l:75
"        }\n"  // l:76
"\n"  // l:77
"        uint texture_color_format = bitfieldExtract(setimage0_i, 20, 4);\n"  // l:78
"        uint width = bitfieldExtract(setimage0_i, 0, 10) + 1;\n"  // l:79
"        uint height = bitfieldExtract(setimage0_i, 10, 10) + 1;\n"  // l:80
"\n"  // l:81
"        // todo: wrap_s/t\n"  // l:82
"        highp vec2 wrappedCoord = clamp(texCoord, 0.0, 1.0);\n"  // l:83
"        uint textureIndex = textureOffset;\n"  // l:84
"\n"  // l:85
"        // always use nearest interpolation\n"  // l:86
"        /*\n"  // l:87
"        Textures are put in blocks:\n"  // l:88
"        https://fgiesen.wordpress.com/2011/01/17/texture-tiling-and-swizzling/\n"  // l:89
"        */\n"  // l:90
"        uint x = uint(wrappedCoord.x * width);\n"  // l:91
"        uint y = uint(wrappedCoord.y * height);\n"  // l:92
"\n"  // l:93
"        uint offset_into_texture;\n"  // l:94
"        uint data;\n"  // l:95
"        if (texture_color_format != 6) {\n"  // l:96
"            offset_into_texture = index_from_pos(x, y, width, height);\n"  // l:97
"            offset_into_texture <<= tex_fmt_shft[texture_color_format];\n"  // l:98
"            offset_into_texture >>= 1;\n"  // l:99
"\n"  // l:100
"            textureIndex += offset_into_texture;\n"  // l:101
"\n"  // l:102
"            // load data at the location. The color formats are all nicely aligned (either by 4, 2, 1 or 0.5 byte, so we\n"  // l:103
"            // can parse that part before parsing the actual color.\n"  // l:104
"            data = texture_data[textureIndex >> 2];  // stored bytes as uints\n"  // l:105
"\n"  // l:106
"            switch (tex_fmt_shft[texture_color_format]) {\n"  // l:107
"                case 0:  // nibble\n"  // l:108
"                case 1:  // byte\n"  // l:109
"                    utemp = bitfieldExtract(offset_into_texture, 0, 2);  // todo: nibble-sized offset for 0\n"  // l:110
"                    data = bitfieldExtract(data, int(utemp * 8), 8);\n"  // l:111
"                    break;\n"  // l:112
"                case 2:  // halfword\n"  // l:113
"                    utemp = offset_into_texture & 0x2u;  // misalignment (always halfword-aligned)\n"  // l:114
"                    data = bitfieldExtract(data, int(utemp * 8), 16);  // get right halfword\n"  // l:115
"                    utemp = data >> 8;  // top byte\n"  // l:116
"                    data = (data & 0xffu) << 8;  // swap bottom byte\n"  // l:117
"                    data = data | utemp;  // convert to LE\n"  // l:118
"                    break;\n"  // l:119
"                case 3:  // word\n"  // l:120
"                    utemp = data >> 16;  // top halfword\n"  // l:121
"                    data = (data & 0xffffu) << 16;  // swap bottom halfword\n"  // l:122
"                    data |= utemp;\n"  // l:123
"                    // we now did [0123] -> [2301], need [3210]\n"  // l:124
"\n"  // l:125
"                    utemp = data & 0x00ff00ffu;  // [.3.1]\n"  // l:126
"                    utemp <<= 8;                 // [3.1.]\n"  // l:127
"                    data >>= 8;                  // [.230]\n"  // l:128
"                    data &= 0x00ff00ffu;         // [.2.0]\n"  // l:129
"                    data |= utemp;               // [3210]\n"  // l:130
"                    break;\n"  // l:131
"                default:\n"  // l:132
"                    break;\n"  // l:133
"            }\n"  // l:134
"        }\n"  // l:135
"        else {\n"  // l:136
"            // data separated into 2 blocks:\n"  // l:137
"            /*\n"  // l:138
"            The RGBA32 format (or RGBA8 as it is sometimes known) is used to store 24 bit depth True Color\n"  // l:139
"            (1 byte per color), with an 8 bit alpha channel.\n"  // l:140
"            Although the pixel data does follow the block order as seen in other formats,\n"  // l:141
"            the data is separated into 2 groups. A and R are encoded in this order in the first group,\n"  // l:142
"            and G and B in the second group.\n"  // l:143
"            So one block in this format (4x4 pixels), as 64 bytes, appears in this order:\n"  // l:144
"\n"  // l:145
"            ARARARARARARARAR (<- 16 bytes)\n"  // l:146
"            ARARARARARARARAR\n"  // l:147
"            GBGBGBGBGBGBGBGB\n"  // l:148
"            GBGBGBGBGBGBGBGB\n"  // l:149
"            */\n"  // l:150
"            // blocks is still 4x4 pixels\n"  // l:151
"            uint tile_x = x >> 2;\n"  // l:152
"            uint tile_y = y >> 2;\n"  // l:153
"            uint i_x = x & 3u;  // in-tile\n"  // l:154
"            uint i_y = y & 3u;  // in-tile\n"  // l:155
"\n"  // l:156
"            // shift by 2 here for 4 bytes per full color in block\n"  // l:157
"            uint block_offset = (tile_y * (width >> 2) + tile_x) * 16 << 2;\n"  // l:158
"\n"  // l:159
"            // within the block though, all colors are in 2 separate blocks\n"  // l:160
"            uint AR_offset = ((i_y << 2) + i_x) << 1;\n"  // l:161
"            uint AR = texture_data[(block_offset + AR_offset) >> 2];  // stored as uints\n"  // l:162
"\n"  // l:163
"            // GB is half a block further\n"  // l:164
"            uint GB_offset = AR_offset + 32;\n"  // l:165
"            uint GB = texture_data[(block_offset + GB_offset) >> 2];  // stored as uints\n"  // l:166
"\n"  // l:167
"            if ((AR_offset & 2u) != 0) {\n"  // l:168
"                // misaligned reads\n"  // l:169
"                AR >>= 16;\n"  // l:170
"                GB >>= 16;\n"  // l:171
"            }\n"  // l:172
"            AR &= 0xffffu;\n"  // l:173
"            GB &= 0xffffu;\n"  // l:174
"\n"  // l:175
"            data = (AR << 16) | GB;\n"  // l:176
"        }\n"  // l:177
"\n"  // l:178
"        vec4 color = vec4(0, 0, 0, 1.0);\n"  // l:179
"\n"  // l:180
"        switch (texture_color_format) {\n"  // l:181
"            // todo: proper parsing (2 modes)\n"  // l:182
"            case 5:\n"  // l:183
"                if ((data & 0x8000u) != 0) {\n"  // l:184
"                    // \"normal\" RGB555\n"  // l:185
"                    color.x = bitfieldExtract(data, 10, 5);\n"  // l:186
"                    color.y = bitfieldExtract(data, 5, 5);\n"  // l:187
"                    color.z = bitfieldExtract(data, 0, 5);\n"  // l:188
"                    color /= 32.0;\n"  // l:189
"                }\n"  // l:190
"                else {\n"  // l:191
"                    // RGB4A3\n"  // l:192
"                    color.x = bitfieldExtract(data, 8, 4);\n"  // l:193
"                    color.y = bitfieldExtract(data, 4, 4);\n"  // l:194
"                    color.z = bitfieldExtract(data, 0, 4);\n"  // l:195
"                    color.w = bitfieldExtract(data, 12, 3);\n"  // l:196
"                    color.w *= 2;  // one bit less\n"  // l:197
"                    color /= 16.0;\n"  // l:198
"                }\n"  // l:199
"                break;\n"  // l:200
"            case 6:\n"  // l:201
"                color = unpackUnorm4x8(data).yxwz;  // big endian caused this to be RABG\n"  // l:202
"                break;\n"  // l:203
"            default:\n"  // l:204
"                color = vec4(1.0, 0.0, 1.0, 1.0);\n"  // l:205
"                break;\n"  // l:206
"        }\n"  // l:207
"\n"  // l:208
"        // debugging stuff:\n"  // l:209
"//        if (height_min_1 == 0x7f) {\n"  // l:210
"//            FragColor = vec4(0.0, 1.0, 0.0, 1.0);\n"  // l:211
"//        }\n"  // l:212
"//        else {\n"  // l:213
"//            FragColor = vec4(0.0, 0.0, 1.0, 1.0);\n"  // l:214
"//        }\n"  // l:215
"\n"  // l:216
"//        if (color.w < 0.1) {\n"  // l:217
"//            discard;\n"  // l:218
"//        }\n"  // l:219
"\n"  // l:220
"        FragColor = color;\n"  // l:221
"        // FragColor = vec4(float(offset_into_texture) / (height_min_1 * width_min_1 * 2), 0.0, 0.0, 1.0);\n"  // l:222
"    }\n"  // l:223
"}\n"  // l:224
"\n"  // l:225
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


// vertexShaderSource (from vertex.glsl, lines 2 to 481)
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
"    uint vertices;\n"  // l:16
"    uint _command;\n"  // l:17
"    uint vertex_stride;\n"  // l:18
"    int arg_offsets[21];\n"  // l:19
"    int data_offsets[21];\n"  // l:20
"    uint array_strides[21];\n"  // l:21
"    uint _cmd_data_size;  // data_size: I don't actually need this in the shader\n"  // l:22
"    uint args[0x1140 >> 2];     // todo: generalize this\n"  // l:23
"    uint data[];\n"  // l:24
"};\n"  // l:25
"\n"  // l:26
"out vec4 vertexColor;\n"  // l:27
"\n"  // l:28
"flat out uint textureData;\n"  // l:29
"flat out uint textureOffset;\n"  // l:30
"out highp vec2 texCoord;\n"  // l:31
"\n"  // l:32
"/*\n"  // l:33
" * I can't pass arrays to functions, so I'll have to make due writing separate read handlers for args/data\n"  // l:34
" * */\n"  // l:35
"#define read8(array) uint read8_ ## array ##(uint address) { return  bitfieldExtract(array[(address) >> 2], extract_offset[(address) & 3], 8); }\n"  // l:36
"#define read8s(array) int read8s_ ## array ##(uint address) { return  bitfieldExtract(int(array[(address) >> 2]), extract_offset[(address) & 3], 8); }\n"  // l:37
"\n"  // l:38
"// bitfieldExtract sign extends the data for us when we pass signed ints!\n"  // l:39
"#define read16(array) uint read16_ ## array ##(uint address) { \\\n"  // l:40
"    uint msb = read8_ ## array ##(address); \\\n"  // l:41
"    uint lsb = read8_ ## array ##(address + 1); \\\n"  // l:42
"    return (msb << 8) | lsb; \\\n"  // l:43
"}\n"  // l:44
"\n"  // l:45
"#define read16s(array) int read16s_ ## array ##(uint address) { \\\n"  // l:46
"    int msb = read8s_ ## array ##(address); \\\n"  // l:47
"    uint lsb = read8_ ## array ##(address + 1); \\\n"  // l:48
"    return (msb << 8) | int(lsb); \\\n"  // l:49
"}\n"  // l:50
"\n"  // l:51
"#define read32(array) uint read32_ ## array ##(uint address) { \\\n"  // l:52
"    uint msh = read16_ ## array ##(address); \\\n"  // l:53
"    uint lsh = read16_ ## array ##(address + 2); \\\n"  // l:54
"    return (msh << 16) | lsh; \\\n"  // l:55
"}\n"  // l:56
"\n"  // l:57
"#define read32s(array) int read32s_ ## array ##(uint address) { \\\n"  // l:58
"    int msh = read16s_ ## array ##(address); \\\n"  // l:59
"    uint lsh = read16_ ## array ##(address + 2); \\\n"  // l:60
"    return (msh << 16) | int(lsh); \\\n"  // l:61
"}\n"  // l:62
"\n"  // l:63
"read8(data)\n"  // l:64
"read8s(data)\n"  // l:65
"read8(args)\n"  // l:66
"read8s(args)\n"  // l:67
"\n"  // l:68
"read16(data)\n"  // l:69
"read16s(data)\n"  // l:70
"read16(args)\n"  // l:71
"read16s(args)\n"  // l:72
"\n"  // l:73
"read32(data)\n"  // l:74
"read32s(data)\n"  // l:75
"read32(args)\n"  // l:76
"read32s(args)\n"  // l:77
"\n"  // l:78
"// temporary stuff\n"  // l:79
"int stemp;\n"  // l:80
"uint utemp;\n"  // l:81
"\n"  // l:82
"// defined in trafo.glsl\n"  // l:83
"vec4 transform_pos(vec3 position, uint posidx);\n"  // l:84
"vec3 transform_tex(vec3 texcoord, uint texidx);\n"  // l:85
"\n"  // l:86
"vec3 load_position(bool from_data, uint FMT, uint offset)\n"  // l:87
"{\n"  // l:88
"    // always load 3 coodinates, decide on whether to use the last one later\n"  // l:89
"    vec3 position;\n"  // l:90
"    if (from_data) {\n"  // l:91
"        switch (FMT) {\n"  // l:92
"            case 0:  // u8\n"  // l:93
"                position.x = read8_data(offset);\n"  // l:94
"                position.y = read8_data(offset + 1);\n"  // l:95
"                position.z = read8_data(offset + 2);\n"  // l:96
"                break;\n"  // l:97
"            case 1:  // s8\n"  // l:98
"                position.x = read8s_data(offset);\n"  // l:99
"                position.y = read8s_data(offset + 1);\n"  // l:100
"                position.z = read8s_data(offset + 2);\n"  // l:101
"                break;\n"  // l:102
"            case 2:  // u16\n"  // l:103
"                position.x = read16_data(offset);\n"  // l:104
"                position.y = read16_data(offset + 2);\n"  // l:105
"                position.z = read16_data(offset + 4);\n"  // l:106
"                break;\n"  // l:107
"            case 3:  // s16\n"  // l:108
"                position.x = read16s_data(offset);\n"  // l:109
"                position.y = read16s_data(offset + 2);\n"  // l:110
"                position.z = read16s_data(offset + 4);\n"  // l:111
"                break;\n"  // l:112
"            case 4:\n"  // l:113
"                utemp = read32_data(offset);\n"  // l:114
"                position.x = uintBitsToFloat(utemp);\n"  // l:115
"                utemp = read32_data(offset + 4);\n"  // l:116
"                position.y = uintBitsToFloat(utemp);\n"  // l:117
"                utemp = read32_data(offset + 8);\n"  // l:118
"                position.z = uintBitsToFloat(utemp);\n"  // l:119
"                break;\n"  // l:120
"            default:\n"  // l:121
"                // invalid format\n"  // l:122
"                position = vec3(0, 0, 0);\n"  // l:123
"                break;\n"  // l:124
"        }\n"  // l:125
"        return position;\n"  // l:126
"    }\n"  // l:127
"    else {\n"  // l:128
"        // read data from args\n"  // l:129
"        // always load 3 coodinates, decide on whether to use the last one later\n"  // l:130
"        switch (FMT) {\n"  // l:131
"            case 0:  // u8\n"  // l:132
"                position.x = read8_args(offset);\n"  // l:133
"                position.y = read8_args(offset + 1);\n"  // l:134
"                position.z = read8_args(offset + 2);\n"  // l:135
"                break;\n"  // l:136
"            case 1:  // s8\n"  // l:137
"                position.x = read8s_args(offset);\n"  // l:138
"                position.y = read8s_args(offset + 1);\n"  // l:139
"                position.z = read8s_args(offset + 2);\n"  // l:140
"                break;\n"  // l:141
"            case 2:  // u16\n"  // l:142
"                position.x = read16_args(offset);\n"  // l:143
"                position.y = read16_args(offset + 2);\n"  // l:144
"                position.z = read16_args(offset + 4);\n"  // l:145
"                break;\n"  // l:146
"            case 3:  // s16\n"  // l:147
"                position.x = read16s_args(offset);\n"  // l:148
"                position.y = read16s_args(offset + 2);\n"  // l:149
"                position.z = read16s_args(offset + 4);\n"  // l:150
"                break;\n"  // l:151
"            case 4:\n"  // l:152
"                utemp = read32_args(offset);\n"  // l:153
"                position.x = uintBitsToFloat(utemp);\n"  // l:154
"                utemp = read32_args(offset + 4);\n"  // l:155
"                position.y = uintBitsToFloat(utemp);\n"  // l:156
"                utemp = read32_args(offset + 8);\n"  // l:157
"                position.z = uintBitsToFloat(utemp);\n"  // l:158
"                break;\n"  // l:159
"            default:\n"  // l:160
"                // invalid format\n"  // l:161
"                position = vec3(0, 0, 0);\n"  // l:162
"                break;\n"  // l:163
"        }\n"  // l:164
"        return position;\n"  // l:165
"    }\n"  // l:166
"}\n"  // l:167
"\n"  // l:168
"void main()\n"  // l:169
"{\n"  // l:170
"    // used for all types of parameters\n"  // l:171
"    uint arg_offset;\n"  // l:172
"    uint data_offset;\n"  // l:173
"\n"  // l:174
"    // placed here mostly for debugging purposes\n"  // l:175
"    vec3 position;\n"  // l:176
"\n"  // l:177
"    if (arg_offsets[9] >= 0)\n"  // l:178
"    {\n"  // l:179
"        /* determine vertex position */\n"  // l:180
"\n"  // l:181
"        // initial values\n"  // l:182
"        arg_offset = arg_offsets[9];\n"  // l:183
"        arg_offset += gl_VertexID * vertex_stride;\n"  // l:184
"\n"  // l:185
"        uint POSVCD  = bitfieldExtract(VCD_lo, 9, 2);\n"  // l:186
"        bool POSCNT  = bitfieldExtract(VAT_A, 0, 1) != 0;\n"  // l:187
"        uint POSFMT  = bitfieldExtract(VAT_A, 1, 3);\n"  // l:188
"        uint POSSHFT = bitfieldExtract(VAT_A, 4, 5);\n"  // l:189
"\n"  // l:190
"        if (POSVCD > 1) {\n"  // l:191
"            // indirect data\n"  // l:192
"            data_offset = data_offsets[9];\n"  // l:193
"\n"  // l:194
"            // determine the \"GC vertex index\"\n"  // l:195
"            int vertex_index;\n"  // l:196
"            if (POSVCD == 2) {\n"  // l:197
"                vertex_index = read8s_args(arg_offset);\n"  // l:198
"            }\n"  // l:199
"            else {\n"  // l:200
"                vertex_index = read16s_args(arg_offset);\n"  // l:201
"            }\n"  // l:202
"\n"  // l:203
"            data_offset += vertex_index * array_strides[9 - draw_arg_POS];\n"  // l:204
"\n"  // l:205
"            position = load_position(true, POSFMT, data_offset);\n"  // l:206
"        }\n"  // l:207
"        else {\n"  // l:208
"            position = load_position(false, POSFMT, arg_offset);\n"  // l:209
"        }\n"  // l:210
"\n"  // l:211
"        // todo: POSSHFT\n"  // l:212
"        if (!POSCNT) {\n"  // l:213
"            // 2D\n"  // l:214
"            position.z = 0;  // todo: what is this value supposed to be?\n"  // l:215
"        }\n"  // l:216
"\n"  // l:217
"        uint posidx;\n"  // l:218
"        if (arg_offsets[0] >= 0) {\n"  // l:219
"            // position matrix index value passed (always direct)\n"  // l:220
"            posidx = read8_args(arg_offsets[0]);\n"  // l:221
"        }\n"  // l:222
"        else {\n"  // l:223
"            posidx = bitfieldExtract(MATIDX_REG_A, 0, 6);\n"  // l:224
"        }\n"  // l:225
"\n"  // l:226
"        gl_Position = transform_pos(position, posidx);\n"  // l:227
"#ifdef DEBUG\n"  // l:228
"//        if (gl_Position.z < 0) {\n"  // l:229
"//            switch (gl_VertexID) {\n"  // l:230
"//                case 0:\n"  // l:231
"//                    gl_Position = vec4(-0.5, -0.5, 0.0, 1.0);\n"  // l:232
"//                    break;\n"  // l:233
"//                case 1:\n"  // l:234
"//                    gl_Position = vec4(0.0, -0.5, 0.0, 1.0);\n"  // l:235
"//                    break;\n"  // l:236
"//                case 2:\n"  // l:237
"//                    gl_Position = vec4(0.5, 0.5, 0.0, 1.0);\n"  // l:238
"//                    break;\n"  // l:239
"//                case 3:\n"  // l:240
"//                    gl_Position = vec4(0.0, 0.5, 0.0, 1.0);\n"  // l:241
"//                    break;\n"  // l:242
"//            }\n"  // l:243
"//        }\n"  // l:244
"#endif\n"  // l:245
"    }\n"  // l:246
"\n"  // l:247
"    if (arg_offsets[11] >= 0)\n"  // l:248
"    {\n"  // l:249
"        /* determine vertex color 0 */\n"  // l:250
"        // todo: different cases (right now only i8 indexed rgba8888\n"  // l:251
"        // initial values\n"  // l:252
"        arg_offset = arg_offsets[11];\n"  // l:253
"        arg_offset += gl_VertexID * vertex_stride;\n"  // l:254
"\n"  // l:255
"        uint COL0VCD  = bitfieldExtract(VCD_lo, 13, 2);\n"  // l:256
"        bool COL0CNT  = bitfieldExtract(VAT_A, 13, 1) != 0;\n"  // l:257
"        uint COL0FMT  = bitfieldExtract(VAT_A, 14, 3);\n"  // l:258
"\n"  // l:259
"        vec4 color;\n"  // l:260
"\n"  // l:261
"        if (COL0VCD > 1) {\n"  // l:262
"            // indirect data\n"  // l:263
"            data_offset = data_offsets[11];\n"  // l:264
"\n"  // l:265
"            int color_index;\n"  // l:266
"            if (COL0VCD == 2) {\n"  // l:267
"                color_index = read8s_args(arg_offset);\n"  // l:268
"            }\n"  // l:269
"            else {\n"  // l:270
"                color_index = read16s_args(arg_offset);\n"  // l:271
"            }\n"  // l:272
"\n"  // l:273
"            data_offset += color_index * array_strides[11 - draw_arg_POS];\n"  // l:274
"\n"  // l:275
"            // always get 4 color value, determine actual value later\n"  // l:276
"            switch (COL0FMT) {\n"  // l:277
"                case 0:  // 16bit rgb565\n"  // l:278
"                    utemp = read16_data(data_offset);\n"  // l:279
"                    color.x = bitfieldExtract(utemp, 0, 5);\n"  // l:280
"                    color.y = bitfieldExtract(utemp, 4, 6);\n"  // l:281
"                    color.z = bitfieldExtract(utemp, 10, 5);\n"  // l:282
"                    color /= 32.0;\n"  // l:283
"                    color.y *= 0.5; // extra bit\n"  // l:284
"                    color.w = 1.0;\n"  // l:285
"                    break;\n"  // l:286
"                case 1:  // 24bit rgb888\n"  // l:287
"                    utemp = read32_data(data_offset);\n"  // l:288
"                    color = unpackUnorm4x8(utemp).wzyx;  // BE to LE\n"  // l:289
"                    color.w = 1.0;  // 3 colors\n"  // l:290
"                    // already normalized\n"  // l:291
"                    break;\n"  // l:292
"                case 2:  // 32bit rgb888x\n"  // l:293
"                    utemp = data[data_offset >> 2];  // data is 4 aligned\n"  // l:294
"                    color = unpackUnorm4x8(utemp);\n"  // l:295
"                    color.w = 1.0;  // 3 colors\n"  // l:296
"                    // already normalized\n"  // l:297
"                    break;\n"  // l:298
"                case 3:  // 16bit rgba4444\n"  // l:299
"                    utemp = read16_data(data_offset);\n"  // l:300
"                    color.x = bitfieldExtract(utemp, 0, 4);\n"  // l:301
"                    color.y = bitfieldExtract(utemp, 3, 4);\n"  // l:302
"                    color.z = bitfieldExtract(utemp, 7, 4);\n"  // l:303
"                    color.w = bitfieldExtract(utemp, 11, 4);\n"  // l:304
"                    color /= 16.0;  // normalize\n"  // l:305
"                    break;\n"  // l:306
"                case 4:  // 24bit rgba6666\n"  // l:307
"                    utemp = read32_data(data_offset);\n"  // l:308
"                    color.x = bitfieldExtract(utemp, 0, 6);\n"  // l:309
"                    color.y = bitfieldExtract(utemp, 5, 6);\n"  // l:310
"                    color.z = bitfieldExtract(utemp, 11, 6);\n"  // l:311
"                    color.w = bitfieldExtract(utemp, 17, 6);\n"  // l:312
"                    color /= 64.0;  // normalize\n"  // l:313
"                    break;\n"  // l:314
"                case 5:  // 32bit rgba8888\n"  // l:315
"                    utemp = data[data_offset >> 2];  // data is 4 aligned\n"  // l:316
"                    color = unpackUnorm4x8(utemp);\n"  // l:317
"                    // already normalized\n"  // l:318
"                    break;\n"  // l:319
"                default:\n"  // l:320
"                    break;\n"  // l:321
"            }\n"  // l:322
"        }\n"  // l:323
"        else {\n"  // l:324
"            switch (COL0FMT) {\n"  // l:325
"                case 0:  // 16bit rgb565\n"  // l:326
"                    utemp = read16_args(arg_offset);\n"  // l:327
"                    color.x = bitfieldExtract(utemp, 0, 5);\n"  // l:328
"                    color.y = bitfieldExtract(utemp, 4, 6);\n"  // l:329
"                    color.z = bitfieldExtract(utemp, 10, 5);\n"  // l:330
"                    color /= 32.0;\n"  // l:331
"                    color.y *= 0.5;  // extra bit\n"  // l:332
"                    color.w = 1.0;\n"  // l:333
"                    break;\n"  // l:334
"                case 1:  // 24bit rgb888\n"  // l:335
"                case 2:  // 32bit rgb888x\n"  // l:336
"                    utemp = read32_args(arg_offset);\n"  // l:337
"                    color = unpackUnorm4x8(utemp).wzyx;  // BE to LE\n"  // l:338
"                    color.w = 1.0;  // 3 colors\n"  // l:339
"                    // already normalized\n"  // l:340
"                    break;\n"  // l:341
"                case 3:  // 16bit rgba4444\n"  // l:342
"                    utemp = read16_args(arg_offset);\n"  // l:343
"                    color.x = bitfieldExtract(utemp, 0, 4);\n"  // l:344
"                    color.y = bitfieldExtract(utemp, 3, 4);\n"  // l:345
"                    color.z = bitfieldExtract(utemp, 7, 4);\n"  // l:346
"                    color.w = bitfieldExtract(utemp, 11, 4);\n"  // l:347
"                    color /= 16.0;  // normalize\n"  // l:348
"                    break;\n"  // l:349
"                case 4:  // 24bit rgba6666\n"  // l:350
"                    utemp = read32_args(arg_offset);\n"  // l:351
"                    color.x = bitfieldExtract(utemp, 0, 6);\n"  // l:352
"                    color.y = bitfieldExtract(utemp, 5, 6);\n"  // l:353
"                    color.z = bitfieldExtract(utemp, 11, 6);\n"  // l:354
"                    color.w = bitfieldExtract(utemp, 17, 6);\n"  // l:355
"                    color /= 64.0;  // normalize\n"  // l:356
"                    break;\n"  // l:357
"                case 5:  // 32bit rgba8888\n"  // l:358
"                    utemp = read32_args(arg_offset);\n"  // l:359
"                    color = unpackUnorm4x8(utemp).wzyx;  // BE to LE\n"  // l:360
"                    // already normalized\n"  // l:361
"                    break;\n"  // l:362
"                default:\n"  // l:363
"                    break;\n"  // l:364
"            }\n"  // l:365
"        }\n"  // l:366
"\n"  // l:367
"        // todo: I am not sure if COLCNT really does anything, but just to be sure\n"  // l:368
"        vertexColor = color;\n"  // l:369
"        if (!COL0CNT) color.w = 1.0;\n"  // l:370
"#ifdef DEBUG\n"  // l:371
"//        if (position.y > 29) {\n"  // l:372
"//            vertexColor = vec4(0.0, 1.0, 0.0, 1.0);\n"  // l:373
"//        }\n"  // l:374
"//        else {\n"  // l:375
"//            vertexColor = vec4(1.0, 0.0, 0.0, 1.0);\n"  // l:376
"//        }\n"  // l:377
"#endif\n"  // l:378
"    }\n"  // l:379
"\n"  // l:380
"    /* load texture data */\n"  // l:381
"    textureData = 0;\n"  // l:382
"    for (int i = 0; i < 8; i++) {\n"  // l:383
"        if (arg_offsets[13 + i] >= 0) {\n"  // l:384
"            // load texture specific data\n"  // l:385
"            textureData = 1u | (uint(i) << 1);\n"  // l:386
"            textureOffset = data_offsets[i];\n"  // l:387
"\n"  // l:388
"            // load texture coordinate (same as position basically)\n"  // l:389
"            arg_offset = arg_offsets[13 + i];\n"  // l:390
"            arg_offset += gl_VertexID * vertex_stride;\n"  // l:391
"\n"  // l:392
"            uint TEXVCD = bitfieldExtract(VCD_hi, 2 * i, 2);\n"  // l:393
"            bool TEXCNT;\n"  // l:394
"            uint TEXFMT;\n"  // l:395
"            uint TEXSHFT;\n"  // l:396
"            switch (i) {\n"  // l:397
"                case 0:\n"  // l:398
"                    // TEX0 is in VAT_A\n"  // l:399
"                    TEXCNT = bitfieldExtract(VAT_A, 21, 1) != 0;\n"  // l:400
"                    TEXFMT = bitfieldExtract(VAT_A, 22, 3);\n"  // l:401
"                    TEXSHFT = bitfieldExtract(VAT_A, 25, 5);\n"  // l:402
"                    break;\n"  // l:403
"                case 1:\n"  // l:404
"                case 2:\n"  // l:405
"                case 3:\n"  // l:406
"                    // TEX1-3 are fully in VAT_B\n"  // l:407
"                    TEXCNT = bitfieldExtract(VAT_B, 9 * (i - 1), 1) != 0;\n"  // l:408
"                    TEXFMT = bitfieldExtract(VAT_B, 9 * (i - 1) + 1, 3);\n"  // l:409
"                    TEXSHFT = bitfieldExtract(VAT_B, 9 * (i - 1) + 4, 5);\n"  // l:410
"                    break;\n"  // l:411
"                case 4:\n"  // l:412
"                    // TEX4 is partly in VAT_B, partly in VAT_C\n"  // l:413
"                    TEXCNT = bitfieldExtract(VAT_B, 27, 1) != 0;\n"  // l:414
"                    TEXFMT = bitfieldExtract(VAT_B, 28, 3);\n"  // l:415
"                    TEXSHFT = bitfieldExtract(VAT_C, 0, 5);\n"  // l:416
"                    break;\n"  // l:417
"                case 5:\n"  // l:418
"                case 6:\n"  // l:419
"                case 7:\n"  // l:420
"                    // TEX5-7 are fully in VAT_C\n"  // l:421
"                    TEXCNT = bitfieldExtract(VAT_C, 5 + 9 * (i - 5), 1) != 0;\n"  // l:422
"                    TEXFMT = bitfieldExtract(VAT_C, 5 + 9 * (i - 5) + 1, 3);\n"  // l:423
"                    TEXSHFT = bitfieldExtract(VAT_C, 5 + 9 * (i - 5) + 4, 5);\n"  // l:424
"                default:\n"  // l:425
"                    // invalid texture format\n"  // l:426
"                    break;\n"  // l:427
"            }\n"  // l:428
"\n"  // l:429
"            vec3 read_tex_coord;\n"  // l:430
"\n"  // l:431
"            if (TEXVCD > 1) {\n"  // l:432
"                // indirect data\n"  // l:433
"                data_offset = data_offsets[13 + i];\n"  // l:434
"\n"  // l:435
"                // determine the GC texture coordiante index\n"  // l:436
"                int tex_coord_index;\n"  // l:437
"                if (TEXVCD == 2) {\n"  // l:438
"                    tex_coord_index = read8s_args(arg_offset);\n"  // l:439
"                }\n"  // l:440
"                else {\n"  // l:441
"                    tex_coord_index = read16s_args(arg_offset);\n"  // l:442
"                }\n"  // l:443
"\n"  // l:444
"                data_offset += tex_coord_index * array_strides[13 + i - draw_arg_POS];\n"  // l:445
"\n"  // l:446
"                read_tex_coord = load_position(true, TEXFMT, data_offset);\n"  // l:447
"            }\n"  // l:448
"            else {\n"  // l:449
"                read_tex_coord = load_position(false, TEXFMT, arg_offset);\n"  // l:450
"            }\n"  // l:451
"\n"  // l:452
"            read_tex_coord.z = 0;  // 2D at most\n"  // l:453
"            if (!TEXCNT) {\n"  // l:454
"                read_tex_coord.y = 0;  // 1D\n"  // l:455
"            }\n"  // l:456
"\n"  // l:457
"            uint texidx;\n"  // l:458
"            if (arg_offsets[1 + i] >= 0) {\n"  // l:459
"                // texture matrix index value passed (always direct)\n"  // l:460
"                texidx = read8_args(arg_offsets[1 + i]);\n"  // l:461
"            }\n"  // l:462
"            else {\n"  // l:463
"                if (i < 4)  {\n"  // l:464
"                    // TEX0-3 in MATIDX_REG_A\n"  // l:465
"                    texidx = bitfieldExtract(MATIDX_REG_A, 6 * (i + 1), 6);\n"  // l:466
"                }\n"  // l:467
"                else {\n"  // l:468
"                    // TEX4-7 in MATIDX_REG_B\n"  // l:469
"                    texidx = bitfieldExtract(MATIDX_REG_B, 6 * (i - 4), 6);\n"  // l:470
"                }\n"  // l:471
"            }\n"  // l:472
"\n"  // l:473
"            texCoord = transform_tex(read_tex_coord, texidx).xy;\n"  // l:474
"            break;\n"  // l:475
"        }\n"  // l:476
"    }\n"  // l:477
"}\n"  // l:478
"\n"  // l:479
;

#endif  // GC__SHADER_H