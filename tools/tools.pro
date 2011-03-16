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

TEMPLATE = app
TARGET = tpl-tool
DEPENDPATH += ..
INCLUDEPATH += ..
CONFIG += qdbus

QMAKE_CXXFLAGS_WARN_ON = -Wall

# Disable Qt keywords, necessary to include glib headers.
# As an option, use -DQT_NO_KEYWORDS in the build system
CONFIG += no_keywords

# Disable verbose gcc command echo, your eyes will thank you
CONFIG += silent

# External deps config
CONFIG += link_pkgconfig
PKGCONFIG += TelepathyQt4Logger TelepathyQt4 QtGLib-2.0 telepathy-logger-0.2

# Input
HEADERS += tpl-tool.h

SOURCES += tpl-tool.cpp

# Output
OBJECTS_DIR = build
MOC_DIR = build
RCC_DIR = build
