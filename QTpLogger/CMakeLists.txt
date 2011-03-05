include(${QT_USE_FILE})

set (tp_q4_logger_CXX
     chat-exists-query.cpp
     chats-for-account-query.cpp
     conversation-dates-query.cpp
     filter-query.cpp
     keyword-query.cpp
     messages-for-date-query.cpp
     query.cpp
     query-private.cpp
     search-hit.cpp
     entry.cpp
     entry-private.cpp
     entry-text-private.cpp
     entity-private.cpp
     entity.cpp
     debug.cpp
)

set (tp_qt4_logger_MOC_SRCS
     chat-exists-query.h
     chats-for-account-query.h
     conversation-dates-query.h
     messages-for-date-query.h
     filter-query.h
     keyword-query.h
     search-hit.h
     query.h
     debug.h
)

set (tp_qt4_logger_INCLUDE_PATHS
     /usr/include/telepathy-logger-0.1
     /usr/include/telepathy-1.0
     /usr/include/libxml2
     /usr/include/dbus-1.0
     /usr/include/glib-2.0
     /usr/lib64/glib-2.0/include
     /usr/lib64/dbus-1.0/include
     /usr/include/QtGStreamer
)

include_directories (${PROJECT_SOURCE_DIR} ${tp_qt4_logger_INCLUDE_PATHS})

add_definitions (-DQT_NO_KEYWORDS)

qt4_wrap_cpp (tp_qt4_logger_MOC_CXX ${tp_qt4_logger_MOC_SRCS})

add_library (telepathy-qt4-logger SHARED ${tp_qt4_logger_MOC_CXX} ${tp_q4_logger_CXX})

target_link_libraries (telepathy-qt4-logger
    QtGLib-2.0 QtCore QtGui telepathy-logger
    telepathy-glib xml2 dbus-glib-1 gio-2.0
    dbus-1 gobject-2.0 gmodule-2.0
    gthread-2.0 rt glib-2.0
)