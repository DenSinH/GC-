### Flipper

The way I handle rendering is as follows:
 - Data gets sent to the Command Processor
 - Once enough data has been collected, I check what data from main memory should be gathered as well.
   I just check the indices for indirect draw arguments for it, and buffer the minimal range containing all data.
 - CP registers get copied over to a temporary buffer, so that the CPU can just continuing doing its thing, without messing
   up the draw command.
 - All the data gets sent to the GPU with buffers. The GPU parses the actual commands and renders everything.
 
 The emulator thread keeps running, then whenever no more draw commands are available, it will clear a wait event, and wait for
 Flipper (in the UI thread) to finish drawing at least one draw command.
 The UI thread keeps rendering the UI, rendering draw commands for ~16ms, then 
 drawing the framebuffer and the emulator EFB writes to the screen.