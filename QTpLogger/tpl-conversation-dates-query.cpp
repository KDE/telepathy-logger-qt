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

#include "tpl-conversation-dates-query.h"

#include <telepathy-logger/log-manager.h>

#include "tpl-query-private.h"
#include "tpl-query-callback-template.h"

using namespace QTpLogger;

// Bridge class between QDate and GDate
class QGDate : public QDate
{
public:
    QGDate(GDate* date) : QDate(date->year, date->month, date->day) {}
};

ConversationDatesQuery::ConversationDatesQuery(const QString &dbusid) :
    Query(dbusid)
{
}

void ConversationDatesQuery::perform(const QString &chatid, bool ischatroom)
{
    // Perform the asynchronous call...
    tpl_log_manager_get_dates_async(this->d->logmanager(), this->d->account(),
                                    chatid.toAscii(), ischatroom,
                                    (GAsyncReadyCallback)this->callback, this);
}

void ConversationDatesQuery::callback(void *logmanager, void *result,
                                      ConversationDatesQuery* self)
{
    // This is different: QDate is a Qt type :)
    TPL_QUERY_FILL_DATA_QT_NATIVE (logmanager, result,
                                   tpl_log_manager_get_dates_finish,
                                   GDate, QGDate, self->dates);
    // Notify
    Q_EMIT self->completed(self->dates);
}
