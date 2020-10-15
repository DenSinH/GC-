#include "DSP_extended_instructions.h"

#include "DSP_memory.h"

#include "log.h"

#include <stdbool.h>

#ifdef CHECK_DSP_SAMEPAGE_LD
#include <assert.h>
#endif

/*
 * Info on what the extended opcodes do was obtained from Dolphin source code, specifically:
 * https://github.com/dolphin-emu/dolphin/blob/master/Source/Core/Core/DSP/Interpreter/DSPIntExtOps.cpp
 *
 * 0000 01rr : DR
 * 0000 10rr : IR
 * 0000 11rr : NR
 * 0001 ddrr : MV
 *
 * 001s s0dd : S
 * 001s s1dd : SN
 * 01dd d0ss : L
 * 01dd d1ss : LN
 *
 * 10dd 000s : LS
 * 10dd 010s : LSN
 * 10dd 100s : LSM
 * 10dd 110s : LSNM
 *
 * 10dd 001s : SL
 * 10dd 011s : SLN
 * 10dd 101s : SLM
 * 10dd 111s : SLNM
 *
 * 11dr 00ss : LD
 * 11dr 01ss : LDN
 * 11dr 10ss : LDM
 * 11dr 11ss : LDNM
 *
 * 11sr 0011 : LDAX
 * 11sr 0111 : LDAXN
 * 11sr 1011 : LDAXM
 * 11sr 1111 : LDAXNM
 * */

// xxxx xxxx 0000 01rr
DSP_EXT_INSTR(DSP_EXT_DR) {
    // Decrement AR $arR
    log_dsp_verbose("DR %x (%x)", instruction & 3, instruction);
    DSP->ar[instruction & 0x03]--;
}

// xxxx xxxx 0000 10rr
DSP_EXT_INSTR(DSP_EXT_IR) {
    // Increment AR $arR
    log_dsp_verbose("IR %x (%x)", instruction & 3, instruction);
    DSP->ar[instruction & 0x03]++;
}

// xxxx xxxx 0000 11rr
DSP_EXT_INSTR(DSP_EXT_NR) {
    // Add $ixR to $arR
    u8 reg = instruction & 0x03;
    log_dsp_verbose("NR %x (%x)", reg, instruction);

    DSP->ar[reg] += DSP->ix[reg];
}

// xxxx xxxx 0001 ddss
DSP_EXT_INSTR(DSP_EXT_MV) {
    // Move value of $acS.S to the $axD.D.
    u8 ss = instruction & 0x03;
    u8 dd = (instruction & 0x0c) >> 2;
    log_dsp_verbose("MV acS.S(%x) -> axD.D(%x) (%x)", ss, dd, instruction);

    *DSP->r[DSP_reg_ac0l + dd] = *DSP->r[DSP_reg_ac0l + ss];
}

// xxxx xxxx 001s s0dd
DSP_EXT_INSTR(DSP_EXT_S) {
    // Store value of $acS.S in the memory pointed by register $arD.
    // Post increment register $arD.
    u8 dd = instruction & 0x03;
    u8 ss = (instruction & 0x18) >> 3;
    log_dsp_verbose("S acS.S(%x) -> [ax%x] (%x)", ss, dd, instruction);

    DSP_write_dmem(DSP, DSP->ar[dd]++, *DSP->r[DSP_reg_ac0l + ss]);
}

// xxxx xxxx 001s s1dd
DSP_EXT_INSTR(DSP_EXT_SN) {
    // Store value of register $acS.S in the memory pointed by register $arD.
    // Add indexing register $ixD to register $arD.
    u8 dd = instruction & 0x03;
    u8 ss = (instruction & 0x18) >> 3;
    log_dsp_verbose("SN acS.S(%x) -> [ax%x] (%x)", ss, dd, instruction);

    DSP_write_dmem(DSP, DSP->ar[dd], *DSP->r[DSP_reg_ac0l + ss]);
    DSP->ar[dd] += DSP->ix[dd];
}

// xxxx xxxx 01dd d0ss
DSP_EXT_INSTR(DSP_EXT_L) {
    // Load $axD.D/$acD.D with value from memory pointed by register $arS.
    // Post increment register $arS.
    u8 ss  = instruction & 0x03;
    u8 ddd = (instruction & 0x38) >> 3;
    log_dsp_verbose("L ax/cD.D(%x) <- [ar%x] (%x)", ddd, ss, instruction);

    u16 value = DSP_read_dmem(DSP, DSP->ar[ss]++);
    *DSP->r[DSP_reg_ax0l + ddd] = value;

    if (DSP->sr.SXM && (DSP_reg_ax0l + ddd) >= DSP_reg_ac0m) {
        // 40 bit sign extend
        DSP->ac[ddd & 1].h = ((i16)value) >> 16;
        DSP->ac[ddd & 1].l = 0;
    }
}

