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
DEPENDPATH += . QTpLogger
INCLUDEPATH += . QTpLogger

# Disable Qt keywords, necessary to include glib headers.
# As an option, use -DQT_NO_KEYWORDS in the build system
CONFIG += no_keywords

# External deps config
CONFIG += link_pkgconfig
PKGCONFIG += telepathy-logger-0.1 telepathy-glib glib-2.0

# Input
HEADERS += QTpLogger/tpl-chat-exists-query.h \
           QTpLogger/tpl-chats-for-account-query.h \
           QTpLogger/tpl-conversation-dates-query.h \
           QTpLogger/tpl-messages-for-date-query.h \
           QTpLogger/tpl-filter-query.h \
           QTpLogger/tpl-keyword-query.h \
           \
           \
           QTpLogger/tpl-query-callback-template.h \
           QTpLogger/tpl-query-private.h \
           \
           QTpLogger/tpl-debug.h \
           QTpLogger/tpl-error.h \
           \
           QTpLogger/tpl-query.h \
           QTpLogger/tpl-search-hit.h \
    QTpLogger/tpl-entry.h \
    QTpLogger/tpl-entry-private.h \
    QTpLogger/tpl-entity.h \
    QTpLogger/tpl-entity-private.h

SOURCES += QTpLogger/tpl-chat-exists-query.cpp \
           QTpLogger/tpl-chats-for-account-query.cpp \
           QTpLogger/tpl-conversation-dates-query.cpp \
           QTpLogger/tpl-debug.cpp \
           QTpLogger/tpl-error.cpp \
           QTpLogger/tpl-filter-query.cpp \
           QTpLogger/tpl-keyword-query.cpp \
           QTpLogger/tpl-messages-for-date-query.cpp \
           QTpLogger/tpl-query-private.cpp \
           QTpLogger/tpl-query.cpp \
           QTpLogger/tpl-search-hit.cpp \
           \
           tests/dummy-main.cpp \
    QTpLogger/tpl-entry-private.cpp \
    QTpLogger/tpl-entry.cpp \
    QTpLogger/tpl-entity-private.cpp \
    QTpLogger/tpl-entity.cpp

# Output
OBJECTS_DIR = build
MOC_DIR = build
RCC_DIR = build
