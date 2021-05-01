TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.c \
    ../../tziot.c \
    ../../tziotapply.c \
    ../../tziotconfig.c \
    ../../tziotconn.c \
    ../../tziotdcom.c \
    ../../tziotload.c \
    ../../tziotparsecmp.c \
    ../../tziotpipe.c \
    ../../tziotstandardlayer.c \
    ../../lib/async-clang/async.c \
    ../../lib/crc16-clang/crc16.c \
    ../../lib/dcom-clang/dcom.c \
    ../../lib/dcom-clang/dcomblockrx.c \
    ../../lib/dcom-clang/dcomblocktx.c \
    ../../lib/dcom-clang/dcomcallback.c \
    ../../lib/dcom-clang/dcomcommon.c \
    ../../lib/dcom-clang/dcomlog.c \
    ../../lib/dcom-clang/dcomparam.c \
    ../../lib/dcom-clang/dcomrx.c \
    ../../lib/dcom-clang/dcomrxcon.c \
    ../../lib/dcom-clang/dcomtx.c \
    ../../lib/dcom-clang/dcomwaitlist.c \
    ../../lib/knock-clang/knock.c \
    ../../lib/lagan-clang/lagan.c \
    ../../lib/tzbox/tzbox.c \
    ../../lib/tzfifo/tzfifo.c \
    ../../lib/tzlist/tzlist.c \
    ../../lib/tzmalloc/bget.c \
    ../../lib/tzmalloc/tzmalloc.c \
    ../../lib/tztime/tztime.c \
    ../../lib/utz-clang/utzcommon.c \
    ../../lib/utz-clang/utzehp.c \
    ../../lib/utz-clang/utzflp.c \
    ../../lib/utz-clang/utznlv1.c

LIBS += -lpthread libwsock32 libws2_32

INCLUDEPATH += ../../ \
    ../../lib/utz-clang \
    ../../lib/dcom-clang \
    ../../lib/tztime \
    ../../lib/tzmalloc \
    ../../lib/tzlist \
    ../../lib/tzfifo \
    ../../lib/lagan-clang \
    ../../lib/crc16-clang \
    ../../lib/pt \
    ../../lib/tzbox \
    ../../lib/knock-clang \
    ../../lib/async-clang \
    ../../lib/tztype-clang

HEADERS += \
    ../../tziot.h \
    ../../tziotapply.h \
    ../../tziotconfig.h \
    ../../tziotconn.h \
    ../../tziotdcom.h \
    ../../tziotload.h \
    ../../tziotparsecmp.h \
    ../../tziotpipe.h \
    ../../tziotstandardlayer.h \
    ../../lib/async-clang/async.h \
    ../../lib/crc16-clang/crc16.h \
    ../../lib/dcom-clang/dcom.h \
    ../../lib/dcom-clang/dcomblockrx.h \
    ../../lib/dcom-clang/dcomblocktx.h \
    ../../lib/dcom-clang/dcomcallback.h \
    ../../lib/dcom-clang/dcomcommon.h \
    ../../lib/dcom-clang/dcomlog.h \
    ../../lib/dcom-clang/dcomparam.h \
    ../../lib/dcom-clang/dcomprotocol.h \
    ../../lib/dcom-clang/dcomrx.h \
    ../../lib/dcom-clang/dcomrxcon.h \
    ../../lib/dcom-clang/dcomtx.h \
    ../../lib/dcom-clang/dcomwaitlist.h \
    ../../lib/knock-clang/knock.h \
    ../../lib/lagan-clang/lagan.h \
    ../../lib/pt/lc.h \
    ../../lib/pt/lc-switch.h \
    ../../lib/pt/pt.h \
    ../../lib/pt/pt-sem.h \
    ../../lib/tzbox/tzbox.h \
    ../../lib/tzfifo/tzfifo.h \
    ../../lib/tzlist/tzlist.h \
    ../../lib/tzmalloc/bget.h \
    ../../lib/tzmalloc/tzmalloc.h \
    ../../lib/tztime/tztime.h \
    ../../lib/tztype-clang/tztype.h \
    ../../lib/utz-clang/utz.h \
    ../../lib/utz-clang/utzcmp.h \
    ../../lib/utz-clang/utzcommon.h \
    ../../lib/utz-clang/utzehp.h \
    ../../lib/utz-clang/utzflp.h \
    ../../lib/utz-clang/utznlv1.h
