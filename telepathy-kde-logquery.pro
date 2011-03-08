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

# May change to 'lib' and remove -test suffix when testing is over :)
TEMPLATE = app
TARGET = QTpLogger-test
DEPENDPATH += . QTpLogger
INCLUDEPATH += . QTpLogger
CONFIG += qdbus

# Disable Qt keywords, necessary to include glib headers.
# As an option, use -DQT_NO_KEYWORDS in the build system
CONFIG += no_keywords

# Disable verbose gcc command echo, your eyes will thank you
CONFIG += silent

# External deps config
CONFIG += link_pkgconfig
PKGCONFIG += telepathy-logger-0.2 telepathy-glib glib-2.0 QtGLib-2.0 TelepathyQt4

# Input
HEADERS += QTpLogger/query-callback-template.h \
           QTpLogger/query-private.h \
           \
           QTpLogger/entity.h \
           QTpLogger/event.h \
           QTpLogger/text-event.h \
           QTpLogger/log-manager.h \
           QTpLogger/pending-operation.h \
           QTpLogger/pending-dates.h \
           QTpLogger/pending-entities.h \
           QTpLogger/pending-events.h \
           QTpLogger/pending-search.h \
           QTpLogger/pending-tpcontacts.h \
           QTpLogger/global.h \
           QTpLogger/constants.h \
           QTpLogger/types.h \
           \
           QTpLogger/query.h

SOURCES += QTpLogger/query.cpp \
           QTpLogger/query-private.cpp \
           \
           QTpLogger/entity.cpp \
           QTpLogger/event.cpp \
           QTpLogger/text-event.cpp \
           QTpLogger/log-manager.cpp \
           QTpLogger/pending-operation.cpp \
           QTpLogger/pending-dates.cpp \
           QTpLogger/pending-entities.cpp \
           QTpLogger/pending-events.cpp \
           QTpLogger/pending-search.cpp \
           QTpLogger/pending-tpcontacts.cpp \
           \
           tests/dummy-main.cpp

# Output
OBJECTS_DIR = build
MOC_DIR = build
RCC_DIR = build
