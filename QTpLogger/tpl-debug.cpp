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

#include "tpl-debug.h"

#include <QtCore/QDebug>

using namespace QTpLogger;

void Debug::echo(bool yes)
{
    qDebug("Logger::Debug::echo(bool) was called");

    qDebug() << yes;
}

void Debug::echo(const QList<QDate> &dates)
{
    qDebug("Logger::Debug::echo(QList<QDate>) was called");

    if (dates.isEmpty()) return;

    qDebug() << dates;
}

void Debug::echo(const QList<Entry> &messages)
{
    qDebug("Logger::Debug::echo(QList<Message>) was called");

    if (messages.isEmpty()) return;

    Q_FOREACH (Entry m, messages) { // TODO overload Message <<
        qDebug() << m.chatid();
    }
}

void Debug::echo(const QList<Entity> &buddies)
{
    qDebug("Logger::Debug::echo(QList<Correspondant>) was called");

    Q_FOREACH (Entity b, buddies) { // TODO overload Message <<
        qDebug() << b.id();
    }
}

void Debug::echo(const QList<SearchHit> &hits)
{
    qDebug("Logger::Debug::echo(QList<Hit>) was called");

    (void)hits; //FIXME
}
