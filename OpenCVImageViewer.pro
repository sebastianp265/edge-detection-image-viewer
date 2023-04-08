QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    imageview.cpp \
    main.cpp \
    mainwindow.cpp \
    opencvutils.cpp

HEADERS += \
    imageview.h \
    mainwindow.h \
    opencvutils.h

FORMS += \
    mainwindow.ui

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
