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

#include <Logger/tpl-conversation-dates-query.h>
#include <Logger/tpl-error.h>
#include <tpl-query-private.h>

using namespace Logger;

void ConversationDatesQueryCallback(GObject *obj, GAsyncResult *result,
                                      ConversationDatesQuery* self);

ConversationDatesQuery::ConversationDatesQuery(const QString &dbusid) :
    Query(dbusid)
{
    //this->d->finishedcb = &ConversationDatesQueryCallback;
}

void ConversationDatesQuery::perform(const QString &chatid, bool ischatroom)
{
    // Perform the asynchronous call...
    tpl_log_manager_get_dates_async(this->d->logmanager, this->d->account, chatid.toAscii(),
                                    ischatroom, (GAsyncReadyCallback)this->callback, this);
}

//XXX replace with callback template
void ConversationDatesQuery::callback(GObject *obj, GAsyncResult *result,
                                    ConversationDatesQuery* self)
{
    (void)obj;

    GList *gdates, *i;
    GError *error = NULL;

    // Check whether everything went fine, and retrieves data...
    gboolean successful = tpl_log_manager_get_dates_finish(self->d->logmanager,
                                                           result, &gdates, &error);

    if (error) {
        throw Error(error);
    }

    // This is placed as second, just as a failback. Otherwise it would prevent
    // more detailed exceptions to be thrown...
    if (!successful) {
        throw Error("tpl_log_manager_get_dates_async was not successfull!");
    }

    GDate *gdate;

    for (i = gdates; i; i = i->next) {
        gdate = (GDate*)i->data;
        self->dates << QDate(gdate->year, gdate->month, gdate->day);
    }

    // Free search results...
    tpl_log_manager_search_free(gdates);

    emit self->completed(self->dates);
}
