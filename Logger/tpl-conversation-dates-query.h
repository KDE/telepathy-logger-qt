#ifndef __CONVERSATION_DATES_QUERY__
#define __CONVERSATION_DATES_QUERY__

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

#include <Logger/Query>

#include <QtCore/QDate>

namespace Logger
{

class ConversationDatesQuery : public Query
{
Q_OBJECT

public:
    explicit ConversationDatesQuery(const QString &dbusid);

public Q_SLOTS:
    void perform(const QString &contact, bool ischatroom = false);

Q_SIGNALS:
    void completed(const QList<QDate> &dates);

private:
    static void callback(void *obj, void *result, ConversationDatesQuery *self);

    QList<QDate> dates;
};

} //namespace

#endif // __CONVERSATION_DATES_QUERY__