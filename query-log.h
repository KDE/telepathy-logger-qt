#ifndef __QUERY_LOG__
#define __QUERY_LOG__

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

#include <QObject>
#include <QDate>

class QString;

namespace Logger
{

class Hit
{
public:
    Hit(TplLogSearchHit *hit) {} //FIXME unknown type
};

class TplEntryText;

class Correspondant
{
public:
    Correspondant(TplEntity *chat);
    Correspondant() {};

    enum Who {
        undefined = TPL_ENTITY_UNKNOWN,
        contact = TPL_ENTITY_CONTACT,
        group = TPL_ENTITY_GROUP,
        self = TPL_ENTITY_SELF
    };

private:
    QString alias, id, avatar;

    Who type;
};

class Message
{
public:
    Message(TplEntryText *tpmessage);
    Message(TplEntry *tpmessage) {};
    Message() {};

//   "message"                  gchar*
//   "message-type"             guint
//   "pending-msg-id"           gint

    enum Direction {
        undefined = 0,
        incoming  = TPL_ENTRY_DIRECTION_IN,
        outcoming = TPL_ENTRY_DIRECTION_OUT
    };

    QString accountpath() {
        return this->_accountpath;
    }
    QString channel() {
        return this->_channel;
    }
    QString chatid() {
        return this->_chatid;
    }
    QString logid() {
        return this->_logid;
    }
    Direction direction() {
        return this->_direction;
    }
    Correspondant sender() {
        return this->_sender;
    }
    Correspondant receiver() {
        return this->_receiver;
    }
    TpAccount* account() {
        return this->_account;
    }
    long timestamp() {
        return this->_timestamp;
    }

private:
    TpAccount* _account;
    long _timestamp;

    QString _accountpath, _channel, _chatid, _logid;
    Direction _direction;
    Correspondant _sender, _receiver;
};

class Error
{
public:
    Error(GError *gerror, bool dontfree = false);
    Error(QString message, int code = 0);

    QString message() {
        return this->_message;
    }
    int code() {
        return this->_code;
    }

private:
    QString _message;
    int _code;
};

class Query : public QObject
{
public:
    explicit Query(QString dbusid, bool isquoted = false);
    ~Query() {}

protected:
    static void callback(GObject *obj, GAsyncResult *result, Query *self) {}

    TplLogManager *logmanager;
    TpAccount *account;

private:
    static void setreadycb(GObject *obj, GAsyncResult *result, Query *self);
};

class ChatExistsQuery : public Query
{
    Q_OBJECT

public:
    explicit ChatExistsQuery(QString dbusid) : Query(dbusid) {}

public slots:
    void perform(QString contact, bool ischat = false);

signals:
    void completed(bool yes);
};

class ConversationDatesQuery : public Query
{
    Q_OBJECT

public:
    explicit ConversationDatesQuery(QString dbusid) : Query(dbusid) {}

public slots:
    void perform(QString contact, bool ischatroom = false);

signals:
    void completed(QList<QDate> dates);

private:
    static void callback(GObject *obj, GAsyncResult *result,
                         ConversationDatesQuery* self);

    QList<QDate> dates;
};

class MessagesForDateQuery : public Query
{
    Q_OBJECT

public:
    explicit MessagesForDateQuery(QString dbusid) : Query(dbusid) {}

public slots:
    void perform(QString contact, bool ischat = false, QDate date = QDate::currentDate());

signals:
    void completed(QList<Message> messages);

private:
    static void callback(GObject *obj, GAsyncResult *result,
                         MessagesForDateQuery* self);

    QList<Message> messages;
};

class KeywordQuery : public Query
{
    Q_OBJECT

public:
    KeywordQuery(QString dbusid);

public slots:
    void perform(QString keyword);

signals:
    void completed(QList<Hit> hits);

private:
    static void callback(GObject *obj, GAsyncResult *result, KeywordQuery *self);

    QList<Hit> hits;
};

class ChatsForAccountQuery : public Query
{
    Q_OBJECT

public:
    ChatsForAccountQuery(QString dbusid) : Query(dbusid) {}

public slots:
    void perform();

signals:
    void completed(QList<Correspondant> chats);

private:
    static void callback(GObject *obj, GAsyncResult *result,
                         ChatsForAccountQuery *self);

    QList<Correspondant> chats;
};

class Debug : public QObject
{
    Q_OBJECT

public:
    Debug() : QObject() {}

public slots:
    void echo(bool yes);
    void echo(QList<QDate> dates);
    void echo(QList<Message> messages);
    void echo(QList<Correspondant> buddies);
};

} // namespace

#endif // __QUERY_LOG__
