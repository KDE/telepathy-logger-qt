######################################################################
# Automatically generated by qmake (2.01a) sab feb 26 11:54:50 2011
######################################################################

# Paths
TEMPLATE = app
TARGET = 
DEPENDPATH += . Logger
INCLUDEPATH += . Logger

# External deps config
CONFIG += link_pkgconfig
PKGCONFIG += telepathy-logger-0.1 telepathy-glib glib-2.0

# Input
HEADERS += Logger/tpl-conversation-dates-query.h \
           Logger/tpl-messages-for-date-query.h \
           Logger/tpl-chat-exists-query.h \
           Logger/tpl-keyword-query.h \
           Logger/tpl-chats-for-account-query.h \
           Logger/tpl-error.h \
           Logger/tpl-query.h \
           Logger/tpl-message.h \
           Logger/tpl-correspondant.h \
           Logger/tpl-debug.h

SOURCES += tpl-query.cpp tpl-debug.cpp \
           tpl-message.cpp tpl-error.cpp \
query-log.cpp dummy-main.cpp

# Output
OBJECTS_DIR = .objects
MOC_DIR = .meta
RCC_DIR = .meta