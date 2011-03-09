# Copyright (C) 2011 Stefano Sanfilippo <stefano.k.sanfilippo@gmail.com>
# Copyright (C) 2011 Collabora Ltd. <http://www.collabora.co.uk/>
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
TARGET = TelepathyQt4Logger-test
DEPENDPATH += . TelepathyQt4Logger
INCLUDEPATH += . TelepathyQt4Logger
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
HEADERS += TelepathyQt4Logger/Entity \
           TelepathyQt4Logger/Event \
           TelepathyQt4Logger/TextEvent \
           TelepathyQt4Logger/LogManager \
           TelepathyQt4Logger/PendingOperation \
           TelepathyQt4Logger/PendingDates \
           TelepathyQt4Logger/PendingEntities \
           TelepathyQt4Logger/PendingEvents \
           TelepathyQt4Logger/PendingSearch \
           TelepathyQt4Logger/Global \
           TelepathyQt4Logger/Constants \
           TelepathyQt4Logger/Types \
           \
           TelepathyQt4Logger/entity.h \
           TelepathyQt4Logger/event.h \
           TelepathyQt4Logger/text-event.h \
           TelepathyQt4Logger/log-manager.h \
           TelepathyQt4Logger/pending-operation.h \
           TelepathyQt4Logger/pending-dates.h \
           TelepathyQt4Logger/pending-entities.h \
           TelepathyQt4Logger/pending-events.h \
           TelepathyQt4Logger/pending-search.h \
           TelepathyQt4Logger/pending-tpcontacts.h \
           TelepathyQt4Logger/global.h \
           TelepathyQt4Logger/constants.h \
           TelepathyQt4Logger/types.h \
           TelepathyQt4Logger/utils.h

SOURCES += TelepathyQt4Logger/entity.cpp \
           TelepathyQt4Logger/event.cpp \
           TelepathyQt4Logger/text-event.cpp \
           TelepathyQt4Logger/log-manager.cpp \
           TelepathyQt4Logger/pending-operation.cpp \
           TelepathyQt4Logger/pending-dates.cpp \
           TelepathyQt4Logger/pending-entities.cpp \
           TelepathyQt4Logger/pending-events.cpp \
           TelepathyQt4Logger/pending-search.cpp \
           TelepathyQt4Logger/pending-tpcontacts.cpp \
           TelepathyQt4Logger/utils.cpp \
           \
           tests/dummy-main.cpp

# Output
OBJECTS_DIR = build
MOC_DIR = build
RCC_DIR = build
