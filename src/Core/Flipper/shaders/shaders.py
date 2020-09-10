import os
import re
import sys

SCRIPT, *_ = sys.argv
SHADER_DIR = f"{os.path.dirname(SCRIPT)}/raw"
CONSTANTS = f"{os.path.dirname(SCRIPT)}/GX_constants.h"
SHADER_H = "#ifndef GC__SHADER_H\n" \
           "#define GC__SHADER_H"

with open(CONSTANTS, "r") as f:
    constants = {}
    for line in f.readlines():
        match = re.match(r"^.*?(\w+).*=.*?(\w+).*$", line)
        if not match:
            continue
        constants[match.group(1)] = match.group(2)

for file in os.listdir(SHADER_DIR):
    with open(os.path.join(SHADER_DIR, file), "r") as f:
        content = f.read().split("\n")

    i = 0
    while i < len(content):
        match = re.match(".*BEGIN\\s+(\w+)", content[i])
        shader_start = i
        i += 1
        if not match:
            continue

        shader_name = match.group(1)
        shader = [f"const char* {shader_name} = "]

        # skip leading empty lines
        while not content[i]:
            i += 1

        while i < len(content) and not re.match(f".*END\\s+{shader_name}", content[i]):
            # escape backslashes and quotes
            line = content[i].replace("\\", "\\\\").replace("\"", "\\\"")
            for const, value in constants.items():
                line = re.sub(f"%{const}%", value, line)

            shader.append(f'"{line}\\n"')
            i += 1

        if i == len(content):
            raise OSError(f"EOF reached while scanning shader '{shader_name}'")

        # remove trailing newlines (only whitespace and quotes in text)
        while re.match(r"^(\s|(\\n)|\")*$", shader[-1]):
            shader.pop(-1)

        text = "\n".join(shader)

        # skip line with END in it
        i += 1
        SHADER_H += f"\n\n// {shader_name} (from {file}, lines {shader_start} to {i - 1})\n{text};\n"

SHADER_H += "\n#endif  // GC__SHADER_H"

print("outputting shaders.h in ", f"{os.path.dirname(SCRIPT)}/shaders.h")

with open(f"{os.path.dirname(SCRIPT)}/shaders.h", "w+") as f:
    f.write(SHADER_H)
