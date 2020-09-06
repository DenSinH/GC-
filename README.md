# GC-

This is probably going to take a while to finish...

#### Memos to self:

required settings:
 - CMake options: `-DCMAKE_TOOLCHAIN_FILE=D:\vcpkg\vcpkg\scripts\buildsystems\vcpkg.cmake`
 
### Todo:

 - `__builtin_bswap32` for memory reads
 
### Instructions
#### Integer Arithmetic Instructions
 - [x] addx
 - [x] addcx
 - [x] addex
 - [x] addi
 - [x] addic
 - [x] addic.
 - [x] addis
 - [ ] addmex
 - [ ] addzex
 - [x] divwx
 - [x] divwux
 - [ ] mulhwx
 - [ ] mulhwux
 - [x] mulli
 - [x] mullwx
 - [x] negx
 - [x] subfx
 - [x] subfcx
 - [x] subfex
 - [x] subficx
 - [ ] subfmex
 - [ ] subfzex
 #### Integer Compare Instructions
 - [x] cmp
 - [x] cmpi
 - [x] cmpl
 - [x] cmpli
 #### Integer Logical Instructions
 - [x] andx
 - [x] andcx
 - [x] andi.
 - [x] andis.
 - [x] cntlzwx
 - [ ] eqvx
 - [ ] extsbx
 - [x] extshx
 - [ ] nandx
 - [x] norx
 - [x] orx
 - [ ] orcx
 - [x] ori
 - [x] oris
 - [x] xorx
 - [x] xori
 - [x] xoris
 #### Integer Rotate Instructions
 - [x] rlwimix
 - [x] rlwinmx
 - [ ] rlwnmx
 #### Integer Shift Instructions
 - [x] slwx
 - [x] srawx
 - [x] srawix
 - [x] srwx
 #### Floating-Point Arithmetic Instructions
 - [x] faddx
 - [x] faddsx
 - [x] fdivx
 - [x] fdivsx
 - [x] fmulx
 - [x] fmulsx
 - [ ] fresx
 - [ ] frsqrtex
 - [x] fsubx
 - [x] fsubsx
 - [ ] fselx
 #### Floating-Point Multiply-Add Instructions
 - [x] fmaddx
 - [x] fmaddsx
 - [ ] fmsubx
 - [ ] fmsubsx
 - [ ] fnmaddx
 - [ ] fnmaddsx
 - [ ] fnmsubx
 - [ ] fnmsubsx
 #### Floating-Point Rounding and Conversion Instructions
 - [ ] fctiwx
 - [ ] fctiwzx
 - [ ] frspx
 #### Floating-Point Compare Instructions
 - [ ] fcmpo
 - [x] fcmpu
 #### Floating-Point Status and Control Register Instructions
 - [ ] mcrfs
 - [ ] mffsx
 - [ ] mtfsb0x
 - [ ] mtfsb1x
 - [x] mtfsfx
 - [ ] mtfsfix
 #### Integer Load Instructions
 - [x] lbz
 - [x] lbzu
 - [ ] lbzux
 - [x] lbzx
 - [x] lha
 - [ ] lhau
 - [ ] lhaux
 - [ ] lhax
 - [x] lhz
 - [ ] lhzu
 - [ ] lhzux
 - [ ] lhzx
 - [x] lwz
 - [x] lwzu
 - [ ] lwzux
 - [ ] lwzx
 #### Integer Store Instructions
 - [x] stb
 - [x] stbu
 - [ ] stbux
 - [ ] stbx
 - [x] sth
 - [ ] sthu
 - [ ] sthux
 - [ ] sthx
 - [x] stw
 - [x] stwu
 - [ ] stwux
 - [ ] stwx
 #### Integer Load and Store with Byte Reverse Instructions
 - [ ] lhbrx
 - [ ] lwbrx
 - [ ] sthbrx
 - [ ] stwbrx
 #### Integer Load and Store Multiple Instructions
 - [x] lmw
 - [x] stmw
 #### Integer Load and Store String Instructions
 - [ ] lswi
 - [ ] lswx
 - [ ] stswi
 - [ ] stswx
 #### Memory Synchronization Instructions
 - [ ] eieio
 - [x] isync
 - [ ] lwarx
 - [ ] stwcx.
 - [x] sync
 #### Floating-Point Load Instructions
 - [x] lfd
 - [ ] lfdu
 - [ ] lfdux
 - [ ] lfdx
 - [x] lfs
 - [ ] lfsu
 - [ ] lfsux
 - [ ] lfsx
 #### Floating-Point Store Instructions
 - [x] stfd
 - [ ] stfdu
 - [ ] stfdux
 - [ ] stfdx
 - [x] stfiwx
 - [x] stfs
 - [ ] stfsu
 - [ ] stfsux
 - [ ] stfsx
 #### Floating-Point Move Instructions
 - [ ] fabsx
 - [x] fmrx
 - [x] fnabsx
 - [x] fnegx
 #### Branch Instructions
 - [x] bx
 - [x] bcx
 - [x] bcctrx
 - [x] bclrx
 #### Condition Register Logical Instructions
 - [ ] crand
 - [ ] crandc
 - [x] creqv
 - [ ] crnand
 - [ ] crnor
 - [x] cror
 - [ ] crorc
 - [x] crxor
 - [x] mcrf
 #### System Linkage Instructions
 - [x] rfi
 - [x] sc
 #### Trap Instructions
 - [ ] tw
 - [ ] twi
 #### Processor Control Instructions
 - [ ] mcrxr
 - [x] mfcr
 - [x] mfmsr
 - [x] mfspr
 - [x] mftb
 - [x] mtcrf
 - [x] mtmsr
 - [x] mtspr
 #### Cache Management Instructions
 - [x] dcbf
 - [x] dcbi
 - [ ] dcbst
 - [ ] dcbt
 - [ ] dcbtst
 - [ ] dcbz
 - [x] icbi
 #### Segment Register Manipulation Instructions.
 - [x] mfsr
 - [ ] mfsrin
 - [ ] mtsr
 - [ ] mtsrin
 #### Lookaside Buffer Management Instructions
 - [ ] tlbie
 - [ ] tlbsync1
 #### External Control Instructions
 - [ ] eciwx
 - [ ] ecowx
 #### Paired-Single Load and Store Instructions
 - [ ] psq_lx
 - [ ] psq_stx
 - [ ] psq_lux
 - [ ] psq_stux
 - [x] psq_l
 - [ ] psq_lu
 - [x] psq_st
 - [ ] psq_stu
 #### Paired-Single Floating Point Arithmetic Instructions
 - [ ] ps_div
 - [x] ps_sub
 - [x] ps_add
 - [ ] ps_sel
 - [ ] ps_res
 - [x] ps_mul
 - [ ] ps_rsqrte
 - [ ] ps_msub
 - [ ] ps_madd
 - [x] ps_nmsub
 - [ ] ps_nmadd
 - [ ] ps_neg
 - [x] ps_mr
 - [ ] ps_nabs
 - [ ] ps_abs
 #### Miscellaneous Paired-Single Instructions
 - [x] ps_sum0
 - [x] ps_sum1
 - [x] ps_muls0
 - [x] ps_muls1
 - [x] ps_madds0
 - [x] ps_madds1
 - [ ] ps_cmpu0
 - [ ] ps_cmpo0
 - [ ] ps_cmpu1
 - [ ] ps_cmpo1
 - [x] ps_merge00
 - [x] ps_merge01
 - [x] ps_merge10
 - [x] ps_merge11
 - [ ] dcbz_l