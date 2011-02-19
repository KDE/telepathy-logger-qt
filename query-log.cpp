#include <query-log.h>

#include <QDebug>

using namespace Logger;

Query::Query(QString dbuspath)
{
	g_type_init();

	this->logmanager = tpl_log_manager_dup_singleton();

	GError *error = NULL;

	TpDBusDaemon *daemon = tp_dbus_daemon_dup(&error);
	
	if (error)
	{
		Error *e = new Error(error);
		g_error_free(error);
		throw e;
	}
	
	error = NULL;

	TpAccount *account = tp_account_new(daemon, dbuspath.toAscii(), &error);

	if (error)
	{
		Error *e = new Error(error);
		g_error_free(error);
		throw e;
	}
	
	qDebug(tp_account_get_nickname(account));
	qDebug(tp_account_get_protocol(account));
	qDebug(tp_account_get_icon_name(account));
	qDebug(tp_account_get_connection_manager(account));

	if (!tp_account_is_valid(account))
	{
		error = g_error_new(TPL_LOG_MANAGER_ERROR, 100,
							"Selected account is not valid!");
		
		Error *e = new Error(error);
		g_error_free(error);
		throw e;
	}

	this->account = account;
}



void ChatExistsQuery::perform(QString chatname, bool ischatroom)
{
	emit completed(tpl_log_manager_exists(this->logmanager, this->account,
										  chatname.toAscii(), ischatroom));
}



void ConversationDatesQuery::perform(QString chatid, bool ischat)
{
	// Perform the asynchronous call...
	tpl_log_manager_get_dates_async(this->logmanager, this->account, chatid.toAscii(),
		ischat, (GAsyncReadyCallback)this->callback, this);
}

void ConversationDatesQuery::callback(GObject *obj, GAsyncResult *result,
									  ConversationDatesQuery* self)
{
	(void)obj;

	GList *gdates, *i; GError *error = NULL;

	// Check whether everything went fine, and retrieves data...
	if (!tpl_log_manager_get_dates_finish(self->logmanager, result, &gdates, &error))
	{
	}
	
	if (error)
	{
		Error *e = new Error(error);
		g_error_free(error);
		throw e;
	}

	GDate *gdate;

	for (i = gdates; i; i = i->next)
	{
		gdate = (GDate*)i->data;
		self->dates << QDate(gdate->year, gdate->month, gdate->day);
	}
	
	// Free search results...
	tpl_log_manager_search_free(gdates);
	
	emit self->completed(self->dates);
}



void MessagesForDateQuery::perform(QString chat, bool ischat, QDate date)
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

	GList *gmessages, *i; GError *error = NULL;

	// Check whether everything went fine, and retrieves data...
	if (!tpl_log_manager_get_messages_for_date_finish(self->logmanager,
		result, &gmessages, &error))
	{
	}
	
	if (error)
	{
		Error *e = new Error(error);
		g_error_free(error);
		throw e;
	}


	TplEntry *gmessage;

	for (i = gmessages; i; i = i->next)
	{
		gmessage = (TplEntry*)i->data;
		self->messages << Message(gmessage);
	}
	
	// Free search results...
	tpl_log_manager_search_free(gmessages);
	
	emit self->completed(self->messages);
}

#if 0

gboolean            tpl_log_manager_get_filtered_messages_finish
                                                        (TplLogManager *self,
                                                         GAsyncResult *result,
                                                         GList **messages,
                                                         GError **error);
void                tpl_log_manager_get_filtered_messages_async
                                                        (TplLogManager *manager,
                                                         TpAccount *account,
                                                         const gchar *chat_id,
                                                         gboolean is_chatroom,
                                                         guint num_messages,
                                                         TplLogMessageFilter filter,
                                                         gpointer filter_user_data,
                                                         GAsyncReadyCallback callback,
                                                         gpointer user_data);







gboolean            tpl_log_manager_get_chats_finish    (TplLogManager *self,
                                                         GAsyncResult *result,
                                                         GList **chats,
                                                         GError **error);
void                tpl_log_manager_get_chats_async     (TplLogManager *self,
                                                         TpAccount *account,
                                                         GAsyncReadyCallback callback,
                                                         gpointer user_data);





gboolean            tpl_log_manager_search_finish       (TplLogManager *self,
                                                         GAsyncResult *result,
                                                         GList **chats,
                                                         GError **error);
void                tpl_log_manager_search_async        (TplLogManager *manager,
                                                         const gchar *text,
                                                         GAsyncReadyCallback callback,
                                                         gpointer user_data);


gboolean (*TplLogMessageFilter)(TplEntry *message, gpointer user_data);

#endif

Error::Error(GError *gerror)
{
	this->_message = QString(gerror->message);
	this->_code = gerror->code;
}
