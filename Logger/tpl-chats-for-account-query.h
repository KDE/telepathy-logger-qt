#ifndef __CHATS_FOR_ACCOUNT_QUERY__
#define __CHATS_FOR_ACCOUNT_QUERY__

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
#include <Logger/Correspondant>

namespace Logger
{

class ChatsForAccountQuery : public Query
{

Q_OBJECT

public:
    ChatsForAccountQuery(const QString &dbusid) : Query(dbusid) {}

public slots:
    void perform();

signals:
    void completed(const QList<Correspondant> &chats);

private:
    static void callback(GObject *obj, GAsyncResult *result,
                         ChatsForAccountQuery *self);

    QList<Correspondant> chats;
};

} //namespace

#endif // __CHATS_FOR_ACCOUNT_QUERY__