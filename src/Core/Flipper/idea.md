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


##### phireToday at 10:15 AM
@DenSinH can you share the rest of you vertex shader?

##### DenSinHToday at 10:16 AM
sure!

the %stuff% gets replaced with python to enum values somewhere else

##### phireToday at 10:20 AM
ok, yeah. I wouldn't have one input per type

##### DenSinHToday at 10:20 AM
what would you suggest?
doing bitwise magic in glsl seems hard cause there are basically no bitwise functions from the looks of it

##### phireToday at 10:20 AM
you should be able to get away with just int32 types
maybe you need uint32 instead

##### DenSinHToday at 10:21 AM
but I have to have different locations right, cause the input types are somewhere else
*something
like different types

##### phireToday at 10:22 AM
but the input assembler stage should handle all that
notice how your pos_2d_uNN variables all share the uint type

##### DenSinHToday at 10:23 AM
yeah

##### phireToday at 10:23 AM
and your pos_2d_sNN variables all share the int type

##### DenSinHToday at 10:23 AM
yes

##### phireToday at 10:24 AM
so you can combine those

##### DenSinHToday at 10:24 AM
dont I have to tell openGL this

and then I need stuff to be in different locations right?

##### phireToday at 10:24 AM
you do have to tell opengl that
but that's all you need to tell opengl
you just have one of them set at any one time
all pointing at the same location

##### DenSinHToday at 10:25 AM
but then how does it know what to pick?

##### phireToday at 10:25 AM
by which one of the glVertexAttribPointers you have configured at the time of the draw call

##### DenSinHToday at 10:26 AM
oh so I shouldnt enable them all?
now that I say it

##### phireToday at 10:26 AM
no, just the one you are using
consider these as a diffrent type of uniform

##### DenSinHToday at 10:26 AM
but then they cant be at the same index right?

##### phireToday at 10:27 AM
they can all be at the same index, because only one will be enabled at anyone one time

##### DenSinHToday at 10:27 AM
but you call enable with glEnableVertexAttribArray right, where you pass the location of the proper one?

##### phireToday at 10:27 AM
and right, yes. the type in your shader should be uint
assuming you are doing byte swapping in the shader

##### DenSinHToday at 10:28 AM
so if they are all at location 0, and I say glEnableVertexAttribArray(0) it doesnt know what to pick
no I end up doing that before
cause there didnt seem to be much bitwise support in glsl for some reason

##### phireToday at 10:28 AM
sure there is

##### DenSinHToday at 10:28 AM
oh

##### phireToday at 10:28 AM
bitfieldExtract() is the main one you need

##### DenSinHToday at 10:29 AM
yeah but can I then do normal bitwise operations with that?
and can I call that on a coordinate of a vector?

##### phireToday at 10:30 AM
you should be using bitfieldExtract and bitfieldInsert if they do what you want
those funcitons map to single instructions on most gpus
and those two should do everything you want

##### DenSinHToday at 10:30 AM
and those work on vectors as well?

##### phireToday at 10:32 AM
I mean, vec3 pos = vec3(byteswap(raw_pos.x), byteswap(raw_pos.y), byteswap(raw_pos.z));

##### DenSinHToday at 10:32 AM
ah okay so like that it should work
yeah thats what I meant haha

##### phireToday at 10:33 AM
I don't think you can apply the same operation to the whole vector at once
but that compiles down to the same thing

##### DenSinHToday at 10:33 AM
okay, but I still dont really get why I could put all the attributes for different types (and different strides) on the same location
like
if Id put all the attributes for position on location 0 theres no way I can get the right stride and all

##### phireToday at 10:34 AM
leave it enabled, do a new call to glVertexAttribPointer whenever the vertex from changes
specifying the new types and strides

##### DenSinHToday at 10:34 AM
ahh okay
like that
and then just disable it if its not present

##### phireToday at 10:35 AM
yeah

##### DenSinHToday at 10:35 AM
so it wont seg fault like it does now if the color buffer is empty :cringeharold:
okay that makes sense

