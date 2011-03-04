set (tp_q4_logger_SRCS
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

add_definitions(-DQT_NO_KEYWORDS)

include_directories (${PROJECT_SOURCE_DIR} ${tp_qt4_logger_INCLUDE_PATHS})

add_library(telepathy-qt4-logger ${tp_q4_logger_SRCS})