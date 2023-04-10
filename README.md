# edge-detection-image-viewer

This app is an image viewer with the possibility of applying filters to an image at runtime, images dynamically change as soon as user click a checkbox or move slider at control panel. This functionality is great for seeing how image changes depending on different parameters.

App was made in Qt Creator using Qt framework and OpenCV library for image processing algorithms.

Supported features:
- opening file (*.png, *.jpeg, *.jpg, *.gif)
- zooming in/out
- going over files in directory by buttons
- applying different combination of image processing algorithms (it's recommended to blur an image or worsen it's quality for better Canny algorithm results)
- saving processed image to a file

How to run (Windows x64 only):
Download binaries from release section and run OpenCVImageViewer.exe, just it :).

How to build:
1. Clone repository
2. Open project in Qt Creator (double click on OpenCVImageViewer.pro)
3. Define system variable OPENCV_SDK_DIR with path to [OpenCVBuildDir/install]
Example (for my PC):
OPENCV_SDK_DIR = "C:\opencv-build\install"
4. Change file names of the libs in the .pro file if you use diffrent version of OpenCV (4.7.0 used), according to this pattern:
LIBS += -L$$(OPENCV_SDK_DIR)/x64/mingw/lib \
    -lopencv_core[xyz]    \
    -lopencv_imgproc[xyz] \
    -lopencv_dnn[xyz]
xyz are the numbers of the version (ex. version x.y.z = 4.7.0)
5. Add OpenCV binaries to Path environment variable.
Example path to binaries: C:\opencv-build\install\x64\mingw\bin
