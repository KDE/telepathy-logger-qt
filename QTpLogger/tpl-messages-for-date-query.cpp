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

#include "tpl-messages-for-date-query.h"

#include <telepathy-logger/log-manager.h>

#include "tpl-query-private.h"
#include "tpl-entry-private.h"

#include "tpl-query-callback-template.h"

using namespace QTpLogger;

MessagesForDateQuery::MessagesForDateQuery(const QString &dbusid) : Query(dbusid)
{
}

void MessagesForDateQuery::perform(const QString &chat, bool ischat,
                                   const QDate &date)
{
    if (!date.isValid()) return;

    // Setup a valid GDate (assuming the QDate is valid)...
    GDate gdate;

    g_date_clear(&gdate, 1);
    g_date_set_dmy(&gdate, (GDateDay)date.day(),
                   (GDateMonth)date.month(), (GDateYear)date.year());

    // Perform the call...
    tpl_log_manager_get_messages_for_date_async(this->d->logmanager(), this->d->account(),
            chat.toAscii(), ischat, &gdate, (GAsyncReadyCallback)this->callback, this);
}

void MessagesForDateQuery::callback(void *logmanager, void *result,
                                    MessagesForDateQuery *self)
{
    TPL_QUERY_FILL_DATA (logmanager, result, tpl_log_manager_get_messages_for_date_finish,
                         TplEntry, Entry, self->messages);

    // Notify
    Q_EMIT self->completed(self->messages);
}