// xxxx xxxx 01dd d1ss  (mistake in Dolphin)
DSP_EXT_INSTR(DSP_EXT_LN) {
    // Load $axD.D/$acD.D with value from memory pointed by register $arS.
    // Add indexing register $ixS to register $arS.
    u8 ss  = instruction & 0x03;
    u8 ddd = (instruction & 0x38) >> 3;
    log_dsp_verbose("LN ax/cD.D(%x) <- [ar%x] (%x)", ddd, ss, instruction);

    u16 value = DSP_read_dmem(DSP, DSP->ar[ss]);
    *DSP->r[DSP_reg_ax0l + ddd] = value;
    DSP->ar[ss] += DSP->ix[ss];

    if (DSP->sr.SXM && (DSP_reg_ax0l + ddd) >= DSP_reg_ac0m) {
        // 40 bit sign extend
        DSP->ac[ddd & 1].h = ((i16)value) >> 16;
        DSP->ac[ddd & 1].l = 0;
    }
}

// xxxx xxxx 10dd 000s
DSP_EXT_INSTR(DSP_EXT_LS) {
    // Load register $axD.D with value from memory pointed by register
    // $ar0. Store value from register $acS.m to memory location pointed by
    // register $ar3. Increment both $ar0 and $ar3.
    u8 s  = instruction & 1;
    u8 dd = (instruction & 0x30) >> 4;
    log_dsp_verbose("LS\n    axD.D(%x) <- [ar0]\n    ac%x.m -> [ar3] (%x)", dd, s, instruction);

    *DSP->r[DSP_reg_ax0l + dd] = DSP_read_dmem(DSP, DSP->ar[0]++);
    DSP_write_dmem(DSP, DSP->ar[3]++, DSP->ac[s].m);
}

// xxxx xxxx 10dd 010s
DSP_EXT_INSTR(DSP_EXT_LSN) {
    // Load register $axD.D with value from memory pointed by register
    // $ar0. Store value from register $acS.m to memory location pointed by
    // register $ar3. Add corresponding indexing register $ix0 to addressing
    // register $ar0 and increment $ar3.
    u8 s  = instruction & 1;
    u8 dd = (instruction & 0x30) >> 4;
    log_dsp_verbose("LSN\n    axD.D(%x) <- [ar0]\n    ac%x.m -> [ar3] (%x)", dd, s, instruction);

    *DSP->r[DSP_reg_ax0l + dd] = DSP_read_dmem(DSP, DSP->ar[0]);
    DSP_write_dmem(DSP, DSP->ar[3]++, DSP->ac[s].m);

    DSP->ar[0] += DSP->ix[0];
}

// xxxx xxxx 10dd 100s
DSP_EXT_INSTR(DSP_EXT_LSM) {
    // Load register $axD.D with value from memory pointed by register
    // $ar0. Store value from register $acS.m to memory location pointed by
    // register $ar3. Add corresponding indexing register $ix3 to addressing
    // register $ar3 and increment $ar0.
    u8 s  = instruction & 1;
    u8 dd = (instruction & 0x30) >> 4;
    log_dsp_verbose("LSM\n    axD.D(%x) <- [ar0]\n    ac%x.m -> [ar3] (%x)", dd, s, instruction);

    *DSP->r[DSP_reg_ax0l + dd] = DSP_read_dmem(DSP, DSP->ar[0]++);
    DSP_write_dmem(DSP, DSP->ar[3], DSP->ac[s].m);

    DSP->ar[3] += DSP->ix[3];
}

