#ifndef GC__SHADER_H
#define GC__SHADER_H

// fragmentShaderSource (from fragment.glsl, lines 2 to 24)
const char* fragmentShaderSource = 
"#version 400 core\n"  // l:1
"\n"  // l:2
"in vec4 vertexColor;\n"  // l:3
"flat in uint texturePresent;\n"  // l:4
"flat in uint textureOffset;     // start of texture into texture_data\n"  // l:5
"\n"  // l:6
"/*\n"  // l:7
"Idea: pass triangle vertex positions as flat in here (just check index %3 and pass 3 vertices)\n"  // l:8
"*/\n"  // l:9
"\n"  // l:10
"out vec4 FragColor;\n"  // l:11
"\n"  // l:12
"void main()\n"  // l:13
"{\n"  // l:14
"    if (texturePresent == 0) {\n"  // l:15
"        FragColor = vec4(vertexColor);\n"  // l:16
"    }\n"  // l:17
"    else {\n"  // l:18
"        FragColor = vec4(1.0, 0.0, 0.0, 1.0);\n"  // l:19
"    }\n"  // l:20
"}\n"  // l:21
"\n"  // l:22
;


// transformationShaderSource (from trafo.glsl, lines 2 to 38)
const char* transformationShaderSource = 
"#version 430 core\n"  // l:1
"\n"  // l:2
"layout (std430, binding = 4) buffer XF_SSBO\n"  // l:3
"{\n"  // l:4
"    float XF_A[0x100];\n"  // l:5
"    float XF_B[0x100];\n"  // l:6
"    float XF_C[0x100];\n"  // l:7
"    float XF_D[0x100];\n"  // l:8
"    float XF_regs[0x58];  // some regs are not floats, but most are\n"  // l:9
"};\n"  // l:10
"\n"  // l:11
"vec4 transform(vec3 position, uint posidx) {\n"  // l:12
"    // this seems to be what libOGC makes from projection matrices\n"  // l:13
"    mat4 projection = mat4(\n"  // l:14
"        XF_regs[0x20], 0, 0, 0,  // first column\n"  // l:15
"        0, XF_regs[0x22], 0, 0,  // second column\n"  // l:16
"        0, 0, XF_regs[0x24], -1,  // third column\n"  // l:17
"        XF_regs[0x21], XF_regs[0x23], XF_regs[0x25], 0  // fourth column\n"  // l:18
"    );\n"  // l:19
"\n"  // l:20
"    // matrices can be accessed per row\n"  // l:21
"    uint posmtx_base = posidx << 2;\n"  // l:22
"    mat4 modelview = mat4(\n"  // l:23
"        XF_A[posidx + 0], XF_A[posidx + 4], XF_A[posidx + 8], 0,\n"  // l:24
"        XF_A[posidx + 1], XF_A[posidx + 5], XF_A[posidx + 9], 0,\n"  // l:25
"        XF_A[posidx + 2], XF_A[posidx + 6], XF_A[posidx + 10], 0,\n"  // l:26
"        XF_A[posidx + 3], XF_A[posidx + 7], XF_A[posidx + 11], 1\n"  // l:27
"    );\n"  // l:28
"\n"  // l:29
"    vec4 pos = vec4(position, 1);\n"  // l:30
"    pos = projection * modelview * pos;\n"  // l:31
"    pos /= pos.w;\n"  // l:32
"\n"  // l:33
"    return pos;\n"  // l:34
"}\n"  // l:35
"\n"  // l:36
;


