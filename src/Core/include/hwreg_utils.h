#ifndef GC__HWREG_UTILS_H
#define GC__HWREG_UTILS_H

#include "flags.h"

#include <assert.h>

#define HW_REG_READ_PRECALL(_name, _section) void _name(struct s_ ##_section * _section, u32 masked_address, int size)
#ifdef RECURSE_HR_ACCESS
#define HW_REG_READ_OVERFLOW_RECURSE(_section, _section_size) if ((size - (_section_size)) > 0 && _section->read[masked_address + (_section_size)]) _section->read[masked_address + (_section_size)](_section, masked_address + 2, size - (_section_size))
#else
#define HW_REG_READ_OVERFLOW_RECURSE(_section, _section_size)
#endif


#define HW_REG_WRITE_CALLBACK(_name, _section) void _name(struct s_ ##_section * _section, u32 masked_address, int size)

#ifdef RECURSE_HR_ACCESS
#define HW_REG_WRITE_OVERFLOW_RECURSE(_section, _section_size) if ((size - (_section_size)) > 0 && _section->write[masked_address + (_section_size)]) _section->write[masked_address + (_section_size)](_section, masked_address + 2, size - (_section_size))
#else
#define HW_REG_WRITE_OVERFLOW_RECURSE(_section, _section_size)
#endif

#define HW_REG_INIT_FUNCTION(_section) void init_ ## _section(struct s_ ##_section * _section)

#endif //GC__HWREG_UTILS_H
