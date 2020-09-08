### Idea for handling graphics:

In the command processor:
flip the bytes of the arguments for draw commands as they are coming in. To do this, make a `u8[22]` (max number of arguments)
to hold the strides for the arguments in order, last element always being the length of the array.

Then when arguments are coming in, place the byte in
`stride[i + 1] - (argc - stride[i]) = stride[i] + stride[i + 1] - argc`
We can also maybe account for this immediately when making the `stride` array. This is
so that the resulting `args` array is little endian!

In flipper: use either different buffers for the different attributes or one main buffer for everything (depending on direct/indirect).

When a draw argument was complete, we send the data to the general buffer, and if there are attributes that are indirect,
we should copy those into the separate buffer, and use the data as indices.

Then in the shader (I plan on doing only position and color in one shader for now),
we handle anything else.

Thought: buffer the arguments in different arrays as well, then load them
into different buffers in flipper (VBO or EBO depending on direct/indirect)
and bind the appropriate ones to render

different VBO/EBOs, one VAO, multiple glVertexAttribute calls, increment every time, indices are not per buffer

use a bool to signify whether flipper is drawing or not, return that on CP[0] reads


DenSinHToday at 10:23 AM

Oh wait I have an idea
I get the draw command arguments one byte at the time anyway, so I could just reverse the order I put them in the buffer
That way it's immediately LE
I just have to invert the strides as well then

phireToday at 11:29 AM

You can only reverse them once you know the type
So yeah, kind of need to be done in the shader, if you go that route

DenSinHToday at 11:47 AM

No I had an idea, when I count the length of the arguments on the start of a draw command, I can calculate the offsets for the arguments immediately, then when I get the arguments (byte by byte), I can calculate where to put them in the buffer I have for arguments in the CP to make them LE immediately
Then I can memcpy that into flipper and I have my arguments little endian immediately
Oh wait for indirect attributes that wont work :/

DenSinHToday at 12:10 PM


Though I googled around a bit, and it seems like it won't be that easy to swap those bytes around in the shader

phireToday at 12:44 PM


ints will be fine

DenSinHToday at 12:46 PM

Yeah it might be easier to just swap it when loading the buffer then tbh

phireToday at 12:46 PM

but I don't thing these is a way to reinterpet a int as a float in shaders

DenSinHToday at 12:47 PM

If I have to memcpy it anyway I'll probably just write a version that swaps bytes around

phireToday at 12:48 PM

oh wait... they literally have a function
https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/intBitsToFloat.xhtml

DenSinHToday at 12:49 PM

Ohh that's nice
Then it might be doable
I looked up if there was a built-in conversion from BE to LE, but that only seemed possible for pixel formats

phireToday at 12:50 PM

that one is easy
LE = BE & 0xFF << 24 | BE & 0xFF00 << 8 | BE & 0xFF0000 >> 8 | BE & 0xFF000000 >> 24;

DenSinHToday at 12:52 PM

Yeah haha I'll just have to do it that way

phireToday at 12:52 PM

wrap it in a macro, or a function

DenSinHToday at 12:53 PM

Can you specify the types of coordinates in glsl?
Like it not being float but uint instead
Or are vecn's always floats

phireToday at 12:53 PM

they are always floats
they are always floats in flipper too

DenSinHToday at 12:54 PM

So I can't just load the values as vectors then I guess

phireToday at 12:54 PM

you need to convert the ints to floats in your vertex shader

DenSinHToday at 12:54 PM

Oh wait there uvecn
Yeah but then I can just always load it as uvecn and then cast it to float and swap the bytes around

phireToday at 12:55 PM

well, swap then cast to float

DenSinHToday at 12:55 PM

Yeah exactly

phireToday at 12:55 PM

don't forget the scale

DenSinHToday at 12:55 PM

But it's more convenient to do that in vectors probably
At least loading it in

phireToday at 12:55 PM

eh, it will compile to the same shader
but if it makes the code look pretier

DenSinHToday at 12:56 PM

Not sure how that works with 8/16/32 bits though :/
Ah
Yeah Ill still have to learn the syntax and the possibilities a bit better
And how to handle the input
phireToday at 12:56 PM

you might need to emulate 16 and 8 bit types

DenSinHToday at 12:57 PM

I was hoping I could pass an extra variable and load the input as different types or something
Instead of writing a different shader for each one
phireToday at 12:57 PM

using bitfieldExtract, another glsl function

DenSinHToday at 12:59 PM

But if I load a 3 byte coordinate, I'd need to load it into a vector that takes a byte pet coordinate right
phireToday at 12:59 PM

also... while GLSL might not have u16 and u8 types, the input assembly state does

DenSinHToday at 12:59 PM

Otherwise it'd just go wrong
Yeah exactly
Oh that probably converts it for me
Or handles it or something

phireToday at 12:59 PM

yeah, it converts from 8 and 16 to 32bit

DenSinHToday at 1:00 PM

Ahh that's very nice
And I saw I could pass extra variables with uniform something right?
That way I can probably specify whether it's u8/16/32/f32 or something

phireToday at 1:01 PM

constant variables across the entire draw call

DenSinHToday at 1:01 PM

Yeah
To hold info on the format
It should be constant for the draw call anyway

phireToday at 1:01 PM

no, input assembly is a fixed function unit before the shader
so the shader can't controll it

DenSinHToday at 1:03 PM

Yeah but ill just always input as uints
And then depending on the format turn it into floats for the gl_Position or color or whatever
I'd just need to pass the info on the format

phireToday at 1:03 PM

you will have to set the number of elements and their widths with vertex attributes

DenSinHToday at 1:04 PM

Yeah but I can't see that from inside the shader right?

phireToday at 1:04 PM

then you can set if they need sign extention or if they should be cast to floats in your uniforms

DenSinHToday at 1:04 PM

Yeah exactly
That's what I was thinking
And parse the input in the shader then
Probably write some generic shaders for position and color separately

phireToday at 1:05 PM

yeah, guess you can do that with uniforms

DenSinHToday at 1:05 PM

And link those

phireToday at 1:10 PM

you can't really link shaders, it's more of a "include them all together"

DenSinHToday at 1:11 PM

Oh huh, but you could use output from one as input for another right?
Still, that would work haha
Something else I remembered, was that since the input is option I'll probably need to separate the input whenever I get it I to separate buffers for attributes maybe
Unless I can use uniforms to specify the location for the inputs as well
That would be perfect