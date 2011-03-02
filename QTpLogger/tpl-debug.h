#ifndef __TPL_DEBUG__
#define __TPL_DEBUG__

/*
 * Copyright (C) 2011 Stefano Sanfilippo <stefano.k.sanfilippo@gmail.com>
 *
 * @author Stefano Sanfilippo <stefano.k.sanfilippo@gmail.com>
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <QTpLogger/Entity>
#include <QTpLogger/Entry>
#include <QTpLogger/SearchHit>

#include <QtCore/QObject>
#include <QtCore/QDate>

namespace Logger
{

class Debug : public QObject
{
Q_OBJECT

public:
    Debug() : QObject() {}

public Q_SLOTS:
    void echo(bool yes);
    void echo(const QList<QDate> &dates);
	void echo(const QList<Entry> &messages);
	void echo(const QList<Entity> &buddies);
    void echo(const QList<SearchHit> &hits);
};

} // namespace

#endif // __TPL_DEBUG__
