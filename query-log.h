#ifndef __QUERY_LOG__
#define __QUERY_LOG__

#include <glib.h>
#include <telepathy-glib/account.h>
#include <telepathy-logger/log-manager.h>

#include <QObject>
#include <QDate>

namespace Logger {

class Message
{
public:
	Message(TplEntry *tpmessage)
	{
		(void)tpmessage; //TODO
	}
};

class Error
{
public:
	Error(GError *gerror);
	
	QString message() { return this->_message; }
	int code() { return this->_code; }

private:
	QString _message;
	int _code;
};

class Query : public QObject
{
public:
	explicit Query(QString dbuspath);
	~Query() {}
	
protected:
	static void callback(GObject *obj, GAsyncResult *result, gpointer self)
	{
		(void)obj; (void)result; (void)self;
	}
	
	TplLogManager *logmanager;
	TpAccount *account;
};

class ChatExistsQuery : public Query
{
Q_OBJECT

public:
	explicit ChatExistsQuery(QString dbuspath) : Query(dbuspath) {}

public slots:
	void perform(QString contact, bool ischat = false);

signals:
	void completed(bool yes);
};

class ConversationDatesQuery : public Query
{
Q_OBJECT

public:
	explicit ConversationDatesQuery(QString dbuspath) : Query(dbuspath) {}

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
	explicit MessagesForDateQuery(QString dbuspath) : Query(dbuspath) {}
	
public slots:
	void perform(QString contact, bool ischat=false, QDate date=QDate::currentDate());

signals:
	void completed(QList<Message> messages);

private:
	static void callback(GObject *obj, GAsyncResult *result,
						 MessagesForDateQuery* self);
	
	QList<Message> messages;
};

} // namespace

#endif // __QUERY_LOG__
