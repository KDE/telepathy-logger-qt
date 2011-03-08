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
     query.h
     debug.h
)

# Automagically setup include paths for external deps.
find_package(GLIB2 REQUIRED)
find_package(GObject REQUIRED)
find_package(DBus REQUIRED)
find_package(DBusGLib REQUIRED)
find_package(LibXml2 REQUIRED)
find_package(TelepathyGlib REQUIRED)
find_package(QtGLib REQUIRED)
find_package(TelepathyLogger REQUIRED)

# Make tpl-qt4 to include all the paths above.
include_directories (
    ${PROJECT_SOURCE_DIR}
    ${GLIB2_INCLUDE_DIR}
    ${GOBJECT_INCLUDE_DIR}
    ${LIBXML2_INCLUDE_DIR}
    ${DBUS_INCLUDE_DIR}
    ${DBUS_GLIB_INCLUDE_DIR}
    ${TELEPATHY_GLIB_INCLUDE_DIR}
    ${TELEPATHY_LOGGER_INCLUDE_DIR}
    ${QTGLIB_INCLUDE_DIR}
)

# Compile MOC srcs.
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
    ${QT_LIBRARIES}
    ${GLIB2_LIBRARIES}
    ${GOBJECT_LIBRARIES}
    ${LIBXML2_LIBRARIES}
    ${DBUS_LIBRARIES}
    ${DBUS_GLIB_LIBRARIES}
    ${TELEPATHY_GLIB_LIBRARIES}
    ${TELEPATHY_LOGGER_LIBRARIES}
    ${QTGLIB_LIBRARIES}
)