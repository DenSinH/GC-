#include "SerialInterface.h"

#include "core_utils.h"
#include "log.h"

// todo: unnecessary (for debugging)
#include "../system.h"

#include <stdio.h>

HW_REG_READ_PRECALL(read_SICOMCSR, SI) {
    log_si("Read SI COMCSR");
    WRITE32(SI->regs, SI_reg_SICOMCSR, SI->COMCSR);
}

void SI_run_buffer(s_SI* SI, u32 chan) {
    e_SI_commands command = SI->regs[SI_buffer_base];

    switch (command) {
        case SI_command_reset:
        case SI_command_ID:
            if (chan == 0) {
                WRITE32(SI->regs, SI_buffer_base, SI_CONTROLLER);  // standard controller
                SI->SISR |= (SISR_RDST0 >> (chan << 3));  // set read status bit for corresponding channel
            }
            else {
                WRITE32(SI->regs, SI_buffer_base, SI_NO_CONTROLLER);
                SI->SISR |= (SISR_NOREP0 >> (chan << 3));  // no response error
            }
            break;
        case SI_command_direct:
            log_si("Get direct controller data for channel %d", chan);
            break;
        case SI_command_origin:
        case SI_command_recalibrate:
            log_si("recalibrate/origin channel %d", chan);
            if (chan == 0) {
                // only enabled controller
                memcpy(&SI->regs[SI_buffer_base], &controller_origin, sizeof(controller_origin));
            }
            break;
        default:
            log_warn("unknown SI command: %02x", command);
            break;
    }
}

HW_REG_WRITE_CALLBACK(write_SICOMCSR, SI) {
    u32 value = READ32(SI->regs, SI_reg_SICOMCSR);
    WRITE32(SI->regs, SI_reg_SICOMCSR, 0);  // clear for next write
    log_si("Write SI COMCSR (%08x)", value);
    SI->COMCSR = (value & ~(value & (SICOMCSR_TCINT | SICOMCSR_RDSTINT)));  // clear interrupt if enabled

    if (SI->COMCSR & SICOMCSR_TSTART) {
        // start transfer, complete after a few cycles
        SI->COMCSR |= SICOMCSR_TCINT;
        SI->COMCSR &= ~(SICOMCSR_TSTART | SICOMCSR_COMERR);  // transfer completes immediately and without errors

        if (SI->COMCSR & SICOMCSR_TCINTMSK) {
            // transfer complete interrupt enabled
            set_PI_intsr(SI->PI, PI_intr_SI, SI_TCINT_DELAY);
        }

        SI->COMCSR |= SICOMCSR_RDSTINT;  // instantly mark transfer as complete
        if (SI->COMCSR & SICOMCSR_RDSTINTMSK) {
            // read status interrupt enabled
            set_PI_intsr(SI->PI, PI_intr_SI, SI_RDSTINT_DELAY);
        }

        // the program expects a response from the SISR
        u32 channel = (SI->COMCSR & 0x6) >> 1;
        log_si("'Starting' data transfer for channel %d", channel);

        SI_run_buffer(SI, channel);
    }
    else if (value & SICOMCSR_TCINT) {
        clear_PI_intsr(SI->PI, PI_intr_SI);
    }
}

HW_REG_READ_PRECALL(read_SISR, SI) {
    // keep buffers always buffered
    SI->SISR &= ~(SISR_WRST0 | (SISR_WRST0 >> 8) | (SISR_WRST0 >> 16) | (SISR_WRST0 >> 24));

    log_si("Read SI SISR (%08x) (@%08x)", SI->SISR, SI->system->cpu.PC);
    WRITE32(SI->regs, SI_reg_SISR, SI->SISR);
}

