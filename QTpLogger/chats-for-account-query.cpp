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

#include "chats-for-account-query.h"

#include <telepathy-logger/log-manager.h>

#include "query-private.h"
#include "entry-private.h"
#include "query-callback-template.h"

using namespace QTpLogger;

ChatsForAccountQuery::ChatsForAccountQuery(const QString &dbusid) : Query(dbusid)
{
}

void ChatsForAccountQuery::perform()
{
    // Perform the call...
    // TODO update to tp-logger 0.2
    /*
    tpl_log_manager_get_chats_async(this->d->logmanager(), this->d->account(),
                                    (GAsyncReadyCallback)this->callback, (gpointer) this);
                                    */
}

void ChatsForAccountQuery::callback(void *logmanager, void *result,
                                    ChatsForAccountQuery *self)
{
    // TODO update to tp-logger 0.2
    /*
    TPL_QUERY_FILL_DATA (logmanager, result, tpl_log_manager_get_chats_finish,
                         TplEntity, Entity, self->chats);
                         */

    // Notify
    // TODO update to tp-logger 0.2
    //Q_EMIT self->completed(self->chats);
}
