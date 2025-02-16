QT       += core gui multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

QMAKE_PROJECT_DEPTH = 0

# 设置 Debug 和 Release 不同的输出目录
debug {
    # 设置最终可执行文件的输出目录
    DESTDIR = $$PWD/bin/debug
    # 设置对象文件的输出目录
    OBJECTS_DIR = $$PWD/bin/debug/objects
    # 设置 MOC 文件的输出目录
    MOC_DIR = $$PWD/bin/debug/moc
    # 设置 UI 文件的输出目录
    UI_DIR = $$PWD/bin/debug/ui
    # 设置资源文件的输出目录
    RESOURCES_DIR = $$PWD/debug/bin/resources
}

release {
    # 设置最终可执行文件的输出目录
    DESTDIR = $$PWD/bin/release
    # 设置对象文件的输出目录
    OBJECTS_DIR = $$PWD/bin/release/objects
    # 设置 MOC 文件的输出目录
    MOC_DIR = $$PWD/bin/release/moc
    # 设置 UI 文件的输出目录
    UI_DIR = $$PWD/bin/release/ui
    # 设置资源文件的输出目录
    RESOURCES_DIR = $$PWD/release/bin/resources
}

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += $$PWD/include \
                $$PWD/include/ffmpeg

LIBS += -L. -lws2_32
LIBS += -L$$PWD/lib \
        -L$$PWD/lib/ffmpeg

LIBS += -lavcodec \
        -lavdevice \
        -lavfilter \
        -lavformat \
        -lavutil \
        -lpostproc \
        -lswresample \
        -lswscale

debug{
    LIBS += -lopencv_world4100d
}

release{
    LIBS += -lopencv_world4100
}

DEFINES += _WINSOCK_DEPRECATED_NO_WARNINGS

SOURCES += \
    $$PWD/src/main.cpp \
    $$PWD/src/mainwindow.cpp \
    $$PWD/src/client.cpp \
    src/chatinterface.cpp \
    src/addfriendinterface.cpp \
    src/imsystemopencv.cpp \
    src/myqlabel.cpp \
    src/opencvwidget.cpp \
    src/register.cpp \
    src/imsystem.cpp \
    src/usrinterface.cpp \
    src/videointerface.cpp

HEADERS += \
    $$PWD/src/mainwindow.h \
    $$PWD/src/client.h \
    src/chatinterface.h \
    src/addfriendinterface.h \
    src/imsystemopencv.h \
    src/myqlabel.h \
    src/opencvwidget.h \
    src/register.h \
    src/Message.h \
    src/imsystem.h \
    src/usrinterface.h \
    src/videointerface.h

FORMS += \
    addfriendinterface.ui \
    chatinterface.ui \
    mainwindow.ui \
    register.ui \
    usrinterface.ui \
    videointerface.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Image.qrc
