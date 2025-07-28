Adapt "Source\setup.cfg" to your directory structure, the IP adresses of your camera servers (especially CameraIPAddress and ports) and your screen size. You don't need to worry about all the parameters in this setup file. Most of them can remain unchanged.

To run Vision, use "vision.bat". It will call "Vision.exe". 
If Vision ends for some reason (camera crash, out of sync with control program,...) it will start again automatically, which is very convenient.

To test vision, load "ExampleData\13Nov09\DataB.dat" using Series->Load Series All Cameras.

Additional information is contained in Manual\Manual.pdf.


To control the cameras, install the Visual C++ program "AndorServerLucaSrBEC - LowCPUPower" on the camera server computers (which could in principle be the same computer as the one on which Vision is running if you have just one camera).
If you don't have a Andor Luca camera, adapt this program to your camera.

It is convenient to use a remote desktop program like VNCServer to access the camera servers from the Vision computer.

If you need to change Vision, you can recompile it using Visual C++ 5.02.