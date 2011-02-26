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

#include <query-log.h>

#include <glib.h>
#include <telepathy-glib/account.h>
#include <telepathy-logger/entry-text.h>

#include <QtCore/QDebug>
#include <QtCore/QString>
#include <QtCore/QStringList>

using namespace Logger;

Query::Query(const QString &dbusid, bool isquoted)
{
    QString quotedDbusID;

    // Escape string as if it were a valid C indentifier...
    if (!isquoted) {
        QStringList chunks = dbusid.split("/");
        gchar *escapedUserName = tp_escape_as_identifier(chunks[2].toAscii());

        quotedDbusID = QString("%1/%2/%3").arg(chunks[0]).arg(chunks[1]).
                         arg(escapedUserName);

        g_free(escapedUserName);
    }

    g_type_init();

    this->logmanager = tpl_log_manager_dup_singleton();

    GError *error = NULL;

    TpDBusDaemon *daemon = tp_dbus_daemon_dup(&error);

    if (error) throw Error(error);

    error = NULL;

    QString path = QString(TP_ACCOUNT_OBJECT_PATH_BASE"%1").arg(quotedDbusID);

    TpAccount *account = tp_account_new(daemon, path.toAscii(), &error);

    if (error) throw Error(error);

    if (!account) throw Error("Account returned by tp_account_new is NULL!");

    tp_account_prepare_async(account, NULL,
                             (GAsyncReadyCallback)this->setreadycb, this);

    // Get rid of the bus proxy...
    g_object_unref(daemon);

    this->account = account;
}

Query::~Query()
{
    g_object_unref(this->account);
}

void Query::setreadycb(GObject *obj, GAsyncResult *result, Query *self)
{
    GError *error = NULL;

    if (!tp_account_prepare_finish(self->account, result, &error)) {
        self->account = NULL;
        throw new Error(error);
    }

    if (!tp_account_is_valid(self->account)) {
        throw new Error("Selected account is not valid!");
    }
}


void ChatExistsQuery::perform(const QString &chatname, bool ischatroom)
{
    emit completed(tpl_log_manager_exists(this->logmanager, this->account,
                                          chatname.toAscii(), ischatroom));
}



void ConversationDatesQuery::perform(const QString &chatid, bool ischatroom)
{
    // Perform the asynchronous call...
    tpl_log_manager_get_dates_async(this->logmanager, this->account, chatid.toAscii(),
                                    ischatroom, (GAsyncReadyCallback)this->callback, this);
}

void ConversationDatesQuery::callback(GObject *obj, GAsyncResult *result,
                                      ConversationDatesQuery* self)
{
    (void)obj;

    GList *gdates, *i;
    GError *error = NULL;

    // Check whether everything went fine, and retrieves data...
    gboolean successful = tpl_log_manager_get_dates_finish(self->logmanager,
                                                           result, &gdates, &error);

    if (error) {
        throw new Error(error);
    }

    // This is placed as second, just as a failback. Otherwise it would prevent
    // more detailed exceptions to be thrown...
    if (!successful) {
        throw new Error("tpl_log_manager_get_dates_async was not successfull!");
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
    tpl_log_manager_get_messages_for_date_async(this->logmanager, this->account,
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
                              self->logmanager, result, &gmessages, &error);

    if(error) {
        throw new Error(error);
    }

    // This is placed as second, just as a failback. Otherwise it would prevent
    // more detailed exceptions to be thrown...
    if(!successful) {
        throw new Error("tpl_log_manager_get_messages_for_date_async was not successfull!");
    }

    TplEntry *gmessage;

    for(i = gmessages; i; i = i->next) {
        gmessage = (TplEntry*)i->data;
        self->messages << Message(gmessage);
    }

    // Free search results...
    tpl_log_manager_search_free(gmessages);

    emit self->completed(self->messages);
}



void KeywordQuery::perform(const QString &keyword)
{
    // Perform the call...
    tpl_log_manager_search_async(this->logmanager, keyword.toAscii(),
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
        tpl_log_manager_search_finish(self->logmanager, result, &ghits, &error);

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
    tpl_log_manager_get_chats_async(this->logmanager, this->account,
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
        tpl_log_manager_get_chats_finish(self->logmanager, result, &gchats, &error);

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
        self->chats << Correspondant(gchat);
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

Error::Error(GError *gerror, bool dontfree)
{
    this->_message = QString(gerror->message);
    this->_code = gerror->code;

    if(!dontfree) {
        g_error_free(gerror);
    }
}

Error::Error(const QString &message, int code) : _message(message), _code(code)
{
    // Silence is golden :)
}

Message::Message(TplEntryText *tpmessage)
{
    gchar *gaccountpath, *gchannel, *gchatid, *glogid;
    TplEntity *gsender, *greceiver;

    g_object_get(tpmessage,
                 "account", &this->_account, "account-path", &gaccountpath,
                 "channel-path", &gchannel, "chat-id", &gchatid,
                 "direction", &this->_direction, "log-id", &glogid,
                 "receiver", &greceiver, "sender", &gsender,
                 "timestamp", &this->_timestamp, NULL);

    this->_accountpath = QString(gaccountpath);
    this->_channel = QString(gchannel);
    this->_chatid = QString(gchatid);
    this->_logid = QString(glogid);
    this->_receiver = Correspondant(greceiver);
    this->_sender = Correspondant(gsender);
}

Correspondant::Correspondant(TplEntity *chat)
{
    gchar *galias, *gid, *gavatar;

    g_object_get(chat,
                 "alias", &galias, "identifier", &gid,
                 "avatar-token", &gavatar, "entity-type", &this->type, NULL);

    this->alias = QString(galias);
    this->id = QString(id);
    this->avatar = QString(gavatar);
}

void Debug::echo(bool yes)
{
    qDebug("Logger::Debug::echo(bool) was called");

    qDebug() << yes;
}

void Debug::echo(const QList<QDate> &dates)
{
    qDebug("Logger::Debug::echo(QList<QDate>) was called");

    qDebug() << dates;
}

void Debug::echo(const QList<Message> &messages)
{
    qDebug("Logger::Debug::echo(QList<Message>) was called");

    foreach(Message m, messages) {
        qDebug() << m.chatid();
    }
}

void Debug::echo(const QList<Correspondant> &buddies)
{
    qDebug("Logger::Debug::echo(QList<Correspondant>) was called");
}

QString Message::accountpath()
{
    return this->_accountpath;
}

QString Message::channel()
{
    return this->_channel;
}

QString Message::chatid()
{
    return this->_chatid;
}

QString Message::logid()
{
    return this->_logid;
}

Message::Direction Message::direction()
{
    return this->_direction;
}

Correspondant Message::sender()
{
    return this->_sender;
}

Correspondant Message::receiver()
{
    return this->_receiver;
}

TpAccount* Message::account()
{
    return this->_account;
}

long Message::timestamp()
{
    return this->_timestamp;
}
