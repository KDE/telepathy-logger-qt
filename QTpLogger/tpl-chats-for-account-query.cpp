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

#include "tpl-chats-for-account-query.h"

#include <telepathy-logger/log-manager.h>

#include "tpl-query-private.h"
#include "tpl-entry-private.h"
#include "tpl-entity-private.h"
#include "tpl-query-callback-template.h"

using namespace QTpLogger;

ChatsForAccountQuery::ChatsForAccountQuery(const QString &dbusid) : Query(dbusid)
{
}

void ChatsForAccountQuery::perform()
{
    // Perform the call...
    tpl_log_manager_get_chats_async(this->d->logmanager(), this->d->account(),
                                    (GAsyncReadyCallback)this->callback, this);
}

void ChatsForAccountQuery::callback(void *logmanager, void *result,
                                    ChatsForAccountQuery *self)
{
    TPL_QUERY_FILL_DATA (logmanager, result, tpl_log_manager_get_chats_finish,
                         TplEntity, EntityPrivate, Entity,
                         self->chats);

    // Notify
    Q_EMIT self->completed(self->chats);
}
