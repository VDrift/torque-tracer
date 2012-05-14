# Torque Tracer

This program was developed to ease data entry for [VDrift](http://vdrift.net) a great cross-platform, open source driving simulation made with drift racing in mind.

## Requirements

* A mildly competent opengl card.
* The following libraries, provided on OS X, on Windows must be in the directory of the program or the Windows system directory:
	* [Freetype](http://freetype.org/)
	* [OpenGL](http://opengl.org/)
	* [SDL](http://libsdl.org/download.php)
	* [SDL_image](http://libsdl.org/projects/SDL_image/)

Source tested to compile under ubuntu/codeblocks/amd64, winxp/visualc6/x86, and osx lion/xcode4/i386, x86-64

## Instructions

* Find a torque curve you want to trace.
* Name it "curve.jpg" and put it in the same directory as this program.
* Run the program.
* Drag the edges or corners of the image to resize it until it lines up with the graph (pay attention to torque, not horsepower). You will probably have to re-adjust each side several times because as the image stretches things you had lined up before will get a little out of line. You can adjust the vertical scale if you want with '=' and '-'.
* Press 'm' to change into draw mode.
* If your graph is in ft-lbs press 'u' once to change the units from metric to imperial (it's important to do this before you start drawing).
* Start tracing the curve by clicking the mouse. The first point you place won't be visible until you place a second.
* If you mess up right click to remove the point which is nearest to your cursor.
* After you're done, press 'w' to write the curve in a VDrift acceptable format to "torque_curve.txt".
* Press escape to quit.

## configuration.ini

There are a few video options you can set in "configuration.ini" located in the same directory as this program:

* display\_x = x resolution
* display\_y = y resolution
* display\_bpp = color depth
* fullscreen = 0 is off, anything else is on
* hardware\_cursor = 0 uses system cursor, anything else uses "mouse.png" (located in Torque Tracer.app/Contents/Resources on OS X)

If you experience cursor lag (from a slower graphics card) try changing the hardware cursor to 0.
For the most accurate tracing you should set the resolution to your monitors maximum native resolution, and turn on fullscreen, however even with a low resolution you should get pretty good results.

## Text Mode

(complete but may need testing)  
When you see a car's quoted torque and horsepower from the manufacturer it says something like this:

> 220 bhp @ 5500 rpm  
> 180 ft-lb @ 3000 rpm

* Press 'm' a second time to enter the text mode which will allow you to define points like these.
* Type the number for torque or horsepower.
* Then type a letter that corresponds to the unit:
	* n = Newton meter (Nm)
	* f = foot-pounds (ft-lbs)
	* k = kiloWatt (kW)
	* h or b = brake horsepower (bhp)
* Then type in the rpm at which this peak occurs.
* Finally press enter. Your point should be placed (with the appropriate conversions) where you specified.
