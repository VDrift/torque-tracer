this program was developed to ease data entry for vdrift (vdrift.net) a great opensource race car game.

requirements: a mildly competent opengl card.  source tested to compile under ubuntu/codeblocks/amd64 and winxp/visualc6/x86, requires opengl, sdl, sdl_image, and freetype.

on windows all these libraries must be in the directory of the program or the windows system directory.



instructions for use
========================================================================
1) find a torque curve you want to trace.

2) name it "curve.jpg" and put it in the same directory as this this program

3) run the program

4) click near the edges or corners of the image to resize the image until it lines up with the graph (pay attention to torque, not horsepower).  you will probably have to re-adjust each side several times because as the image stretches things you had lined up before will get a little out or line.

5) press 'm' to shift the mode into draw mode

6) if your graph is in ft-lbs press 'u' once. to change the units from metric to imperial (it's important to do this before you start drawing)

8) start tracing the curve by clicking the mouse the first point you place won't be visible until you place a second.

9) if you mess up right click to remove the point that is nearest to your cursor.

10) after you're done, press 'w' to write the curve in a vdrift acceptable format to "torque_cuve.txt"

11) press escape to quit



configuration.ini
========================================================================
very simplistic video configuration.  type in the resolution and bpp you want.  fullscreen and hardware cursor are booleans (0 is "no", everything else is "yes").  if you experience cursor lag (from a slower graphics card) try changing the hardware cursor to 0.  for most accurate tracing you should set the resolution to your monitors maximum native resolution, and set fullscreen to 1.  but even low res it should give pretty good results.





enter text mode (complete but may need testing)
========================================================================
whenever you see a cars quoted torque and horsepower from the manufacturer it says something like this:
220 bhp @ 5500 rpm, 180 ft-lb @ 3000 rpm

by pressing the 'm' key a second time you can also get to enter text mode which will allow you to define points like these.
once in enter text mode type the number for torque or horsepower, then type a letter that corresponds to the units.

n = Newton meter (Nm)
f = foot-pounds (ft-lbs)
k = kiloWatt (kW)
h or b = brake horsepower (bhp)

then type in the rpm at which this peak occurs.

then press enter.  your point should be placed (regarding the appropriate conversions) where you specified.


