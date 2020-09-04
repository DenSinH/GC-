#include "CommandProcessor.h"
#include "core_utils.h"
#include "log.h"
#include "flags.h"

#ifdef CHECK_CP_COMMAND
#include <assert.h>
#define ASSERT_ARG_SIZE
#else

#endif

u32 get_CP_reg(s_CP* CP, e_CP_regs reg_hi, e_CP_regs reg_lo) {
    return (READ16(CP->regs, reg_hi) << 16) | READ16(CP->regs, reg_lo);
}

void set_CP_reg(s_CP* CP, e_CP_regs reg_hi, e_CP_regs reg_lo, u32 value) {
    WRITE16(CP->regs, reg_hi, value >> 16);
    WRITE16(CP->regs, reg_lo, value & 0xffff);
}


HW_REG_INIT_FUNCTION(CP) {

}

inline void load_CP_reg(u8 RID, u32 value) {
    log_cp("Load CP register %02x with %08x", RID, value);
}

inline void load_XF_regs(u16 length, u16 base_addr, u8* values_buffer) {
    log_cp("Load %d XF registers starting at %04x", length, base_addr);
}

inline void load_INDX(e_CP_cmd opcode, u16 index, u8 length, u16 base_addr) {
    log_cp("Load %d XF indexed (%02x, index value %02x) starting at %04x", length, opcode, index, base_addr);
}

inline void call_DL(u32 list_addr, u32 list_size) {
    log_cp("Call DL at %08x of size %08x", list_addr, list_size);
}

inline void load_BP_reg(u32 value) {
    // value contains RID _and_ value here
    log_cp("Load BP reg: %08x", value);
}

inline void feed_CP(s_CP* CP, u8 data) {
    // feed from the buffer from some index
    // always read argument (we never get here with a NOP anyway)
    CP->args[CP->argc++] = data;
    switch (CP->command) {
        case CP_cmd_NOP:
            log_fatal("Uncaught NOP in argument feeding");
            break;
        case CP_cmd_load_CP_reg:
            if (CP->argc == 5) {
                // 8bit RID + 32 bit value
                load_CP_reg(CP->args[0], READ32(CP->args, 1));
                CP->fetching = false;
            }
            break;
        case CP_cmd_load_XF_reg:
            if (CP->argc >= 2 && CP->argc == 2 + 2 + ((1 + READ16(CP->args, 0)) << 2)) {
                // 16bit (length - 1) + 16bit (1st addr) + 32bit * length (values)
                // in args we correct for the -1, and so do we do this for the buffer (starts at index 4)
                load_XF_regs(READ16(CP->args, 0) + 1, READ16(CP->args, 2), &CP->args[4]);
                CP->fetching = false;
            }
            break;
        case CP_cmd_load_INDX_A:
        case CP_cmd_load_INDX_B:
        case CP_cmd_load_INDX_C:
        case CP_cmd_load_INDX_D:
            if (CP->argc == 4) {
                // 16bit (index value) + 4bit (length - 1) + 12bit (1st address)
                u16 length_and_base_address = READ16(CP->args, 2);
                load_INDX(CP->command, READ16(CP->args, 0), length_and_base_address >> 12, length_and_base_address & 0x0fff);
                CP->fetching = false;
            }
            break;
        case CP_cmd_call_DL:
            if (CP->argc == 8) {
                // 32bit (list addr) + 32bit (list size)
                call_DL(READ32(CP->args, 0), READ32(CP->args, 4));
                CP->fetching = false;
            }
            break;
        case CP_cmd_inval_vertex_cache:
            // no arguments
            CP->command = data;
            CP->argc = 0;
            // keep fetching
            break;
        case CP_cmd_load_BP_reg:
            if (CP->argc == 4) {
                // 8bit (reg addr) + 24bit (value)
                load_BP_reg(READ32(CP->args, 0));
                CP->fetching = false;
            }
            break;
        case CP_cmd_QUADS ... CP_cmd_MAX:
            // all drawing commands
            // todo: unstub
            if (CP->argc >= 2 && CP->argc == 2 + (READ16(CP->args, 0) << 1)) {
                // from the demo it seems like there are only 2 bytes of data per vertex
                log_cp("Draw primitive (%02x, stubbed)", CP->command & 0xf8);
                CP->fetching = false;
            }
            break;
        default:
            log_fatal("Invalid CP command: %08x", CP->command);
    }
}


void execute_buffer(s_CP* CP, const u8* buffer_ptr, u8 buffer_size) {
    u32 write_ptr = get_CP_reg(CP, CP_reg_FIFO_write_ptr_hi, CP_reg_FIFO_write_ptr_lo);
    // we read and parse the commands instantly, we don't need to get the read pointer
    u32 FIFO_base = get_CP_reg(CP, CP_reg_FIFO_base_hi, CP_reg_FIFO_base_lo);
    u32 FIFO_end = get_CP_reg(CP, CP_reg_FIFO_end_hi, CP_reg_FIFO_end_lo);

    u8 i = 0;
    while (i < buffer_size) {

        // log_cp("Should write/read to %08x (%08x -> %08x)", write_ptr++, FIFO_base, FIFO_end);
        if (!CP->fetching) {
            // reset CP command fetching environment
            CP->command = buffer_ptr[i++];
            CP->argc = 0;
            CP->fetching = CP->command != CP_cmd_NOP;
        }
        else {
            feed_CP(CP, buffer_ptr[i++]);
        }

        // FIFO_end is inclusive!
        if (write_ptr > FIFO_end) {
            write_ptr = FIFO_end;
        }
    }

    set_CP_reg(CP, CP_reg_FIFO_write_ptr_hi, CP_reg_FIFO_write_ptr_lo, write_ptr);
    set_CP_reg(CP, CP_reg_FIFO_read_ptr_hi, CP_reg_FIFO_read_ptr_lo, write_ptr);
}