// xxxx xxxx 10dd 110s
DSP_EXT_INSTR(DSP_EXT_LSNM) {
    // Load register $axD.D with value from memory pointed by register
    // $ar0. Store value from register $acS.m to memory location pointed by
    // register $ar3. Add corresponding indexing register $ix0 to addressing
    // register $ar0 and add corresponding indexing register $ix3 to addressing
    // register $ar3.
    u8 s  = instruction & 1;
    u8 dd = (instruction & 0x30) >> 4;
    log_dsp_verbose("LSNM\n    axD.D(%x) <- [ar0]\n    ac%x.m -> [ar3] (%x)", dd, s, instruction);

    *DSP->r[DSP_reg_ax0l + dd] = DSP_read_dmem(DSP, DSP->ar[0]);
    DSP_write_dmem(DSP, DSP->ar[3], DSP->ac[s].m);

    DSP->ar[0] += DSP->ix[0];
    DSP->ar[3] += DSP->ix[3];
}

// xxxx xxxx 10dd 001s
DSP_EXT_INSTR(DSP_EXT_SL) {
    // Store value from register $acS.m to memory location pointed by register
    // $ar0. Load register $axD.D with value from memory pointed by register
    // $ar3. Increment both $ar0 and $ar3.
    u8 s  = instruction & 1;
    u8 dd = (instruction & 0x30) >> 4;
    log_dsp_verbose("SL\n    ac%x.m -> [ar3]\n    axD.D(%x) <- [ar0] (%x)", dd, s, instruction);

    DSP_write_dmem(DSP, DSP->ar[3]++, DSP->ac[s].m);
    *DSP->r[DSP_reg_ax0l + dd] = DSP_read_dmem(DSP, DSP->ar[0]++);
}

// xxxx xxxx 10dd 011s
DSP_EXT_INSTR(DSP_EXT_SLN) {
    // Store value from register $acS.m to memory location pointed by register
    // $ar0. Load register $axD.D with value from memory pointed by register
    // $ar3. Add corresponding indexing register $ix0 to addressing register $ar0
    // and increment $ar3.
    u8 s  = instruction & 1;
    u8 dd = (instruction & 0x30) >> 4;
    log_dsp_verbose("SLN\n    ac%x.m -> [ar3]\n    axD.D(%x) <- [ar0] (%x)", dd, s, instruction);

    DSP_write_dmem(DSP, DSP->ar[3]++, DSP->ac[s].m);
    *DSP->r[DSP_reg_ax0l + dd] = DSP_read_dmem(DSP, DSP->ar[0]);

    DSP->ar[0] += DSP->ix[0];
}

// xxxx xxxx 10dd 101s
DSP_EXT_INSTR(DSP_EXT_SLM) {
    // Store value from register $acS.m to memory location pointed by register
    // $ar0. Load register $axD.D with value from memory pointed by register
    // $ar3. Add corresponding indexing register $ix3 to addressing register $ar3
    // and increment $ar0.
    u8 s  = instruction & 1;
    u8 dd = (instruction & 0x30) >> 4;
    log_dsp_verbose("SLM\n    ac%x.m -> [ar3]\n    axD.D(%x) <- [ar0] (%x)", dd, s, instruction);

    DSP_write_dmem(DSP, DSP->ar[3], DSP->ac[s].m);
    *DSP->r[DSP_reg_ax0l + dd] = DSP_read_dmem(DSP, DSP->ar[0]++);

    DSP->ar[3] += DSP->ix[3];
}

// xxxx xxxx 10dd 111s
DSP_EXT_INSTR(DSP_EXT_SLNM) {
    // Store value from register $acS.m to memory location pointed by register
    // $ar0. Load register $axD.D with value from memory pointed by register
    // $ar3. Add corresponding indexing register $ix0 to addressing register $ar0
    // and add corresponding indexing register $ix3 to addressing register $ar3.
    u8 s  = instruction & 1;
    u8 dd = (instruction & 0x30) >> 4;
    log_dsp_verbose("SLNM\n    ac%x.m -> [ar3]\n    axD.D(%x) <- [ar0] (%x)", dd, s, instruction);

    DSP_write_dmem(DSP, DSP->ar[3], DSP->ac[s].m);
    *DSP->r[DSP_reg_ax0l + dd] = DSP_read_dmem(DSP, DSP->ar[0]);

    DSP->ar[0] += DSP->ix[0];
    DSP->ar[3] += DSP->ix[3];
}

static inline bool same_page(u16 addr1, u16 addr2) {
    return (addr1 & 0xfc00) == (addr2 & 0xfc00);
}

static inline bool valid_page(u16 addr) {
    return (addr & 0x7000) == 0 || (addr & 0xf000) == 0xf000;
}

