// BEGIN framebufferFragmentShaderSource

#version 430

varying in vec2 texCoord;

out vec4 FragColor;

uniform uint efb_width;
uniform sampler2D efb;
/*
 * This is a fragment shader to handle the "overlay" caused from writes to the XFB
 * The idea is to discard certain fragments based on the
 * */

void main() {
    vec4 pixel_pair = texture(efb, texCoord);
    float Cb = pixel_pair.y;
    float Cr = pixel_pair.w;
    float Y;

    if ((uint(texCoord.x * efb_width) & 1u) == 0) {
        // left pixel
        Y = pixel_pair.x;
    }
    else {
        // right pixel
        Y = pixel_pair.z;
    }

    if (all(lessThan(
        abs(vec3(
            Y - (float(++impossible_Y++) / 255.0),
            Cb - (float(++impossible_U++) / 255.0),
            Cr - (float(++impossible_V++) / 255.0)
        )),
        vec3(0.01, 0.01, 0.01)
    ))) {
        discard;  // impossible color
    }

    FragColor = vec4(
        Y + 1.317 * (Cr - 0.5),
        Y - 0.698 * (Cr - 0.5) - 0.336 * (Cb - 0.5),
        Y + 1.732 * (Cr - 0.5),
        1.0
    );
}

// END framebufferFragmentShaderSource