# How to set OpenCV in Visual Studios environment

## Download files
1. Download [OpenCV-2.3.0-win-src.zip](http://sourceforge.net/projects/opencvlibrary/files/opencv-win/ 
) and unzip  
**Make sure the version is 2.3.0! My codes work best in 2.3.0**
2. Delete all content except bin, lib, include folder  
3. For alternative, I've uploaded pre-compiled [OpenCV](https://github.com/ameliacode/Multimedia_programming/OpenCV-2.3.0). You can just use this instead

## Setup directories
1. Copy **OpenCV-2.3.0** and paste the folders in the project directory

## Setup in Visual Studios
From the menu bar, click **Project > Properties**  
Make sure Configuration is **Active(Debug)** and Platform is **Active(Win32)**

1. VC++ Directories > Include directories:
```
$(SolutionDir) OpenCV-2.3.0\include
```


2. VC++ Directories > Library directories:
```
$(SolutionDir) OpenCV-2.3.0\lib
```


3. Linker > Input > Additional dependencies:
```
opencv_core230.lib 
opencv_highgui230.lib 
opencv_imgproc230.lib
```
4. Add **#include <opencv2/opencv.hpp>** in source code