BADJOKES is a collection of graphical joke programs that run on the Windows
operating system. The ICONDANCE and BLASTBTN programs are not my creations but have
been reverse engineered to some reasonable degree of accuracy. If you've never seen
either of them before I invite you to go and watch their awesomeness here:

https://www.youtube.com/watch?v=e5Iw3MTQMkw
https://www.youtube.com/watch?v=16XJBnjzMYU


   ICONDANCE
///////////////////////////////////////////////////////////////////////////////////

Before running it, uncheck "Auto arrange icons" and "Align icons to grid" under
"View" from the desktop context menu. Otherwise the dance will look a little stiff.

The last act will play on until the message box is closed.


   BLASTBTN
///////////////////////////////////////////////////////////////////////////////////

Not all buttons (windows) are eligible to explode.

Adjust the dimension checks to see larger windows explode. (default: 450x100)

The ESCAPE key on your keyboard will be registered globally as a hotkey. While the
program is running, push this key and it will clean up (itself) and exit.


   Compiling the above two programs
///////////////////////////////////////////////////////////////////////////////////

Microsoft's C/C++ Compiler or MinGW's GCC works. Compile as 32-bit (or 64-bit,
although I haven't tried).

For BLASTBTN, you'll want a compatible Resource Compiler. If you are using GCC you
can use "windres" like so:

i686-w64-mingw32-windres RESOURCE.rc RESOURCE.o

i686-w64-mingw32-gcc -Wall -s -nostartfiles
                     -municode -mwindows BLASTBTN.c RESOURCE.o 
                     -lwinmm -Wl,--entry=_wmain



   The rest
///////////////////////////////////////////////////////////////////////////////////
MOTIONTRAILS.exe - an attempt at an awesome effect. self explanatory.
ORBS.exe         - not so magical floating orbs that follow you around.
SPACEDUST.exe    - planets. orbits. chases. colors
WBOUNCE.exe      - fun and annoying.

The ESCAPE key is registered as a global hotkey. Push it to stop the above programs

Tested on Windows 10, 8.1, and 7.