// xxxx xxxx 11dr 00ss
DSP_EXT_INSTR(DSP_EXT_LD) {
    // example for "nx'ld $AX0.L, $AX1.L, @$AR0"
    // Loads the word pointed by AR0 to AX0.H, then loads the word pointed by AR3 to AX0.L.
    // Increments AR0 and AR3.
    // If AR0 and AR3 point into the same memory page (upper 6 bits of addr are the same -> games are
    // not doing that!)
    // then the value pointed by AR0 is loaded to BOTH AX0.H and AX0.L.
    // If AR0 points into an invalid memory page (ie 0x2000), then AX0.H keeps its old value.
    // If AR3 points into an invalid memory page, then AX0.L gets the same value as AX0.H.
    u8 d  = (instruction & 0x20) >> 5;
    u8 r  = (instruction & 0x10) >> 4;
    u8 ss = instruction & 0x03;
    log_dsp_verbose("LD\n    ax0.%x <- [ar%x]\n    ax1.%x <- [ar3] (%x)", d, r, ss, instruction);

#ifdef CHECK_DSP_SAMEPAGE_LD
    assert(!same_page(DSP->ar[ss], DSP->ar[3]));
#endif

    if (valid_page(DSP->ar[ss])) *DSP->r[DSP_reg_ax0l + (d << 1)] = DSP_read_dmem(DSP, DSP->ar[ss]);
    if (valid_page(DSP->ar[3])) *DSP->r[DSP_reg_ax1l + (r << 1)] = DSP_read_dmem(DSP, DSP->ar[3]);

    DSP->ar[ss]++;
    DSP->ar[3]++;
}

// xxxx xxxx 11sr 0011
DSP_EXT_INSTR(DSP_EXT_LDAX) {
    // this one is so weird ???
    u8 s  = (instruction & 0x20) >> 5;
    u8 r  = (instruction & 0x10) >> 4;
    log_dsp_verbose("LDAX\n    ax0.%x <- [ar%x]\n    ax1.%x <- [ar3] (%x)", r, s, r, instruction);

#ifdef CHECK_DSP_SAMEPAGE_LD
    assert(!same_page(DSP->ar[s], DSP->ar[3]));
#endif

    if (valid_page(DSP->ar[s])) *DSP->r[DSP_reg_ax0l + r] = DSP_read_dmem(DSP, DSP->ar[s]);
    if (valid_page(DSP->ar[3])) *DSP->r[DSP_reg_ax0l + r] = DSP_read_dmem(DSP, DSP->ar[3]);

    DSP->ar[s]++;
    DSP->ar[3]++;
}

// xxxx xxxx 11dr 01ss
DSP_EXT_INSTR(DSP_EXT_LDN) {
    u8 d  = (instruction & 0x20) >> 5;
    u8 r  = (instruction & 0x10) >> 4;
    u8 ss = instruction & 0x03;
    log_dsp_verbose("LDN\n    ax0.%x <- [ar%x]\n    ax1.%x <- [ar3] (%x)", d, r, ss, instruction);

#ifdef CHECK_DSP_SAMEPAGE_LD
    assert(!same_page(DSP->ar[ss], DSP->ar[3]));
#endif

    if (valid_page(DSP->ar[ss])) *DSP->r[DSP_reg_ax0l + (d << 1)] = DSP_read_dmem(DSP, DSP->ar[ss]);
    if (valid_page(DSP->ar[3])) *DSP->r[DSP_reg_ax1l + (r << 1)] = DSP_read_dmem(DSP, DSP->ar[3]);

    DSP->ar[ss] += DSP->ix[ss];
    DSP->ar[3]++;
}

// xxxx xxxx 11sr 0111
DSP_EXT_INSTR(DSP_EXT_LDAXN) {
    // this one is so weird ???
    u8 s  = (instruction & 0x20) >> 5;
    u8 r  = (instruction & 0x10) >> 4;
    log_dsp_verbose("LDAXN\n    ax0.%x <- [ar%x]\n    ax1.%x <- [ar3] (%x)", r, s, r, instruction);

#ifdef CHECK_DSP_SAMEPAGE_LD
    assert(!same_page(DSP->ar[s], DSP->ar[3]));
#endif

    if (valid_page(DSP->ar[s])) *DSP->r[DSP_reg_ax0l + r] = DSP_read_dmem(DSP, DSP->ar[s]);
    if (valid_page(DSP->ar[3])) *DSP->r[DSP_reg_ax0l + r] = DSP_read_dmem(DSP, DSP->ar[3]);

    DSP->ar[s] += DSP->ix[s];
    DSP->ar[3]++;
}

