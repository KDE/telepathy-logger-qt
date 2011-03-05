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

find_path (GLIB_INCLUDE_PATHS glib.h)
find_path (DBUS_INCLUDE_PATHS dbus.h)
find_path (GLIB_CONFIG_PATHS glibconfig.h)

set (TELEPATHY_INCLUDE_PATHS /usr/lib64/glib-2.0/include /usr/include/telepathy-1.0 /usr/include/telepathy-logger-0.1)

find_library (QTGLIB_LIBRARIES QtGLib-2.0)
find_library (TELEPATHY_LIBRARIES telepathy-logger telepathy-glib)
find_library (GLIB_AND_CO_LIBRARIES glib-2.0 gio-2.0 gobject-2.0 gmodule-2.0 gthread-2.0)
find_library (DBUS_LIBRARIES dbus-glib-1 dbus-1)
find_library (XML_LIBRARIES xml2)

include_directories (${PROJECT_SOURCE_DIR} ${GLIB_INCLUDE_PATHS} ${TELEPATHY_INCLUDE_PATHS} ${DBUS_INCLUDE_PATHS} ${GLIB_CONFIG_PATHS})

add_definitions (-DQT_NO_KEYWORDS)

qt4_wrap_cpp (tp_qt4_logger_MOC_CXX ${tp_qt4_logger_MOC_SRCS})

add_library (telepathy-qt4-logger SHARED ${tp_qt4_logger_MOC_CXX} ${tp_q4_logger_CXX})

target_link_libraries (telepathy-qt4-logger
    ${QT_LIBRARIES} ${XML_LIBRARIES} rt
    ${TELEPATHY_LIBRARIES} ${DBUS_LIBRARIES}
    ${GLIB_AND_CO_LIBRARIES} ${QTGLIB_LIBRARIES}
)