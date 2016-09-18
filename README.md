## Metis test application that compiles for Linux and the PlayStation using CMAKE.

Moving squares is a simple application that has a square that moves from side to side, up and down. It will also change color when the correct button is pressed.

### Controls
* UP
 * LINUX: Keyboard UP
 * PSX: D-Pad up
* DOWN
 * LINUX: Keyboard DOWN
 * PSX: D-Pad down
* LEFT
 * LINUX: Keyboard LEFT
 * PSX: D-Pad left
* RIGHT
 * LINUX: Keyboard RIGHT
 * PSX: D-Pad right
* COLOR CHANGE
 * LINUX: Keyboard x
 * PSX: x(cross) button

### Requirements

PlayStation (KUBUNTU 14.04)

	Psy-Q SDK from psxdev.net, with updated CPE2X from forums
	WINE
	CMAKE
	BUILD_ESSENTIALS
	
Linux (KUBUNTU 14.04)

        CMAKE
	GLEW (repository)
	GLFW v3.2.1 (download and compile from source)
	BUILD_ESSENTIALS
	

### Library Install Directions

#### DOWNLOAD LINKS
   1. [PlayStation Dev Kit ... psxdev.net](http://www.psxdev.net/downloads.html)
   
   2. [PlayStation CPE2X update ... psxdev.net](http://www.psxdev.net/forum/viewtopic.php?f=53&t=225)

   3. [Windows Reg File for PlayStation Dev Kit ... psxdev.net/forum](http://www.psxdev.net/forum/viewtopic.php?f=49&t=206)

   4. [GLFW library ... glfw.org](http://www.glfw.org/download.html)

Linux using WINE

        1. Install WINE
        2. Initialize WINE (winecfg works well for this)
        3. Copy psyq directory to wine drive_c directory
        4. Copy the updated CPE2X into psyq/bin, rename it to CPE2XWIN.EXE
        5. Mark all exes as executable (chmod +x *.EXE) in psyq/bin directory.
        6. Use the command "wine regedit" to import reg file downloaded from the forum.
        7. Add the psyq folders bin directory to your user .bashrc file paths.
                example: export PATH=/home/user/.wine/drive_c/psyq/bin:$PATH
        8. Check install by running in your terminal "CCPSX.EXE" should show help for the compiler.
        9. Check CPE2XWIN.EXE by running "CPE2XWIN.EXE" should give fopen error.
        11. Install other dependcies via your package manager, or download, compile and install.
        
Linux using GLFW

        1. Download, Compile, and install GLFW v3.2.1 
           (requires cmake, and will inform you of missing dependcies)
        2. Install GLEW (repository version in Ubuntu 14.04 is fine)
        3. Install other dependcies via your package manager, or download, compile and install.
	
### Build Instructions
        1st: 
                -Generate make file for system.
                cmake .
                
                OR
                
                -Generate makefile for the PlayStation using its tool chain. Will work in Linux and Windows(NOT TESTED).
                cmake . -DCMAKE_TOOLCHAIN_FILE=toolchain/psx.cmake 
                
        2nd:
                -Build using toolchain specified, will generate executable into a bin_(arch) directory.
                make


