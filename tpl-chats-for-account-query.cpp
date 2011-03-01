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

#include <Logger/tpl-chats-for-account-query.h>

#include <tpl-query-private.h>
#include <tpl-message-private.h>
#include <tpl-correspondant-private.h>

#include <tpl-error.h>

using namespace Logger;

void ChatsForAccountQuery::perform()
{
    // Perform the call...
    tpl_log_manager_get_chats_async(this->d->logmanager(), this->d->account(),
                                    (GAsyncReadyCallback)this->callback, this);
}

void ChatsForAccountQuery::callback(void *_logmanager, void *_result,
                                    ChatsForAccountQuery *self)
{
    TplLogManager *logmanager = static_cast<TplLogManager*>(_logmanager);
    GAsyncResult *result = static_cast<GAsyncResult*>(_result);

    if (!TPL_IS_LOG_MANAGER (logmanager)) {
        throw Error("Query callback returned an invalid TplLogManager object.");
    }

    GList *gchats, *i;
    GError *error = NULL;

    // Check whether everything went fine, and retrieves data...
    gboolean successful =
        tpl_log_manager_get_chats_finish(self->d->logmanager(), result, &gchats, &error);

    if(error) {
        throw Error(error);
    }

    // This is placed as second, just as a failback. Otherwise it would prevent
    // more detailed exceptions to be thrown...
    if(!successful) {
        throw Error("tpl_log_manager_get_chats_async was not successfull!");
    }

    for(i = gchats; i; i = i->next) {
        CorrespondantPrivateData *d = new
          CorrespondantPrivateData((TplEntity*)i->data);

        self->chats << Correspondant(d);
    }

    // Free search results...
    tpl_log_manager_search_free(gchats);

    Q_EMIT self->completed(self->chats);
}