// vertexShaderSource (from vertex.glsl, lines 2 to 369)
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
"layout (std430, binding = 3) buffer command_SSBO\n"  // l:14
"{\n"  // l:15
"    uint vertices;\n"  // l:16
"    uint _command;\n"  // l:17
"    uint vertex_stride;\n"  // l:18
"    int arg_offsets[21];\n"  // l:19
"    int data_offsets[21];\n"  // l:20
"    uint array_strides[21];\n"  // l:21
"    uint _data_size;  // data_size: I don't actually need this in the shader\n"  // l:22
"    uint args[0x1140 >> 2];     // todo: generalize this\n"  // l:23
"    uint data[];\n"  // l:24
"};\n"  // l:25
"\n"  // l:26
"out vec4 vertexColor;\n"  // l:27
"\n"  // l:28
"out uint texturePresent;\n"  // l:29
"out uint textureOffset;\n"  // l:30
"\n"  // l:31
"/*\n"  // l:32
" * I can't pass arrays to functions, so I'll have to make due writing separate read handlers for args/data\n"  // l:33
" * */\n"  // l:34
"#define read8(array) uint read8_ ## array ##(uint address) { return  bitfieldExtract(array[(address) >> 2], extract_offset[(address) & 3], 8); }\n"  // l:35
"#define read8s(array) int read8s_ ## array ##(uint address) { return  bitfieldExtract(int(array[(address) >> 2]), extract_offset[(address) & 3], 8); }\n"  // l:36
"\n"  // l:37
"// bitfieldExtract sign extends the data for us when we pass signed ints!\n"  // l:38
"#define read16(array) uint read16_ ## array ##(uint address) { \\\n"  // l:39
"    uint msb = read8_ ## array ##(address); \\\n"  // l:40
"    uint lsb = read8_ ## array ##(address + 1); \\\n"  // l:41
"    return (msb << 8) | lsb; \\\n"  // l:42
"}\n"  // l:43
"\n"  // l:44
"#define read16s(array) int read16s_ ## array ##(uint address) { \\\n"  // l:45
"    int msb = read8s_ ## array ##(address); \\\n"  // l:46
"    uint lsb = read8_ ## array ##(address + 1); \\\n"  // l:47
"    return (msb << 8) | int(lsb); \\\n"  // l:48
"}\n"  // l:49
"\n"  // l:50
"#define read32(array) uint read32_ ## array ##(uint address) { \\\n"  // l:51
"    uint msh = read16_ ## array ##(address); \\\n"  // l:52
"    uint lsh = read16_ ## array ##(address + 2); \\\n"  // l:53
"    return (msh << 16) | lsh; \\\n"  // l:54
"}\n"  // l:55
"\n"  // l:56
"#define read32s(array) int read32s_ ## array ##(uint address) { \\\n"  // l:57
"    int msh = read16s_ ## array ##(address); \\\n"  // l:58
"    uint lsh = read16_ ## array ##(address + 2); \\\n"  // l:59
"    return (msh << 16) | int(lsh); \\\n"  // l:60
"}\n"  // l:61
"\n"  // l:62
"read8(data);\n"  // l:63
"read8s(data);\n"  // l:64
"read8(args);\n"  // l:65
"read8s(args);\n"  // l:66
"\n"  // l:67
"read16(data);\n"  // l:68
"read16s(data);\n"  // l:69
"read16(args);\n"  // l:70
"read16s(args);\n"  // l:71
"\n"  // l:72
"read32(data);\n"  // l:73
"read32s(data);\n"  // l:74
"read32(args);\n"  // l:75
"read32s(args);\n"  // l:76
"\n"  // l:77
"// defined in trafo.glsl\n"  // l:78
"vec4 transform(vec3 position, uint posidx);\n"  // l:79
"\n"  // l:80
"void main()\n"  // l:81
"{\n"  // l:82
"    // used for all types of parameters\n"  // l:83
"    uint arg_offset;\n"  // l:84
"    uint data_offset;\n"  // l:85
"\n"  // l:86
"    int stemp;\n"  // l:87
"    uint utemp;\n"  // l:88
"\n"  // l:89
"    // placed here mostly for debugging purposes\n"  // l:90
"    vec3 position;\n"  // l:91
"\n"  // l:92
"    if (arg_offsets[9] >= 0)\n"  // l:93
"    {\n"  // l:94
"        /* determine vertex position */\n"  // l:95
"\n"  // l:96
"        // initial values\n"  // l:97
"        arg_offset = arg_offsets[9];\n"  // l:98
"        arg_offset += gl_VertexID * vertex_stride;\n"  // l:99
"\n"  // l:100
"        uint POSVCD  = bitfieldExtract(VCD_lo, 9, 2);\n"  // l:101
"        bool POSCNT  = bitfieldExtract(VAT_A, 0, 1) != 0;\n"  // l:102
"        uint POSFMT  = bitfieldExtract(VAT_A, 1, 3);\n"  // l:103
"        uint POSSHFT = bitfieldExtract(VAT_A, 4, 5);\n"  // l:104
"\n"  // l:105
"        if (POSVCD > 1) {\n"  // l:106
"            // indirect data\n"  // l:107
"            data_offset = data_offsets[9];\n"  // l:108
"\n"  // l:109
"            // determine the \"GC vertex index\"\n"  // l:110
"            int vertex_index;\n"  // l:111
"            if (POSVCD == 2) {\n"  // l:112
"                vertex_index = read8s_args(arg_offset);\n"  // l:113
"            }\n"  // l:114
"            else {\n"  // l:115
"                vertex_index = read16s_args(arg_offset);\n"  // l:116
"            }\n"  // l:117
"            // todo: pass strides\n"  // l:118
"            data_offset += vertex_index * array_strides[9 - draw_arg_POS];\n"  // l:119
"\n"  // l:120
"            // always load 3 coodinates, decide on whether to use the last one later\n"  // l:121
"            switch (POSFMT) {\n"  // l:122
"                case 0:  // u8\n"  // l:123
"                    position.x = read8_data(data_offset);\n"  // l:124
"                    position.y = read8_data(data_offset + 1);\n"  // l:125
"                    position.z = read8_data(data_offset + 2);\n"  // l:126
"                    break;\n"  // l:127
"                case 1:  // s8\n"  // l:128
"                    position.x = read8s_data(data_offset);\n"  // l:129
"                    position.y = read8s_data(data_offset + 1);\n"  // l:130
"                    position.z = read8s_data(data_offset + 2);\n"  // l:131
"                    break;\n"  // l:132
"                case 2:  // u16\n"  // l:133
"                    position.x = read16_data(data_offset);\n"  // l:134
"                    position.y = read16_data(data_offset + 2);\n"  // l:135
"                    position.z = read16_data(data_offset + 4);\n"  // l:136
"                    break;\n"  // l:137
"                case 3:  // s16\n"  // l:138
"                    position.x = read16s_data(data_offset);\n"  // l:139
"                    position.y = read16s_data(data_offset + 2);\n"  // l:140
"                    position.z = read16s_data(data_offset + 4);\n"  // l:141
"                    break;\n"  // l:142
"                case 4:\n"  // l:143
"                    utemp = read32_data(data_offset);\n"  // l:144
"                    position.x = uintBitsToFloat(utemp);\n"  // l:145
"                    utemp = read32_data(data_offset + 4);\n"  // l:146
"                    position.y = uintBitsToFloat(utemp);\n"  // l:147
"                    utemp = read32_data(data_offset + 8);\n"  // l:148
"                    position.z = uintBitsToFloat(utemp);\n"  // l:149
"                    break;\n"  // l:150
"                default:\n"  // l:151
"                    // invalid format\n"  // l:152
"                    return;\n"  // l:153
"            }\n"  // l:154
"        }\n"  // l:155
"        else {\n"  // l:156
"            // read data from args\n"  // l:157
"            // always load 3 coodinates, decide on whether to use the last one later\n"  // l:158
"            switch (POSFMT) {\n"  // l:159
"                case 0:  // u8\n"  // l:160
"                    position.x = read8_args(arg_offset);\n"  // l:161
"                    position.y = read8_args(arg_offset + 1);\n"  // l:162
"                    position.z = read8_args(arg_offset + 2);\n"  // l:163
"                    break;\n"  // l:164
"                case 1:  // s8\n"  // l:165
"                    position.x = read8s_args(arg_offset);\n"  // l:166
"                    position.y = read8s_args(arg_offset + 1);\n"  // l:167
"                    position.z = read8s_args(arg_offset + 2);\n"  // l:168
"                    break;\n"  // l:169
"                case 2:  // u16\n"  // l:170
"                    position.x = read16_args(arg_offset);\n"  // l:171
"                    position.y = read16_args(arg_offset + 2);\n"  // l:172
"                    position.z = read16_args(arg_offset + 4);\n"  // l:173
"                    break;\n"  // l:174
"                case 3:  // s16\n"  // l:175
"                    position.x = read16s_args(arg_offset);\n"  // l:176
"                    position.y = read16s_args(arg_offset + 2);\n"  // l:177
"                    position.z = read16s_args(arg_offset + 4);\n"  // l:178
"                    break;\n"  // l:179
"                case 4:\n"  // l:180
"                    utemp = read32_args(arg_offset);\n"  // l:181
"                    position.x = uintBitsToFloat(utemp);\n"  // l:182
"                    utemp = read32_args(arg_offset + 4);\n"  // l:183
"                    position.y = uintBitsToFloat(utemp);\n"  // l:184
"                    utemp = read32_args(arg_offset + 8);\n"  // l:185
"                    position.z = uintBitsToFloat(utemp);\n"  // l:186
"                    break;\n"  // l:187
"                default:\n"  // l:188
"                    // invalid format\n"  // l:189
"                    return;\n"  // l:190
"            }\n"  // l:191
"        }\n"  // l:192
"\n"  // l:193
"        // todo: POSSHFT\n"  // l:194
"        if (!POSCNT) {\n"  // l:195
"            // 2D\n"  // l:196
"            position.z = 0;  // todo: what is this value supposed to be?\n"  // l:197
"        }\n"  // l:198
"\n"  // l:199
"        uint posidx;\n"  // l:200
"        if (arg_offsets[0] >= 0) {\n"  // l:201
"            // position matrix index value passed (always direct)\n"  // l:202
"            posidx = read8_args(arg_offsets[0]);\n"  // l:203
"        }\n"  // l:204
"        else {\n"  // l:205
"            posidx = bitfieldExtract(MATIDX_REG_A, 0, 6);\n"  // l:206
"        }\n"  // l:207
"\n"  // l:208
"        gl_Position = transform(position, posidx);\n"  // l:209
"#ifdef DEBUG\n"  // l:210
"        switch (gl_VertexID) {\n"  // l:211
"            case 0:\n"  // l:212
"                gl_Position = vec4(-0.5, -0.5, 0.0, 1.0);\n"  // l:213
"                break;\n"  // l:214
"            case 1:\n"  // l:215
"                gl_Position = vec4(0.0, -0.5, 0.0, 1.0);\n"  // l:216
"                break;\n"  // l:217
"            case 2:\n"  // l:218
"                gl_Position = vec4(0.5, 0.5, 0.0, 1.0);\n"  // l:219
"                break;\n"  // l:220
"            case 3:\n"  // l:221
"                gl_Position = vec4(0.0, 0.5, 0.0, 1.0);\n"  // l:222
"                break;\n"  // l:223
"        }\n"  // l:224
"#endif\n"  // l:225
"    }\n"  // l:226
"\n"  // l:227
"    if (arg_offsets[11] >= 0)\n"  // l:228
"    {\n"  // l:229
"        /* determine vertex color 0 */\n"  // l:230
"        // todo: different cases (right now only i8 indexed rgba8888\n"  // l:231
"        // initial values\n"  // l:232
"        arg_offset = arg_offsets[11];\n"  // l:233
"        arg_offset += gl_VertexID * vertex_stride;\n"  // l:234
"\n"  // l:235
"        uint COL0VCD  = bitfieldExtract(VCD_lo, 13, 2);\n"  // l:236
"        bool COL0CNT  = bitfieldExtract(VAT_A, 13, 1) != 0;\n"  // l:237
"        uint COL0FMT  = bitfieldExtract(VAT_A, 14, 3);\n"  // l:238
"\n"  // l:239
"        vec4 color;\n"  // l:240
"\n"  // l:241
"        // todo: signed offset\n"  // l:242
"        if (COL0VCD > 1) {\n"  // l:243
"            // indirect data\n"  // l:244
"            data_offset = data_offsets[11];\n"  // l:245
"\n"  // l:246
"            int color_index;\n"  // l:247
"            if (COL0VCD == 2) {\n"  // l:248
"                color_index = read8s_args(arg_offset);\n"  // l:249
"            }\n"  // l:250
"            else {\n"  // l:251
"                color_index = read16s_args(arg_offset);\n"  // l:252
"            }\n"  // l:253
"\n"  // l:254
"            data_offset += color_index * array_strides[11 - draw_arg_POS];\n"  // l:255
"\n"  // l:256
"            // always get 4 color value, determine actual value later\n"  // l:257
"            switch (COL0FMT) {\n"  // l:258
"                case 0:  // 16bit rgb565\n"  // l:259
"                    utemp = read16_data(data_offset);\n"  // l:260
"                    color.x = bitfieldExtract(utemp, 0, 5)  / 32.0;\n"  // l:261
"                    color.y = bitfieldExtract(utemp, 4, 6)  / 64.0;\n"  // l:262
"                    color.z = bitfieldExtract(utemp, 10, 5) / 32.0;\n"  // l:263
"                    color.w = 1.0;\n"  // l:264
"                    break;\n"  // l:265
"                case 1:  // 24bit rgb888\n"  // l:266
"                    utemp = read32_data(data_offset);\n"  // l:267
"                    color = unpackUnorm4x8(utemp).wzyx;  // BE to LE\n"  // l:268
"                    color.w = 1.0;  // 3 colors\n"  // l:269
"                    // already normalized\n"  // l:270
"                    break;\n"  // l:271
"                case 2:  // 32bit rgb888x\n"  // l:272
"                    utemp = data[data_offset >> 2];  // data is 4 aligned\n"  // l:273
"                    color = unpackUnorm4x8(utemp);\n"  // l:274
"                    color.w = 1.0;  // 3 colors\n"  // l:275
"                    // already normalized\n"  // l:276
"                    break;\n"  // l:277
"                case 3:  // 16bit rgba4444\n"  // l:278
"                    utemp = read16_data(data_offset);\n"  // l:279
"                    color.x = bitfieldExtract(utemp, 0, 4);\n"  // l:280
"                    color.y = bitfieldExtract(utemp, 3, 4);\n"  // l:281
"                    color.z = bitfieldExtract(utemp, 7, 4);\n"  // l:282
"                    color.w = bitfieldExtract(utemp, 11, 4);\n"  // l:283
"                    color /= 16.0;  // normalize\n"  // l:284
"                    break;\n"  // l:285
"                case 4:  // 24bit rgba6666\n"  // l:286
"                    utemp = read32_data(data_offset);\n"  // l:287
"                    color.x = bitfieldExtract(utemp, 0, 6);\n"  // l:288
"                    color.y = bitfieldExtract(utemp, 5, 6);\n"  // l:289
"                    color.z = bitfieldExtract(utemp, 11, 6);\n"  // l:290
"                    color.w = bitfieldExtract(utemp, 17, 6);\n"  // l:291
"                    color /= 64.0;  // normalize\n"  // l:292
"                    break;\n"  // l:293
"                case 5:  // 32bit rgba8888\n"  // l:294
"                    utemp = data[data_offset >> 2];  // data is 4 aligned\n"  // l:295
"                    color = unpackUnorm4x8(utemp);\n"  // l:296
"                    // already normalized\n"  // l:297
"                    break;\n"  // l:298
"                default:\n"  // l:299
"                    break;\n"  // l:300
"            }\n"  // l:301
"        }\n"  // l:302
"        else {\n"  // l:303
"            switch (COL0FMT) {\n"  // l:304
"                case 0:  // 16bit rgb565\n"  // l:305
"                    utemp = read16_args(arg_offset);\n"  // l:306
"                    color.x = bitfieldExtract(utemp, 0, 5)  / 32.0;\n"  // l:307
"                    color.y = bitfieldExtract(utemp, 4, 6)  / 64.0;\n"  // l:308
"                    color.z = bitfieldExtract(utemp, 10, 5) / 32.0;\n"  // l:309
"                    color.w = 1.0;\n"  // l:310
"                    break;\n"  // l:311
"                case 1:  // 24bit rgb888\n"  // l:312
"                case 2:  // 32bit rgb888x\n"  // l:313
"                    utemp = read32_args(arg_offset);\n"  // l:314
"                    color = unpackUnorm4x8(utemp).wzyx;  // BE to LE\n"  // l:315
"                    color.w = 1.0;  // 3 colors\n"  // l:316
"                    // already normalized\n"  // l:317
"                    break;\n"  // l:318
"                case 3:  // 16bit rgba4444\n"  // l:319
"                    utemp = read16_args(arg_offset);\n"  // l:320
"                    color.x = bitfieldExtract(utemp, 0, 4);\n"  // l:321
"                    color.y = bitfieldExtract(utemp, 3, 4);\n"  // l:322
"                    color.z = bitfieldExtract(utemp, 7, 4);\n"  // l:323
"                    color.w = bitfieldExtract(utemp, 11, 4);\n"  // l:324
"                    color /= 16.0;  // normalize\n"  // l:325
"                    break;\n"  // l:326
"                case 4:  // 24bit rgba6666\n"  // l:327
"                    utemp = read32_args(arg_offset);\n"  // l:328
"                    color.x = bitfieldExtract(utemp, 0, 6);\n"  // l:329
"                    color.y = bitfieldExtract(utemp, 5, 6);\n"  // l:330
"                    color.z = bitfieldExtract(utemp, 11, 6);\n"  // l:331
"                    color.w = bitfieldExtract(utemp, 17, 6);\n"  // l:332
"                    color /= 64.0;  // normalize\n"  // l:333
"                    break;\n"  // l:334
"                case 5:  // 32bit rgba8888\n"  // l:335
"                    utemp = read32_args(arg_offset);\n"  // l:336
"                    color = unpackUnorm4x8(utemp).wzyx;  // BE to LE\n"  // l:337
"                    // already normalized\n"  // l:338
"                    break;\n"  // l:339
"                default:\n"  // l:340
"                    break;\n"  // l:341
"            }\n"  // l:342
"        }\n"  // l:343
"\n"  // l:344
"        // todo: I am not sure if COLCNT really does anything, but just to be sure\n"  // l:345
"        vertexColor = color;\n"  // l:346
"        if (!COL0CNT) color.w = 1.0;\n"  // l:347
"#ifdef DEBUG\n"  // l:348
"        if (position.y > 29) {\n"  // l:349
"            vertexColor = vec4(0.0, 1.0, 0.0, 1.0);\n"  // l:350
"        }\n"  // l:351
"        else {\n"  // l:352
"            vertexColor = vec4(1.0, 0.0, 0.0, 1.0);\n"  // l:353
"        }\n"  // l:354
"#endif\n"  // l:355
"    }\n"  // l:356
"\n"  // l:357
"    texturePresent = 0;\n"  // l:358
"    for (int i = 0; i < 8; i++) {\n"  // l:359
"        if (arg_offsets[13 + i] >= 0) {\n"  // l:360
"            texturePresent = 1;\n"  // l:361
"            textureOffset = data_offsets[i];\n"  // l:362
"            break;\n"  // l:363
"        }\n"  // l:364
"    }\n"  // l:365
"}\n"  // l:366
"\n"  // l:367
;

#endif  // GC__SHADER_H