This Is (Not) A Readme (Yet)
============================

Maybe something will be made of this project. Maybe not. It's my hope
that it doesn't turn into that [other project](https://github.com/Iambian/VANISH) that both never got anywhere and was named specifically
so that it'd reflect its true nature of having gotten nowhere.
After all, acetone is a thing used to make messes go away...

Here's hoping it in itself doesn't do that.

Progress
--------
* Core tested (via nestest) to work correctly for all official instructions.
* Opcode disassembler with all known instructions. This has been hacked
  backwards and forwards to make it Do The Thing, ~~which is basically the only
  thing you're seeing at the moment~~
* ROM files up to 48KB can now be converted via .IPYNB test script.
* Converted ROM files loads using generic machinery. ~~Pls don't put any ROM other
  than nestest on the calc. It can't tell the difference;~~ it'll just load the
  first thing that looks like a ROM file. ~~Everything else has been rigged to
  debug specifically for nestest and they'll get super confused~~.
* Partially implemented Mapper 0. ~~I still haven't implemented a PPU, so... yeah.~~
* Simple PPU implemented. Is now capable of no-shenanigans BG and 
  sprite rendering. Some basic things still remain to be implemented.
* Interrupts kinda worked but now it's probably broken again since I
  just made changes where the stack now has to operate like one.


