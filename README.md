# .hack//fragment Item Transfer Tool
## Introduction
FCHucker, FTwinker, Free Trader Yarthkins.

This program lets you transfer items and GP between your .hack//fragment characters without having to go online, trade them to someone, and then have them traded back. 

## Building
Requires: 
* [ps2sdk](https://github.com/ps2dev/ps2sdk)
* [gsKit](https://github.com/ps2dev/gsKit). 

Follow install and build instructions for ps2sdk and gsKit, and this should compile with a simple:

    make

## Extras
Includes a helper python 3 script:
tex2incbin:

> reads an image file using Pillow, creates a plain binary file, then
> creates .c and .h files which utilize the INCBIN and INCBIN_EXTERN macros from [incbin.h](https://github.com/graphitemaster/incbin) to include the resulting binary files into the program without having to create a giant .c or .h file.
> 
> If you haven't used incbin, I highly recommend it. 

## Usage
Run the .elf file. There's help text at the bottom of the screen that should be help you out.

You can select any two characters from either memory card to perform a transfer on. 
Items will be transferred to On Hand Inventory(40 slots), first, either filling up the existing slot, if one exists, or using an empty one, if available. Anything left over is transferred to Storage(99 slots). Anything that is left after filling up both of those is given back to the source character.

Currently, it does not support transferring items between a single character's On Hand and Storage, nor does it allow specifying whether to transfer items to the another character's On Hand or Storage. This feature could be added, but in game there's already a feature for this, so prioritize you item transfers. 

**I HIGHLY recommend backing up your save files before using this.** 
Worst case scenario you'll have bugged up inventory and storage, but it's better to be safe than sorry.

## Limitations
**Key Items cannot be transferred.**
I will personally never introduce this feature because I don't really see the point, and I don't even want to know what, if any game breaking bugs there might be as a result.

**This will not let you bypass inventory limitations**
You cannot have more than 99 of an item in either on hand inventory or storage.

If you have 40 different types of items in your On Hand inventory, and you try to transfer an item you already have 99 of in your Storage, but none in On Hand, the transfer will fail.  The transfer result dialog will tell you that there were no free inventory slots. 

Likewise, if you have 99 of an item in your On Hand inventory, and your Storage has 99 different other types of items, but free slots in On Hand, the transfer will fail, and the transfer result dialog will tell you there were no free storage slots.

## Bugs
I'm sure there's some bugs with the Item lists displaying properly after transferring items. I've tried my best to catch them all, but there are still some glitches I haven't been able to reproduce consistently. Worst case scenario, save, return, initiate another transfer.

## Other Notes
The repo is named FCHucker, which stands for (F)ragment ( C)haracter (Hucker).
The name of the original program I wrote for decrypting save files was actually named Hucker, after an unused save file string inside the Fragment elf files (BISLPS-00000HUCKER)

There's also a Build 8(GU) themed UI texture (xwindo00.png) that you could use in place of UITex.png, but there's some issues with the alpha on some of the UI elements that I haven't sorted out yet. It has something to do with the alpha blending or testing. I dunno. If you want to mess with it, go for it.

 Save file crypto is now officially open source and in the wild. I personally couldn't care less at this point. In the past it was kept private out of a misguided attempt to prevent cheating, but the save files are pretty much dumped into memory straight from the file, so this doesn't really allow you to do anything that you couldn't already do with plain memory edits. 
well, I suppose it would be helpful for mapping out the save files, but even that's still easy to accomplish with memory viewing/editing.

## License
For licensing information see UNLICENSE.

