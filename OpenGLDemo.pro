TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        3rd/glad/src/glad.c \
        main.cpp

LIBS += -lOpenGL32
LIBS += -luser32
LIBS += -lgdi32
LIBS += -lshell32
LIBS += -L$$PWD/3rd/glfw/lib/ -lglfw3


INCLUDEPATH += $$PWD/3rd/glfw/include
DEPENDPATH += $$PWD/3rd/glfw/include

INCLUDEPATH += $$PWD/3rd/glad/include
DEPENDPATH += $$PWD/3rd/glad/include
