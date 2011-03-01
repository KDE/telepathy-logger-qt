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

#include <Logger/tpl-keyword-query.h>

#include <telepathy-logger/log-manager.h>

#include <tpl-query-private.h>
#include <tpl-message-private.h>
#include <tpl-error.h>

using namespace Logger;

KeywordQuery::KeywordQuery(const QString &dbusid) : Query(dbusid)
{
}

void KeywordQuery::perform(const QString &keyword)
{
    // Perform the call...
    tpl_log_manager_search_async(this->d->logmanager(), keyword.toAscii(),
                                 (GAsyncReadyCallback)this->callback, this);
}

void KeywordQuery::callback(void *_logmanager, void*_result, KeywordQuery *self)
{
    // Cast parameters...
    TplLogManager *logmanager = static_cast<TplLogManager*>(_logmanager);
    GAsyncResult *result = static_cast<GAsyncResult*>(_result);

    if (!TPL_IS_LOG_MANAGER (logmanager)) {
        throw Error("KeywordQuery callback returned an invalid TplLogManager object.");
    }

    GList *ghits, *i;
    GError *error = NULL;

    // Check whether everything went fine, and retrieves data...
    gboolean successful =
        tpl_log_manager_search_finish(logmanager, result, &ghits, &error);

    if(error) {
        throw Error(error);
    }

    // This is placed as second, just as a failback. Otherwise it would prevent
    // more detailed exceptions to be thrown...
    if(!successful) {
        throw Error("tpl_log_manager_search_async was not successfull!");
    }

    TplLogSearchHit *ghit;

    for(i = ghits; i; i = i->next) {
        ghit = (TplLogSearchHit*)i->data; // FIXME
        self->hits << Hit(ghit);
    }

    // Free search results...
    tpl_log_manager_search_free(ghits);

    Q_EMIT self->completed(self->hits);
}
