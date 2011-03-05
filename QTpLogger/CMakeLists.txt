# Copyright (C) 2011 Stefano Sanfilippo <stefano.k.sanfilippo@gmail.com>
#
# @author Stefano Sanfilippo <stefano.k.sanfilippo@gmail.com>
#
# This library is free software; you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published
# by the Free Software Foundation; either version 2.1 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>

# Include Qt4 setup stuff and inhibit keywords.
include(${QT_USE_FILE})
add_definitions (-DQT_NO_KEYWORDS)

# Source code files.
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

# MOC source code files.
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

# Setup include paths for external deps. TODO
set (GLIB_INCLUDE_PATHS /usr/include/glib-2.0 /usr/lib64/glib-2.0/include)
set (DBUS_INCLUDE_PATHS /usr/include/dbus-1.0 /usr/lib64/dbus-1.0/include)
set (QGLIB_INCLUDE_PATHS /usr/include/QtGStreamer /usr/include/QtCore /usr/include/glib-2.0 /usr/lib64/glib-2.0/include)
set (TELEPATHY_INCLUDE_PATHS /usr/lib64/glib-2.0/include /usr/include/telepathy-1.0 /usr/include/telepathy-logger-0.1)

# Setup linker paths.
find_library (QTGLIB_LIBRARIES
    QtGLib-2.0
)
find_library (TELEPATHY_LIBRARIES
    telepathy-logger
    telepathy-glib
)
find_library (GLIB_AND_CO_LIBRARIES
    glib-2.0
    gio-2.0
    gobject-2.0
    gmodule-2.0
    gthread-2.0
)
find_library (DBUS_LIBRARIES
    dbus-glib-1
    dbus-1
)
find_library (XML_LIBRARIES
    xml2
)

# Make tpl-qt4 to include all the paths above.
include_directories (
    ${PROJECT_SOURCE_DIR}
    ${GLIB_INCLUDE_PATHS}
    ${TELEPATHY_INCLUDE_PATHS}
    ${QGLIB_INCLUDE_PATHS}
    ${DBUS_INCLUDE_PATHS}
)

# Compile MOX srcs.
qt4_wrap_cpp (tp_qt4_logger_MOC_CXX
  ${tp_qt4_logger_MOC_SRCS}
)

# Compile and link as a shared object.
add_library (telepathy-qt4-logger SHARED
    ${tp_qt4_logger_MOC_CXX}
    ${tp_q4_logger_CXX}
)

# Link against all deps above (to check for linker errors)
target_link_libraries (telepathy-qt4-logger
    ${QT_LIBRARIES} ${XML_LIBRARIES} rt
    ${TELEPATHY_LIBRARIES} ${DBUS_LIBRARIES}
    ${GLIB_AND_CO_LIBRARIES} ${QTGLIB_LIBRARIES}
)