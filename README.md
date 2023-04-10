# Edge Detection Image Viewer

This app is an image viewer with the possibility of applying filters to an image at runtime, images dynamically change as soon as the user clicks a checkbox or moves the slider at the control panel. This functionality is great for seeing how the image changes depending on different parameters.

App was made in Qt Creator using Qt framework and OpenCV library for image processing algorithms.

Supported features:

- Opening file (*.png, *.jpeg, *.jpg, *.gif)
- Zooming in/out
- Going over files in directory by buttons
- Applying different combinations of image processing algorithms (it's recommended to blur an image or worsen its quality for better Canny algorithm results)
- Saving processed image to a file

<p align="center">
    <img src="https://user-images.githubusercontent.com/90553150/230987466-980042a7-d640-44f5-b5b2-c6b89f657925.jpg" width="30%">
    <img src="https://user-images.githubusercontent.com/90553150/230987322-8112fd9f-d5c0-4b4a-b98d-939a21222b7b.png" width="30%">
    <img src="https://user-images.githubusercontent.com/90553150/230987416-cb6a4585-54bb-4fd5-9db3-946bc0682b2e.png" width="30%">
</p>
<p align="center">
  <em>Left: Before | Middle: After - Canny + quality decrease | Right: After - Holistically-Nested Edge Detection (HED)</em>
</p>

## How to run (Windows x64 only)

1. Download binaries from the release section and run `OpenCVImageViewer.exe`.

## How to build

1. Clone the repository
2. Open the project in Qt Creator (double click on `OpenCVImageViewer.pro`)
3. Define the system variable `OPENCV_SDK_DIR` with the path to `[OpenCVBuildDir/install]`. Example (for my PC): `OPENCV_SDK_DIR = "C:\opencv-build\install"`
4. Change file names of the libs in the `.pro` file if you use a different version of OpenCV (4.7.0 used), according to this pattern:
LIBS += -L$$(OPENCV_SDK_DIR)/x64/mingw/lib
-lopencv_core[xyz]
-lopencv_imgproc[xyz]
-lopencv_dnn[xyz]
`xyz` are the numbers of the version (ex. version x.y.z = 4.7.0)
5. Add OpenCV binaries to the Path environment variable. Example path to binaries: `C:\opencv-build\install\x64\mingw\bin`
6. Qt Creator restart is sometimes needed for reloading system variables.
