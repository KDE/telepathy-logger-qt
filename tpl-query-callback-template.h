#ifndef __QUERY_CALLBACK_TEMPLATE__
#define __QUERY_CALLBACK_TEMPLATE__

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

#include <Logger/tpl-error.h>

#include <QtCore/QList>

// TODO to be used like this:
//
//     queryCompletedCallback<TplEntry, Message, MessagePrivateData,
//       tpl_log_manager_get_messages_for_date_finish>
//          ((GObject*)logmanager, (GAsyncResult*)result, self->messages);

typedef gboolean (*RetrieveResultsFunction)(TplLogManager*, GAsyncResult*,
                                            GList **, GError**);

using namespace Logger;

template <typename GTypeForQueryResults,
          class QTypeForQueryResults,
          class QPrivateDataType,
          RetrieveResultsFunction hasfinished>

void queryCompletedCallback(GObject *logmanager, GAsyncResult *result, QList<QTypeForQueryResults> &results)
{
    GList *gresults;
    GList *i;
    GError *error = NULL;

    if (!TPL_IS_LOG_MANAGER (logmanager)) {
        throw Error("Query callback returned an invalid TplLogManager object.");
    }

    // Check whether everything went fine, and retrieves data...
    gboolean success = hasfinished((TplLogManager*)logmanager, result,
                                    &gresults, &error);

    if (error) {
        throw Error(error);
    }

    // This is placed as second, just as a failback. Otherwise it would prevent
    // more detailed exceptions from being thrown...
    if (!success) {
        throw Error("An async call to TpLogger failed with no specified error!");
    }

    // Iterate over the list, cast GObjects and initialises correspective QObject
    for (i = gresults; i; i = i->next) {
        QPrivateDataType *d = new QPrivateDataType((GTypeForQueryResults*)i->data);
        results << QTypeForQueryResults(d);
    }

    // Free search results...
    tpl_log_manager_search_free(gresults);
}

#endif // __QUERY_CALLBACK_TEMPLATE__