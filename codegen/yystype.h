/*
    Copyright (C) 2010  George Kiagiadakis <kiagiadakis.george@gmail.com>

    This library is free software; you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <QtCore/QList>
#include <QtCore/QByteArray>
#include <QtCore/QHash>

typedef union YYSTYPE {
    QByteArray *Id;
    QList<QByteArray> *IdList;
    QHash<QByteArray, QByteArray> *HashMap;
    QPair<QByteArray, QByteArray> *Pair;
} YYSTYPE;

#define YYSTYPE_IS_DECLARED
#include "parser.hpp"
