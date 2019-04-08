Reversed the IconDance and Blast Button joke programs you may have seen on this
lovely channel here: https://www.youtube.com/user/danooct1 

https://www.youtube.com/watch?v=e5Iw3MTQMkw
https://www.youtube.com/watch?v=16XJBnjzMYU

Found them online and upon first examination I found that they were both Delphi
binaries, so I used the Interactive Delphi Reconstructor to help me get the gist
of things. Afterwards, translating the rest of the assembly to C was pretty
straightforward.

As a consequence, some parts of the originals I had to change slightly or leave
out altogether for being somewhat unecessary.

That being said, these aren't the most accurate interpretations. Performance-wise
however, I really don't see a difference.



   ICONDANCE
///////////////////////////////////////////////////////////////////////////////////

Before running it, uncheck "Auto arrange icons" and "Align icons to grid" under
"View" from the desktop context menu. Otherwise the dance will look a little stiff.

The last act will play on until message box is closed.

On first run, the window associated with the icons may get minimized and it will
look as if nothing is happening (The original did this too). The obvious fix to
this is to skip it in the Minimize callback function, but laziness got the better
of me and I left it as is. Just run it again.



   BLASTBTN
///////////////////////////////////////////////////////////////////////////////////

Not all buttons (windows) are eligible to explode.

Adjust the dimension checks to see larger windows explode. (default: 450x100)

The Escape Key on your keyboard will be registered globally as a hotkey. While the
program is running, push this key on your keyboard and it will clean up and exit.



   Compiling the above two programs
///////////////////////////////////////////////////////////////////////////////////

Microsoft's C/C++ Compiler or MinGW's GCC works. Compile as 32-bit.

If your using GCC, include these switches:
-mwindows
-municode
-nostartfiles
-Wl,--entry=_wmain

For BLASTBTN, you'll want a compatible Resource Compiler, so you can use MingGW's
"windres", again if your using GCC. Just include the resulting .o file next to the
source file on the commandline when compiling:

i686-w64-mingw32-windres RESOURCE.rc RESOURCE.o

i686-w64-mingw32-gcc -Wall -s -nostartfiles 
                      -municode -mwindows BLASTBTN.c RESOURCE.o 
                      -lwinmm -Wl,--entry=_wmain


Some other things I made:

   MOTIONTRAILS.exe
///////////////////////////////////////////////////////////////////////////////////

An attempt at replicating that crazy cool effect when windows would crash and you
could drag windows around and see its trail. You know what I'm talking about.

   ORBS.exe
///////////////////////////////////////////////////////////////////////////////////

I make 5 white orbs orbit randomly around the mouse pointer wherever it goes.


The ESCAPE key is registered as a global hotkey. Push it to exit each of the two 
above programs.

Tested on Windows 10, 8.1, and 7.



Have fun!

- Yarn
