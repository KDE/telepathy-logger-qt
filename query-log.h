#ifndef __QUERY_LOG__
#define __QUERY_LOG__

#include <glib.h>
#include <telepathy-glib/account.h>
#include <telepathy-logger/log-manager.h>

#include <QObject>
#include <QDate>
#include <QString>
#include <QDebug>

namespace Logger {

class Hit
{
public:
  Hit(TplLogSearchHit *hit) {}
};

class Message
{
public:
  Message(TplEntry *tpmessage)
  {
    (void)tpmessage; //TODO
  }
};

class Chat {}; //TODO

class Error
{
public:
  Error(GError *gerror, bool dontfree = false);
  Error(QString message, int code = 0);

  QString message() { return this->_message; }
  int code() { return this->_code; }

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
  void perform(QString contact, bool ischat = false);

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
  void perform(QString contact, bool ischat=false, QDate date=QDate::currentDate());

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
  void completed(QList<Chat> chats);

private:
  static void callback(GObject *obj, GAsyncResult *result,
					   ChatsForAccountQuery *self);

  QList<Chat> chats;
};

class QueryDebug : public QObject
{
Q_OBJECT

public:
  QueryDebug() : QObject() {}

public slots:
  void echo(bool yes) { qDebug() << yes; }
};

} // namespace

#endif // __QUERY_LOG__
