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

#include "tpl-filter-query.h"

#include <telepathy-logger/entry-text.h>

#include "tpl-query-private.h"
#include "tpl-entry-private.h"
#include "tpl-query-callback-template.h"

using namespace QTpLogger;

FilterQuery::FilterQuery(const QString &dbusid) :
    Query(dbusid)
{
    qDebug("FilterQuery class not implemented yet!");
}

void FilterQuery::perform(const QString &chatid, bool ischatroom)
{
#if 0
    // Perform the asynchronous call...
    tpl_log_manager_get_filtered_messages_async(
        this->d->logmanager(), this->d->account(), chatid.toAscii(), ischatroom,
        // guint num_messages, TplLogMessageFilter filter, gpointer filter_user_data,
        (GAsyncReadyCallback)this->callback, this);

    // typedef gboolean(*TplLogMessageFilter)(TplEntry *message, gpointer user_data);
#endif
}

// FIXME check if data is really TplEntry!!

void FilterQuery::callback(void *logmanager, void *result, FilterQuery* self)
{
    TPL_QUERY_FILL_DATA (logmanager, result,
                         tpl_log_manager_get_filtered_messages_finish,
                         TplEntry, EntryPrivate, Entry,
                         self->messages);

    // Notify
    Q_EMIT self->completed(self->messages);
}
