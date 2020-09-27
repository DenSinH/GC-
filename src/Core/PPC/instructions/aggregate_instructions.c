#include "instructions.h"

GEKKO_INSTR(instr_000100) {
    // note: for this aggregate instruction, some have 5 bits to signify the instruction, others 10
    // I want to handle the 10bit case in the default case for the statement
    switch (instruction.raw & 0x03e) {
        // bottom 6 - 1 bits
        case PS_MUL_OPCODE_EXTENDED_5bit:
            ps_mul(cpu, instruction);
            return;
        case PS_MSUB_OPCODE_EXTENDED_5bit:
            ps_msub(cpu, instruction);
            return;
        case PS_MADD_OPCODE_EXTENDED_5bit:
            ps_madd(cpu, instruction);
            return;
        case PS_MADDS0_OPCODE_EXTENDED_5bit:
            ps_madds0(cpu, instruction);
            return;
        case PS_MADDS1_OPCODE_EXTENDED_5bit:
            ps_madds1(cpu, instruction);
            return;
        case PS_SUM0_OPCODE_EXTENDED_5bit:
            ps_sum0(cpu, instruction);
            return;
        case PS_SUM1_OPCODE_EXTENDED_5bit:
            ps_sum1(cpu, instruction);
            return;
        case PS_MULS0_OPCODE_EXTENDED_5bit:
            ps_muls0(cpu, instruction);
            return;
        case PS_MULS1_OPCODE_EXTENDED_5bit:
            ps_muls1(cpu, instruction);
            return;
        default:
            switch (instruction.raw & 0x7fe) {
                // bottom 11 bits
                case PS_NEG_OPCODE_EXTENDED_10bit:
                    ps_neg(cpu, instruction);
                    return;
                case PS_MR_OPCODE_EXTENDED_10bit:
                    ps_mr(cpu, instruction);
                    return;
                case PS_MERGE00_OPCODE_EXTENDED_10bit:
                    ps_merge00(cpu, instruction);
                    return;
                case PS_MERGE01_OPCODE_EXTENDED_10bit:
                    ps_merge01(cpu, instruction);
                    return;
                case PS_MERGE10_OPCODE_EXTENDED_10bit:
                    ps_merge10(cpu, instruction);
                    return;
                case PS_MERGE11_OPCODE_EXTENDED_10bit:
                    ps_merge11(cpu, instruction);
                    return;
                default:
                    unimplemented(cpu, instruction);
            }
    }
}

GEKKO_INSTR(instr_010011) {
    switch (instruction.raw & 0x7fe) {
        // bottom 11 - 1 bits
        case BCLR_X_OPCODE_EXTENDED:
            bclr_x(cpu, instruction);
            return;
        case BCCTR_X_OPCODE_EXTENDED:
            bcctr_x(cpu, instruction);
            return;
        case MCRF_OPCODE_EXTENDED:
            mcrf(cpu, instruction);
            return;
        case CREQV_OPCODE_EXTENDED:
            creqv(cpu, instruction);
            return;
        case CROR_OPCODE_EXTENDED:
            cror(cpu, instruction);
            return;
        case CRXOR_OPCODE_EXTENDED:
            crxor(cpu, instruction);
            return;
        case ISYNC_OPCODE_EXTENDED:
            isync(cpu, instruction);
            return;
        case RFI_OPCODE_EXTENDED:
            rfi(cpu, instruction);
            return;
        default:
            unimplemented(cpu, instruction);

    }
}

