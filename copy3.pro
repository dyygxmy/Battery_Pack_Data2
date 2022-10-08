######################################################################
# Automatically generated by qmake (2.01a) ?? 4? 23 17:10:15 2015
######################################################################

QT       += core  gui   xml  sql  network
TEMPLATE = app
CONFIG += C++11
TARGET = V2.166
DEPENDPATH += .
INCLUDEPATH += .
CFLAGS += -Wno-psabi
# Input
#include(./source/websocket.pri)
HEADERS += \
           GlobalVarible.h \
           mainwindow.h \
           worker.h \
    newconfiginfo.h \
    TestInput_Cursor/formhanzitable.h \
    TestInput_Cursor/keyboard.h \
    TestInput_Cursor/dialogtooltip.h \
    TestInput_Cursor/ChineseTranslator.h \
    TestInput_Cursor/formhanziselector.h \
    TestInput_Cursor/formnumpad.h \
    TestInput_Cursor/HanziManager.h \
    TestInput_Cursor/formsymbolpad.h \
    TestInput_Cursor/inputmethod.h \
    TestInput_Cursor/padbase.h \
    optiondialog.h \
    save.h \
    basicset.h \
    communication.h \
    tightenthread.h \
    inputevents.h \
    wificonnect.h \
    taotong.h \
    vinreverse.h \
    rfidcontroler.h \
    serialComs.h \
    json/FlexLexer.h \
    json/json_parser.hh \
    json/json_scanner.h \
    json/location.hh \
    json/parser_p.h \
    json/parser.h \
    json/parserrunnable.h \
    json/position.hh \
    json/qjson_debug.h \
    json/qjson_export.h \
    json/qobjecthelper.h \
    json/serializer.h \
    json/serializerrunnable.h \
    json/stack.hh \
    rootdialog.h \
    connectplc.h \
    passwordpanel.h \
    paintarea.h \
    qextserial/qextserialport.h \
    qextserial/qextserialport_global.h \
    logdelete.h \
    sqlthreadsvw2.h \
    mesuploadsvw2.h \
    stepcontrol.h \
    codeedit.h \
    inoutput.h \
    interface.h \
    readw500.h \
    serialComsRead.h \
    stepedit.h \
    stepparameter.h \
    parsejson.h \
    airtightnesstest.h \
    inputmodule.h \
    airtest.h \
    mastertcp.h \
    stepeditbev.h \
    dialogbindtray.h \
    RollSubtitleLable.h
FORMS += \
         mainwindow.ui \
    newconfiginfo.ui \
    TestInput_Cursor/formsymbolpad.ui \
    TestInput_Cursor/formnumpad.ui \
    TestInput_Cursor/formhanziselector.ui \
    TestInput_Cursor/keyboard.ui \
    TestInput_Cursor/dialogtooltip.ui \
    TestInput_Cursor/formhanzitable.ui \
    optiondialog.ui \
    save.ui \
    basicset.ui \
    rootdialog.ui \
    passwordpanel.ui \
    codeedit.ui \
    stepedit.ui \
    stepparameter.ui \
    stepeditbev.ui \
    dialogbindtray.ui
SOURCES += \
           main.cpp \
           mainwindow.cpp \
           worker.cpp \
    newconfiginfo.cpp \
    TestInput_Cursor/formhanzitable.cpp \
    TestInput_Cursor/formsymbolpad.cpp \
    TestInput_Cursor/HanziManager.cpp \
    TestInput_Cursor/formhanziselector.cpp \
    TestInput_Cursor/formnumpad.cpp \
    TestInput_Cursor/padbase.cpp \
    TestInput_Cursor/keyboard.cpp \
    TestInput_Cursor/dialogtooltip.cpp \
    TestInput_Cursor/inputmethod.cpp \
    optiondialog.cpp \
    save.cpp \
    basicset.cpp \
    communication.cpp \
    tightenthread.cpp \
    inputevents.cpp \
    wificonnect.cpp \
    taotong.cpp \
    vinreverse.cpp \
    rfidcontroler.cpp \
    serialComs.cpp \
    json/json_parser.cc \
    json/json_scanner.cc \
    json/json_scanner.cpp \
    json/parser.cpp \
    json/parserrunnable.cpp \
    json/qobjecthelper.cpp \
    json/serializer.cpp \
    json/serializerrunnable.cpp \
    rootdialog.cpp \
    connectplc.cpp \
    passwordpanel.cpp \
    paintarea.cpp \
    qextserial/qextserialport_unix.cpp \
    qextserial/qextserialport.cpp \
    logdelete.cpp \
    sqlthreadsvw2.cpp \
    mesuploadsvw2.cpp \
    stepcontrol.cpp \
    codeedit.cpp \
    inoutput.cpp \
    interface.cpp \
    readw500.cpp \
    serialComsRead.cpp \
    stepedit.cpp \
    stepparameter.cpp \
    parsejson.cpp \
    airtightnesstest.cpp \
    inputmodule.cpp \
    airtest.cpp \
    mastertcp.cpp \
    stepeditbev.cpp \
    dialogbindtray.cpp \
    RollSubtitleLable.cpp
RESOURCES += file.qrc \
    TestInput_Cursor/keypad.qrc

OTHER_FILES +=

UI_DIR=./UI
