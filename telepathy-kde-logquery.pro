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
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

# May change to 'lib' when testing is over :)
TEMPLATE = app
TARGET = telepathy-kde-logquery
DEPENDPATH += . Logger
INCLUDEPATH += . Logger

# Disable Qt keywords, necessary to include glib headers.
# As an option, use -DQT_NO_KEYWORDS in the build system
CONFIG += no_keywords

# External deps config
CONFIG += link_pkgconfig
PKGCONFIG += telepathy-logger-0.1 telepathy-glib glib-2.0

# Input
HEADERS += tpl-correspondant-private.h \
           tpl-message-private.h \
           tpl-query-private.h \
           tpl-query-callback-template.h \
           Logger/tpl-chat-exists-query.h \
           Logger/tpl-chats-for-account-query.h \
           Logger/tpl-conversation-dates-query.h \
           Logger/tpl-correspondant.h \
           Logger/tpl-debug.h \
           Logger/tpl-error.h \
           Logger/tpl-hit.h \
           Logger/tpl-query-main-loop.h \
           Logger/tpl-keyword-query.h \
           Logger/tpl-message.h \
           Logger/tpl-messages-for-date-query.h \
           Logger/tpl-query.h \
           Logger/Log \
           Logger/Correspondant \
           Logger/Query \
           Logger/Message \
           Logger/Hit \
           Logger/QueryMainLoop

SOURCES += tpl-chat-exists-query.cpp \
           tpl-conversation-dates-query.cpp \
           tpl-messages-for-date-query.cpp \
           tpl-keyword-query.cpp \
           tpl-chats-for-account-query.cpp \
           tpl-filter-query.cpp \
           tpl-correspondant.cpp \
           tpl-message.cpp \
           tpl-query.cpp \
           tpl-debug.cpp \
           tpl-error.cpp \
           tpl-query-main-loop.cpp \
           tpl-correspondant-private.cpp \
           tpl-message-private.cpp \
           tpl-query-private.cpp \
           dummy-main.cpp

# Output
OBJECTS_DIR = .objects
MOC_DIR = .meta
RCC_DIR = .meta