HW_REG_WRITE_CALLBACK(write_SISR, SI) {
    u32 value = READ32(SI->regs, SI_reg_SISR);
    SI->SISR = (value & 0xf0f0f0f0) | (SI->SISR & ~(value & 0x0f0f0f0f)); // clear errors

    log_si("Write SI SISR (%08x) (@%08x)", SI->SISR, SI->system->cpu.PC);

    if (SI->SISR & SISR_Write_Buffer) {
        log_si("Copying SI buffers");
        // instantly copy buffers and clear this bit
        for (int i = 0; i < 4; i++) {
            if (SI->regs[SI_reg_SIC0OUTBUF + (i * SI_CHAN_STRIDE) + 1] == 0x40) {
                // write command (no others are mentioned in YAGCD or Dolphin)
                SI->modes[i] = SI->regs[SI_reg_SIC0OUTBUF + (i * SI_CHAN_STRIDE) + 2];  // first parameter
            }
        }

        SI->SISR &= ~SISR_Write_Buffer;
    }
}

void read_SI_Joy1x(s_SI* SI, int index) {
    log_si("Polled gamepad %d buttons 1", index);
    WRITE32(SI->regs, SI_reg_Joy1_base + (index * SI_CHAN_STRIDE), SI->joypad_buttons_1[index]);
}

HW_REG_READ_PRECALL(read_SI_Joy11, SI) { read_SI_Joy1x(SI, 0); }
HW_REG_READ_PRECALL(read_SI_Joy12, SI) { read_SI_Joy1x(SI, 1); }
HW_REG_READ_PRECALL(read_SI_Joy13, SI) { read_SI_Joy1x(SI, 2); }
HW_REG_READ_PRECALL(read_SI_Joy14, SI) { read_SI_Joy1x(SI, 3); }

void read_SI_Joy2x(s_SI* SI, int index) {
    log_si("Polled gamepad %d buttons 2", index);
    WRITE32(SI->regs, SI_reg_Joy2_base + (index * SI_CHAN_STRIDE), SI->joypad_buttons_2[index]);
}

HW_REG_READ_PRECALL(read_SI_Joy21, SI) { read_SI_Joy2x(SI, 0); }
HW_REG_READ_PRECALL(read_SI_Joy22, SI) { read_SI_Joy2x(SI, 1); }
HW_REG_READ_PRECALL(read_SI_Joy23, SI) { read_SI_Joy2x(SI, 2); }
HW_REG_READ_PRECALL(read_SI_Joy24, SI) { read_SI_Joy2x(SI, 3); }


HW_REG_WRITE_CALLBACK(write_SI_buffer, SI) {
    log_si("SI buffer data: %08x", READ32(SI->regs, 0x80));
}

// todo: for debugging, remove
HW_REG_READ_PRECALL(report_read, SI) {
    log_si("Poll buffer at %08x (%08x)", SI->system->cpu.PC, READ32(SI->regs, SI_buffer_base));
}


HW_REG_INIT_FUNCTION(SI) {
    SI->write[SI_buffer_base >> SI_SHIFT] = write_SI_buffer;
    SI->read[SI_buffer_base >> SI_SHIFT] = report_read;

    SI->read[SI_reg_SICOMCSR >> SI_SHIFT] = read_SICOMCSR;
    SI->write[SI_reg_SICOMCSR >> SI_SHIFT] = write_SICOMCSR;
    SI->read[SI_reg_SISR >> SI_SHIFT] = read_SISR;
    SI->write[SI_reg_SISR >> SI_SHIFT] = write_SISR;

    SI->read[(SI_reg_Joy1_base) >> SI_SHIFT] = read_SI_Joy11;
    SI->read[(SI_reg_Joy2_base) >> SI_SHIFT] = read_SI_Joy21;
    SI->read[(SI_reg_Joy1_base + 0xc) >> SI_SHIFT] = read_SI_Joy12;
    SI->read[(SI_reg_Joy2_base + 0xc) >> SI_SHIFT] = read_SI_Joy22;
    SI->read[(SI_reg_Joy1_base + 0x18) >> SI_SHIFT] = read_SI_Joy13;
    SI->read[(SI_reg_Joy2_base + 0x18) >> SI_SHIFT] = read_SI_Joy23;
    SI->read[(SI_reg_Joy1_base + 0x24) >> SI_SHIFT] = read_SI_Joy14;
    SI->read[(SI_reg_Joy2_base + 0x24) >> SI_SHIFT] = read_SI_Joy24;
}