#ifndef __QUERY_PRIVATE__
#define __QUERY_PRIVATE__

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

#include <telepathy-logger/log-manager.h>

#include <QtCore/QString>

namespace Logger
{

class QueryPrivateData;

typedef void (*QueryCallback)(GObject *obj, GAsyncResult *result,
                              QueryPrivateData *self);

class QueryPrivateData
{
public:
    QueryPrivateData(const QString &dbusid);
    ~QueryPrivateData();

    static void setreadycb(GObject *obj, GAsyncResult *result,
                           QueryPrivateData *self);

    QueryCallback finishedcb;
    TplLogManager *logmanager;
    TpAccount *account;
};

} //namespace

#endif // __QUERY_PRIVATE__