def to_array(name: str, data: bytes) -> str:
    assert (len(data) & 0x1f) == 0, "data length should be a multiple of 32 bytes"

    content = ""
    for i in range(0, len(data), 32):
        content += "    " + "".join("0x%02x, " % b for b in data[i:i + 32]) + "\n"

    content = f"const static u8 {name}[] = {{\n{content}}};"
    return content


def to_header(name: str, data: bytes) -> None:
    with open(f"./{name}.h", "w+") as f:
        f.write(
f"""
#ifndef {name.upper() + '_H'}
#define {name.upper() + '_H'}

#include "default.h"

{to_array(name, data)}

#endif // {name.upper() + '_H'}
""")


with open("./dsp_coef.bin", "rb") as f:
    coef = f.read()

with open("./dsp_rom.bin", "rb") as f:
    rom = f.read()

to_header("dsp_coef", coef)
to_header("dsp_rom", rom)