

Welp I've had these for a few months now since reversing some samples I managed to
get a whiles back. When I first looked at them they were both Delphi binaries, so I
used the Interactive Delphi Recontructor to help me understand some of the function
calls. After getting the gist of things, translating the rest of the assembly to C
was pretty straightforward.

I only wanted to know how the interesting bits worked and not get caught up in 
little details here and there, so some parts of the originals I changed slightly 
or left out altogether for being somewhat unecessary.

That being said, these aren't the most accurate interpretations. Performance-wise
however, I really don't see a difference.



   ICONDANCE
///////////////////////////////////////////////////////////////////////////////////

Before running it, uncheck "Auto arrange icons" and "Align icons to grid" under
"View" from the desktop context menu. Otherwise the dance will look a little stiff.



   BLASTBTN
///////////////////////////////////////////////////////////////////////////////////

Not all buttons (windows) are eligible to explode.

Adjust the dimension checks to see larger windows explode. (default: 450x100)

The Escape Key on your keyboard will be registered globally as a hotkey. While the
program is running, push this key on your keyboard and it will clean up and exit.



   Compiling
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

i686w-w64-mingw32-windres RESOURCE.rc RESOURCE.o

i686w-w64-mingw32-gcc -Wall -s -nostartfiles 
                      -municode -mwindows BLASTBTN.c RESOURCE.o 
                      -lwinmm -Wl,--entry=_wmain




- Yarn
