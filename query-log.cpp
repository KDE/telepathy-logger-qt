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

#include <glib.h>
#include <telepathy-logger/entry-text.h>
#include <telepathy-logger/log-manager.h>

#include <Logger/Log>

#include <tpl-query-private.h>
#include <tpl-message-private.h>

using namespace Logger;

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
    tpl_log_manager_get_messages_for_date_async(this->d->logmanager, this->d->account,
            chat.toAscii(), ischat, &gdate, (GAsyncReadyCallback)this->callback, this);
}

void MessagesForDateQuery::callback(GObject *obj, GAsyncResult *result,
                                    MessagesForDateQuery *self)
{
    (void)obj;

    GList *gmessages, *i;
    GError *error = NULL;

    // Check whether everything went fine, and retrieves data...
    gboolean successful = tpl_log_manager_get_messages_for_date_finish(
                             self->d->logmanager, result, &gmessages, &error);

    if(error) {
        throw Error(error);
    }

    // This is placed as second, just as a failback. Otherwise it would prevent
    // more detailed exceptions from being thrown...
    if(!successful) {
        throw Error("tpl_log_manager_get_messages_for_date_async was not successfull!");
    }

    for (i = gmessages; i; i = i->next) {
        MessagePrivateData *pmessage = new MessagePrivateData((TplEntry*)i->data);
        self->messages << Message(pmessage);
    }

    // Free search results...
    tpl_log_manager_search_free(gmessages);

    emit self->completed(self->messages);
}



void KeywordQuery::perform(const QString &keyword)
{
    // Perform the call...
    tpl_log_manager_search_async(this->d->logmanager, keyword.toAscii(),
                                 (GAsyncReadyCallback)this->callback, this);
}

void KeywordQuery::callback(GObject *obj, GAsyncResult *result,
                            KeywordQuery *self)
{
    (void)obj;

    GList *ghits, *i;
    GError *error = NULL;

    // Check whether everything went fine, and retrieves data...
    gboolean successful =
		tpl_log_manager_search_finish(self->d->logmanager, result, &ghits, &error);

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
        ghit = (TplLogSearchHit*)i->data;
        self->hits << Hit(ghit);
    }

    // Free search results...
    tpl_log_manager_search_free(ghits);

    emit self->completed(self->hits);
}

void ChatsForAccountQuery::perform()
{
    // Perform the call...
    tpl_log_manager_get_chats_async(this->d->logmanager, this->d->account,
                                    (GAsyncReadyCallback)this->callback, this);
}

void ChatsForAccountQuery::callback(GObject *obj, GAsyncResult *result,
                                    ChatsForAccountQuery *self)
{
    (void)obj;

    GList *gchats, *i;
    GError *error = NULL;

    // Check whether everything went fine, and retrieves data...
    gboolean successful =
		tpl_log_manager_get_chats_finish(self->d->logmanager, result, &gchats, &error);

    if(error) {
        throw Error(error);
    }

    // This is placed as second, just as a failback. Otherwise it would prevent
    // more detailed exceptions to be thrown...
    if(!successful) {
        throw Error("tpl_log_manager_get_chats_async was not successfull!");
    }

    TplEntity *gchat;

    for(i = gchats; i; i = i->next) {
        gchat = (TplEntity*)i->data;
        self->chats << Correspondant(); // FIXME gchat);
    }

    // Free search results...
    tpl_log_manager_search_free(gchats);

    emit self->completed(self->chats);
}

#if 0

For later implementation of custom filters...

gboolean tpl_log_manager_get_filtered_messages_finish
(TplLogManager *self,
 GAsyncResult *result,
 GList **messages,
 GError **error);

void tpl_log_manager_get_filtered_messages_async
(TplLogManager *manager,
 TpAccount *account,
 const gchar *chat_id,
 gboolean is_chatroom,
 guint num_messages,
 TplLogMessageFilter filter,
 gpointer filter_user_data,
 GAsyncReadyCallback callback,
 gpointer user_data);

gboolean(*TplLogMessageFilter)(TplEntry *message, gpointer user_data);

#endif