##### phireToday at 10:36 AM
I think (don't quote me on this) but disabling just causes your shader to see zeros
so you might not even have to make your shader branch

##### DenSinHToday at 10:36 AM
and then Id just have to glDisableVertexAttribArray every time?
that would make sense

##### phireToday at 10:37 AM
for safety, you should probally just spin though every attribute and either disable it, or enable it and set the type

##### DenSinHToday at 10:37 AM
I think Im going to have to re-do it again and add the vertex attributes that way
then I have to do less parsing on the CPU again
and I can just throw everything in the same buffer
still, I have to solve the issue with indexing

##### phireToday at 10:38 AM
I think that's optimal

##### DenSinHToday at 10:38 AM
so indirect stuff I still have to copy over

##### phireToday at 10:38 AM
does opengl not have a way to do that?

##### DenSinHToday at 10:38 AM
see thats whats messing with this strategy
yeah
but the GC can index everything differently
like index colors and positions differently
from the looks of it
and I looked it up, but I dont think I can bind multiple EBOs for different attributes

##### phireToday at 10:39 AM
well... I guess you could go the other route

##### DenSinHToday at 10:39 AM
yeah...
cause the problem is really that if stuff is indirect its much harder to do this way

##### phireToday at 10:40 AM
put all the other vertex arrays in uniform buffers
or one big large uniform buffer
then you can just do the indexing in your shader

##### DenSinHToday at 10:40 AM
you can buffer uniforms?

##### phireToday at 10:40 AM
sure
it essentially acts like a big memory array you can index into

##### DenSinHToday at 10:41 AM
Id still have to know how much I need to buffer then, but that should be okay

##### phireToday at 10:41 AM
just allocate a buffer of 200mb

##### DenSinHToday at 10:41 AM
that should be way more than enough lol

##### phireToday at 10:41 AM
modern GPUs have plenty of ram

##### DenSinHToday at 10:42 AM
can you set the size of vertex buffers as well?
or does it just determine that by itself
can I actually ask you a question for the emulation of these graphics in general
what would be a good way to handle it in general

##### phireToday at 10:43 AM
you want to use the glBufferStorage functionality

##### DenSinHToday at 10:43 AM
after yesterday I thought it might just be a lot easier to render to a framebuffer (texture) in the emulation thread and then render that on the screen in the ui thread
cause the way I handle it now is pretty dumb and not scalable I feel like

##### phireToday at 10:45 AM
basically the opengl bufferstorage extention (opengl 4.4 and later) lets you allocate a buffer of memory on the GPU
then map it into your programs address permently
so currently you are rendering both the ui and the emulation on the same thread?

##### DenSinHToday at 10:46 AM
no I render the imGui stuff on a different thread
and I tried to mix the flipper stuff into that
but I couldnt figure out a nice way to do it
so then I thought

##### phireToday at 10:46 AM
yeah, it gets a little awkard because the opengl context can only exist on a single thread

##### DenSinHToday at 10:47 AM
maybe I should do the emulation stuff to a different framebuffer
yeah exactly

##### phireToday at 10:47 AM
yeah, it's not supported

##### DenSinHToday at 10:47 AM
what I ended up doing is whenever flipper got a draw call (which was just 1 draw call), I just sort of let it sit and handled it in the ui thread
but thats impossible to do properly
basically
so I should probably have another openGL context in the emulation thread and render to a framebuffer there, then blit that onto the screen in the ui thread right?

##### phireToday at 10:50 AM
you can't really do this.
you have to get all the opengl commands to a single thread

##### DenSinHToday at 10:50 AM
ah dang

##### phireToday at 10:51 AM
btw, that's the other reason why you should use persistant buffer mapping
because any thread can write into a permently mapped buffer
and you can create a compact repenstation of the commands you need to issue on the opengl context thread

##### DenSinHToday at 10:52 AM
as opposed to VAOs and VBOs

##### phireToday at 10:52 AM
well, you still have VAOs and VBOs. they are just inside persistant mappings
your compressed command format will look pretty much like: [fence for data upload], [draw 200 triangles from buffers at 0x1230, 0x1240, 0x1250]

##### DenSinHToday at 10:55 AM
oh man
I really feel unqualified for this lol

##### phireToday at 10:55 AM
yeah, maybe

##### DenSinHToday at 10:55 AM
okay but by the buffers, would you mean buffers in the GC memory, or just the host memory?

##### phireToday at 10:56 AM
gpu memory

##### DenSinHToday at 10:56 AM
ah okay

##### phireToday at 10:56 AM
buffer storage allows you to map a range of host gpu memory into your application space

##### DenSinHToday at 10:56 AM
but looking at the glBufferStorage, couldnt I just buffer all of the GC memory somehow?

##### phireToday at 10:57 AM
so your emu thread (or a dedicated copy thread) can copy data straght out of GC memory into gpu memory
yes, but no
gpu memory has latency penalites, and often caching is disabled

##### DenSinHToday at 10:58 AM
ah so the data would be corrupt half the time

##### phireToday at 10:58 AM
corruption shouldn't be an issue
just really slow

##### DenSinHToday at 10:59 AM
oh
but then what would I do with it?
man

##### phireToday at 11:01 AM
mapping memory allows two things
1. Faster copies (or writes) straight to gpu memory
2. The ablitly to upload data to the gpu without being on the opengl context thread

##### DenSinHToday at 11:02 AM
ahh

##### phireToday at 11:02 AM
it's really fast compared to other ways to write to the GPU
but you don't want to treat it as regular memory and do read-write-modifiy operations on it

##### DenSinHToday at 11:03 AM
yeah Id just want to write to it from the emu thread
and read from it in the ui thread
right?

##### phireToday at 11:03 AM
no, just tell opengl to read it
it's already on the gpu, you can tell your draw calls to refrence it directly
you can tell your shaders to refrence it directly

##### DenSinHToday at 11:04 AM
yeah but from the emu thread I need to write to it right?

##### phireToday at 11:04 AM
yes

##### DenSinHToday at 11:05 AM
and I would create it as GL_UNIFORM_BUFFER then?
how would I be able to access that stuff from shaders then actually
actually, for the arrays you index into from the shader you want "Shader Storage Buffer Objects"

##### DenSinHToday at 11:07 AM
ah I see

##### phireToday at 11:07 AM
https://www.khronos.org/opengl/wiki/Shader_Storage_Buffer_Object
Shader Storage Buffer Object
sorry, I try to remember the common parts of the 3 graphics APIs

##### DenSinHToday at 11:08 AM
haha I mean youre really helping me out here
no need to be sorry

##### phireToday at 11:08 AM
:slight_smile:

##### DenSinHToday at 11:09 AM
okay yeah I see that I can access that then
so then I would just throw all my data to that
and queue up the commands somewhere else
then render those in the ui thread

##### phireToday at 11:09 AM
yeah, that's what I'd do

##### DenSinHToday at 11:09 AM
okay
and then
could I, within the UI thread, render to a separate framebuffer for the emulator?
cause the imGui stuff needs to be refreshed all the time

##### phireToday at 11:10 AM
yeah, you should

##### DenSinHToday at 11:10 AM
is there an easy way to handle that?

##### phireToday at 11:10 AM
haha, I wish

##### DenSinHToday at 11:10 AM
Ill probably have to bind something every time then right
:feelsbadman:
from what I could tell Id need to render to a texture

##### phireToday at 11:11 AM
oh that part is easy. Just bind the framebuffer you want to write to
and rendering to textures is an essential skill you should learn
and essential for getting certian graphical effects in Wind Waker
most gamecube games actually

##### DenSinHToday at 11:12 AM
you mean to use textures in general?

##### phireToday at 11:12 AM
render to texture effects

##### DenSinHToday at 11:13 AM
ah yeah

##### phireToday at 11:13 AM
in wind waker, all the shadows are done with render to texture
all the dynamic shadows

##### DenSinHToday at 11:13 AM
I was really hoping I could just get the triangle to render in a proper, scalable way before I go and study graphics a bit more

##### phireToday at 11:13 AM
heat haze effects are done with render to texture

##### DenSinHToday at 11:14 AM
cause getting the triangle didnt feel too exciting when I felt like I just did it in a pretty hacky way :/

##### phireToday at 11:14 AM
Wind Waker also has a depth-blur effect
that is done with render-to-texture

##### DenSinHToday at 11:14 AM
is rendering to textures hard to do then?
or at least, in the context of wanting to render to that and then blitting it to the screen

##### phireToday at 11:15 AM
nope
hence why I recomend learning now

##### DenSinHToday at 11:15 AM
before I go and try to make a separate frame buffer and stuff like that?

##### phireToday at 11:16 AM
render-to-texture is a seperate framebuffer
mostly

##### DenSinHToday at 11:16 AM
is it not just allocating a framebuffer/texture for flipper, rendering to that, then fetching it and blitting it to the screen?

##### phireToday at 11:17 AM
rather than "blitting" just set it as a texture and render it into the imgui framebuffer

##### DenSinHToday at 11:19 AM
http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-14-render-to-texture/ I guess this would be how to do it sort of
Tutorial 14 : Render To Texture
Free tutorials for modern Opengl (3.3 and later) in C/C++
okay so thinking about the glBufferStorage again, wouldnt it be nicer to have multiple, one for command data, and then Id need some for the indirect data that was sent right?
oh wait I see now that you can create layouts in the buffer as well

##### phireToday at 11:25 AM
you might need one for each type
I haven't tried this yet

##### DenSinHToday at 11:26 AM
like, each argument type you mean?
(thats what I was planning on doing with VBOs before)

##### phireToday at 11:27 AM
no, like one for uniforms, one for SSBOs, one for indices, one for framebuffers, one for textures
I'm not sure if you can make those co-exist in the same buffer
you can in vulkan

##### DenSinHToday at 11:27 AM
ahh
indices too?
wouldnt I handle that within the shader if I went this route?

##### phireToday at 11:29 AM
lets see.
with this route, which we are mostly taking because opengl doesn't support multiple indecies per vertex
I guess you would actually scrap the opengl concept of "index"
and just have a raw non-indexed draw call pointing at a vertex buffer, which is actually all your index data
then the shader will take the indices and pull in the actual vertex data from a command_SSBO

##### DenSinHToday at 11:31 AM
what Im thinking seeing this now is: have a struct in the CP that has a short for the vertex count, a buffer for the command data, so that I dont have to parse it at all in the emulator thread, then a separate buffer where I buffer all the arrays its been sent (probably quite a big one), and I just send that to the GPU on draw commands
I guess Id still sort of need to parse the data to see what array stuff I need to send
cause I cant just send all of the GCs memory every time :lul:

##### phireToday at 11:32 AM
yeah, I have a few protoypes for dolphin which move towards that design

##### DenSinHToday at 11:33 AM
the hardest part is that Id need to know how much data I actually need to send
I guess I could also add in some values that signify where in the "array part" of the data the different attributes start

##### phireToday at 11:33 AM
currently, dolphin has a "vertex loader jit" which unifies all the verices to a flat array of float data

##### DenSinHToday at 11:34 AM
so it actually parses everything before sending it to the GPU

##### phireToday at 11:34 AM
yeah, but it's a bit of a bottleneck

##### DenSinHToday at 11:35 AM
but how large would the arrays that hold indirect data need to be?
I guess id need to parse the indices then

##### phireToday at 11:35 AM
size of the largest index

##### DenSinHToday at 11:35 AM
yeah
and then Id still need to send the shader some info on what index to actually check
not sure if thats some default thing
I guess itd just need to be a buffer with the values 0, 1, ...

##### phireToday at 11:36 AM
just use the non-indexed version of draw

##### DenSinHToday at 11:37 AM
what?

##### phireToday at 11:37 AM
glDrawBuffers
no, wrong command
glDrawArrays

##### DenSinHToday at 11:38 AM
but that wont work right?
cause I wouldnt really be buffering anything in VBOs anymore
cause all the data would be in the command_SSBO

##### phireToday at 11:39 AM
the VBO would be the index data from CP

##### DenSinHToday at 11:40 AM
that would still involve parsing all the data though then

##### phireToday at 11:40 AM
just the index data
I suspect there isn't a way around that

##### DenSinHToday at 11:41 AM
well, doing it in the shader

##### phireToday at 11:41 AM
you need to know the highest index

##### DenSinHToday at 11:41 AM
true yeah
but in the actual draw command I would be parsing the data anyway
or like
in the shader I mean
wait
yeah I would want to just buffer the whole command, and the arrays into the command_SSBO, then send an array of increasing indices to the shader
so that in the shader I know where in the data I need to look

##### phireToday at 11:43 AM
I guess you can over-copy
max index is what, 64,000

##### DenSinHToday at 11:43 AM
yeah but that would be the case for all attributes
not sure if all of them are ever present at the same time
that would be at most
12 * 64000 * 21 or something
I also don't know how long the commands usually are tbh
like how long of a buffer Id need to allocate for that

##### phireToday at 11:45 AM
if you can have multiple commands collapse into eachother, their copies will mostly overlap
I'm not sure if this is a smart idea or not
a optimised loop that finds the min/max of each index while coping them to GPU memory probally won't take that much longer than a simple copy

##### DenSinHToday at 11:47 AM
it all just depends on the way the commands look
there might be some very extensive commands where it would be faster to just copy everything, but smaller commands you are probably better off finding the extreme indices

##### phireToday at 11:48 AM
you might have to generate one for each possible combinations of index sizes

##### DenSinHToday at 11:48 AM
there are quite many possibilities for that lol
how long do you reckon a buffer for the command data would need to be?

##### phireToday at 11:49 AM
only a few hundred, I think
command data is easy, just a few mb
on the CPU, not the GPU

##### DenSinHToday at 11:51 AM
okay doing it this way sounds doable now
would be nice if there was a default way to send the indices for the vertices in an increasing manner though

##### phireToday at 11:52 AM
and you might actually end up with a faster vertex pipeline than dolphin

##### DenSinHToday at 11:52 AM
:lul:
that would be nuts
first order of business would be rendering to a separate framebuffer though
for sure
cause the way it works now is the most hacky shit ever
oh wait then theres something else
Id still need to queue up the commands
and with a lot of data like this that might be slow as well
cause the struct holding the vertex data would be quite large, and I would have to like, copy that over or something
unless I maybe cycle through a few

##### phireToday at 11:55 AM
vertex data can be copied straight to the GPU

##### DenSinHToday at 11:55 AM
yeah but if I buffer data for 2 commands, before the first is done executing, it would fail right?

##### phireToday at 11:55 AM
only the sizes and pointers to those buffers and the daraw type should be in your command stream
look into fences

##### DenSinHToday at 11:56 AM
that would block the commands until the current one is done?

##### phireToday at 11:56 AM
all you need to do is make sure the next command you are about to render has all it's data written

##### DenSinHToday at 11:56 AM
you mean the buffer doesnt overflow?

##### phireToday at 11:56 AM
the data for each command shouldn't be overlapping, you should have plenty of room

##### DenSinHToday at 11:57 AM
hmm okay well, once I have something like this working for one triangle, I feel like its a lot more scalable
so then I can add that later I guess
or look into that later

##### phireToday at 11:59 AM
you can use https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glDrawElementsBaseVertex.xhtml to render starting from a non-zero index into a buffer

##### DenSinHToday at 12:00 PM
so I guess I would just do this with a constant array [0, 1, ...]

##### phireToday at 12:01 PM
yes

