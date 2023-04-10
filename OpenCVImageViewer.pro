QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

SOURCES += \
    imageprocessor.cpp \
    imageview.cpp \
    main.cpp \
    mainwindow.cpp \
    opencvpanel.cpp

HEADERS += \
    imageprocessor.h \
    imageview.h \
    mainwindow.h \
    opencvpanel.h

FORMS += \
    mainwindow.ui \
    opencvpanel.ui

INCLUDEPATH += $$(OPENCV_SDK_DIR)\include

LIBS += -L$$(OPENCV_SDK_DIR)/x64/mingw/lib \
    -lopencv_core470    \
    -lopencv_imgproc470 \
    -lopencv_dnn470

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