// xxxx xxxx 11dr 10ss
DSP_EXT_INSTR(DSP_EXT_LDM) {
    u8 d  = (instruction & 0x20) >> 5;
    u8 r  = (instruction & 0x10) >> 4;
    u8 ss = instruction & 0x03;
    log_dsp_verbose("LDM\n    ax0.%x <- [ar%x]\n    ax1.%x <- [ar3] (%x)", d, r, ss, instruction);

#ifdef CHECK_DSP_SAMEPAGE_LD
    assert(!same_page(DSP->ar[ss], DSP->ar[3]));
#endif

    if (valid_page(DSP->ar[ss])) *DSP->r[DSP_reg_ax0l + (d << 1)] = DSP_read_dmem(DSP, DSP->ar[ss]);
    if (valid_page(DSP->ar[3])) *DSP->r[DSP_reg_ax1l + (r << 1)] = DSP_read_dmem(DSP, DSP->ar[3]);

    DSP->ar[ss]++;
    DSP->ar[3] += DSP->ix[3];
}

// xxxx xxxx 11sr 1011
DSP_EXT_INSTR(DSP_EXT_LDAXM) {
    // this one is so weird ???
    u8 s  = (instruction & 0x20) >> 5;
    u8 r  = (instruction & 0x10) >> 4;
    log_dsp_verbose("LDAXM\n    ax0.%x <- [ar%x]\n    ax1.%x <- [ar3] (%x)", r, s, r, instruction);

#ifdef CHECK_DSP_SAMEPAGE_LD
    assert(!same_page(DSP->ar[s], DSP->ar[3]));
#endif

    if (valid_page(DSP->ar[s])) *DSP->r[DSP_reg_ax0l + r] = DSP_read_dmem(DSP, DSP->ar[s]);
    if (valid_page(DSP->ar[3])) *DSP->r[DSP_reg_ax0l + r] = DSP_read_dmem(DSP, DSP->ar[3]);

    DSP->ar[s]++;
    DSP->ar[3] += DSP->ix[3];
}

// xxxx xxxx 11dr 10ss
DSP_EXT_INSTR(DSP_EXT_LDNM) {
    u8 d  = (instruction & 0x20) >> 5;
    u8 r  = (instruction & 0x10) >> 4;
    u8 ss = instruction & 0x03;
    log_dsp_verbose("LDNM\n    ax0.%x <- [ar%x]\n    ax1.%x <- [ar3] (%x)", d, r, ss, instruction);

#ifdef CHECK_DSP_SAMEPAGE_LD
    assert(!same_page(DSP->ar[ss], DSP->ar[3]));
#endif

    if (valid_page(DSP->ar[ss])) *DSP->r[DSP_reg_ax0l + (d << 1)] = DSP_read_dmem(DSP, DSP->ar[ss]);
    if (valid_page(DSP->ar[3])) *DSP->r[DSP_reg_ax1l + (r << 1)] = DSP_read_dmem(DSP, DSP->ar[3]);

    DSP->ar[ss] += DSP->ix[ss];
    DSP->ar[3]  += DSP->ix[3];
}

// xxxx xxxx 11dr 1111
DSP_EXT_INSTR(DSP_EXT_LDAXNM) {
    // this one is so weird ???
    u8 s  = (instruction & 0x20) >> 5;
    u8 r  = (instruction & 0x10) >> 4;
    log_dsp_verbose("LDAXNM\n    ax0.%x <- [ar%x]\n    ax1.%x <- [ar3] (%x)", r, s, r, instruction);

#ifdef CHECK_DSP_SAMEPAGE_LD
    assert(!same_page(DSP->ar[s], DSP->ar[3]));
#endif

    if (valid_page(DSP->ar[s])) *DSP->r[DSP_reg_ax0l + r] = DSP_read_dmem(DSP, DSP->ar[s]);
    if (valid_page(DSP->ar[3])) *DSP->r[DSP_reg_ax0l + r] = DSP_read_dmem(DSP, DSP->ar[3]);

    DSP->ar[s] += DSP->ix[s];
    DSP->ar[3] += DSP->ix[3];
}

// 0000 0000
DSP_EXT_INSTR(DSP_EXT_NOP) {

}

// ???? ????
DSP_EXT_INSTR(DSP_EXT_INVALID) {
    log_fatal("Invalid extended DSP instruction: %02x", instruction);
}