GEKKO_INSTR(instr_011111) {
    /* 0x7c000000 */
    switch (instruction.raw & 0x7fe) {
        // bottom 11 - 1 bits
        case ADD_OPCODE_EXTENDED:
            add(cpu, instruction);
            return;
        case ADDC_OPCODE_EXTENDED:
            addc(cpu, instruction);
            return;
        case ADDE_OPCODE_EXTENDED:
            adde(cpu, instruction);
            return;
        case ADDME_OPCODE_EXTENDED:
            addme(cpu, instruction);
            return;
        case ADDZE_OPCODE_EXTENDED:
            addze(cpu, instruction);
            return;
        case SUBF_OPCODE_EXTENDED:
            subf(cpu, instruction);
            return;
        case SUBFE_OPCODE_EXTENDED:
            subfe(cpu, instruction);
            return;
        case SUBFC_OPCODE_EXTENDED:
            subfc(cpu, instruction);
            return;
        case CMP_OPCODE_EXTENDED:
            cmp(cpu, instruction);
            return;
        case CMPL_OPCODE_EXTENDED:
            cmpl(cpu, instruction);
            return;
        case DIVW_OPCODE_EXTENDED:
            divw(cpu, instruction);
            return;
        case DIVWU_OPCODE_EXTENDED:
            divwu(cpu, instruction);
            return;
        case MULLW_OPCODE_EXTENDED:
            mullw(cpu, instruction);
            return;
        case ANDX_OPCODE_EXTENDED:
            andx(cpu, instruction);
            return;
        case ORX_OPCODE_EXTENDED:
            orx(cpu, instruction);
            return;
        case XORX_OPCODE_EXTENDED:
            xorx(cpu, instruction);
            return;
        case NOR_OPCODE_EXTENDED:
            nor(cpu, instruction);
            return;
        case ANDC_OPCODE_EXTENDED:
            andc(cpu, instruction);
            return;
        case NEG_OPCODE_EXTENDED:
            neg(cpu, instruction);
            return;
        case EXTSH_OPCODE_EXTENDED:
            extsh(cpu, instruction);
            return;
        case EXTSB_OPCODE_EXTENDED:
            extsb(cpu, instruction);
            return;
        case CNTLZW_OPCODE_EXTENDED:
            cntlzw(cpu, instruction);
            return;
        case SLW_OPCODE_EXTENDED:
            slw(cpu, instruction);
            return;
        case SRW_OPCODE_EXTENDED:
            srw(cpu, instruction);
            return;
        case SRAW_OPCODE_EXTENDED:
            sraw(cpu, instruction);
            return;
        case SRAWI_OPCODE_EXTENDED:
            srawi(cpu, instruction);
            return;
        case STWX_OPCODE_EXTENDED:
            stwx(cpu, instruction);
            return;
        case STHX_OPCODE_EXTENDED:
            sthx(cpu, instruction);
            return;
        case STBX_OPCODE_EXTENDED:
            stbx(cpu, instruction);
            return;
        case LWZX_OPCODE_EXTENDED:
            lwzx(cpu, instruction);
            return;
        case LHZX_OPCODE_EXTENDED:
            lhzx(cpu, instruction);
            return;
        case LBZX_OPCODE_EXTENDED:
            lbzx(cpu, instruction);
            return;
        case LFDX_OPCODE_EXTENDED:
            lfdx(cpu, instruction);
            return;
        case STFIWX_OPCODE_EXTENDED:
            stfiwx(cpu, instruction);
            return;
        case MFMSR_OPCODE_EXTENDED:
            mfmsr(cpu, instruction);
            return;
        case MTMSR_OPCODE_EXTENDED:
            mtmsr(cpu, instruction);
            return;
        case MFSPR_OPCODE_EXTENDED:
            mfspr(cpu, instruction);
            return;
        case MTSPR_OPCODE_EXTENDED:
            mtspr(cpu, instruction);
            return;
        case MFCR_OPCODE_EXTENDED:
            mfcr(cpu, instruction);
            return;
        case MTCRF_OPCODE_EXTENDED:
            mtcrf(cpu, instruction);
            return;
        case MFSR_OPCODE_EXTENDED:
            mfsr(cpu, instruction);
            return;
        case MTSR_OPCODE_EXTENDED:
            mtsr(cpu, instruction);
            return;
        case MFTB_OPCODE_EXTENDED:
            mftb(cpu, instruction);
            return;
        case SYNC_OPCODE_EXTENDED:
            sync(cpu, instruction);
            return;
        case DCBF_OPCODE_EXTENDED:
            dcbf(cpu, instruction);
            return;
        case DCBI_OPCODE_EXTENDED:
            dcbi(cpu, instruction);
            return;
        case ICBI_OPCODE_EXTENDED:
            icbi(cpu, instruction);
            return;
        default:
            unimplemented(cpu, instruction);
    }
}

GEKKO_INSTR(instr_111011) {
    switch (instruction.raw & 0x03e) {
        // bottom 6 - 1 bits
        case FSUBS_OPCODE_EXTENDED:
            fsubs(cpu, instruction);
            return;
        case FADDS_OPCODE_EXTENDED:
            fadds(cpu, instruction);
            return;
        case FMULS_OPCODE_EXTENDED:
            fmuls(cpu, instruction);
            return;
        case FDIVS_OPCODE_EXTENDED:
            fdivs(cpu, instruction);
            return;
        case FMSUBS_OPCODE_EXTENDED:
            fmsubs(cpu, instruction);
            return;
        case FMADDS_OPCODE_EXTENDED:
            fmadds(cpu, instruction);
            return;
        case FNMSUBS_OPCODE_EXTENDED:
            fnmsubs(cpu, instruction);
            return;
        case FNMADDS_OPCODE_EXTENDED:
            fnmadds(cpu, instruction);
            return;
        default:
            unimplemented(cpu, instruction);
    }
}

GEKKO_INSTR(instr_111111) {
    // note: for this aggregate instruction, some have 5 bits to signify the instruction, others 10
    // I want to handle the 10bit case in the default case for the statement
    switch (instruction.raw & 0x03e) {
        // bottom 6 - 1 bits
        case FSUB_OPCODE_EXTENDED_5bit:
            fsub(cpu, instruction);
            return;
        case FADD_OPCODE_EXTENDED_5bit:
            fadd(cpu, instruction);
            return;
        case FMUL_OPCODE_EXTENDED_5bit:
            fmul(cpu, instruction);
            return;
        case FDIV_OPCODE_EXTENDED_5bit:
            fdiv(cpu, instruction);
            return;
        case FMADD_OPCODE_EXTENDED_5bit:
            fmadd(cpu, instruction);
            return;
        case FRSQRTE_OPCODE_EXTENDED_5bit:
            frsqrte(cpu, instruction);
            return;
        default:
            switch (instruction.raw & 0x7fe) {
                // bottom 11 - 1 bits
                case FABSX_OPCODE_EXTENDED_10bit:
                    fabsx(cpu, instruction);
                    return;
                case FCMPU_OPCODE_EXTENDED_10bit:
                    fcmpu(cpu, instruction);
                    return;
                case FMR_OPCODE_EXTENDED_10bit:
                    fmr(cpu, instruction);
                    return;
                case FRSP_OPCODE_EXTENDED_10bit:
                    frsp(cpu, instruction);
                    return;
                case FCTIWZ_OPCODE_EXTENDED_10bit:
                    fctiwz(cpu, instruction);
                    return;
                case FNEG_OPCODE_EXTENDED_10bit:
                    fneg(cpu, instruction);
                    return;
                case MTFSF_OPCODE_EXTENDED_10bit:
                    mtfsf(cpu, instruction);
                    return;
                case MTFSB0_OPCODE_EXTENDED_10bit:
                    mtfsb0(cpu, instruction);
                    return;
                case MTFSB1_OPCODE_EXTENDED_10bit:
                    mtfsb1(cpu, instruction);
                    return;
                default:
                    unimplemented(cpu, instruction);
            }
    }
}