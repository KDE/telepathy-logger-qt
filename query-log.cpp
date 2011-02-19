#include <query-log.h>

#include <QStringList>

using namespace Logger;

Query::Query(QString dbusid, bool isquoted)
{
  // Escape string as if it were a valid C indentifier...
  if (!isquoted)
  {
	QStringList chunks = dbusid.split("/");

	dbusid = QString("%1/%2/%3").arg(chunks[0]).arg(chunks[1]).
				arg(tp_escape_as_identifier(chunks[2].toAscii()));
  }

  g_type_init();

  this->logmanager = tpl_log_manager_dup_singleton();

  GError *error = NULL;

  TpDBusDaemon *daemon = tp_dbus_daemon_dup(&error);

  if (error)
  {
	throw new Error(error);
  }

  error = NULL;

  QString path = QString(TP_ACCOUNT_OBJECT_PATH_BASE"%1").arg(dbusid);

  TpAccount *account = tp_account_new(daemon, path.toAscii(), &error);

  if (error)
  {
	throw new Error(error);
  }

  if (!account)
  {
	throw new Error("Account returned by tp_account_new was NULL!");
  }

  tp_account_prepare_async(account, NULL,
				(GAsyncReadyCallback)this->setreadycb, this);

  this->account = account;
}

void Query::setreadycb(GObject *obj, GAsyncResult *result, Query *self)
{
  GError *error = NULL;

  if (!tp_account_prepare_finish (self->account, result, &error))
  {
	self->account = NULL;
	throw new Error(error);
  }

  if (!tp_account_is_valid(self->account))
  {
	throw new Error("Selected account is not valid!");
  }
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
  gboolean successful = tpl_log_manager_get_dates_finish(self->logmanager, result, &gdates, &error);

  if (error)
  {
	throw new Error(error);
  }

  // This is placed as second, just as a failback. Otherwise it would prevent
  // more detailed exceptions to be thrown...
  if (!successful)
  {
	throw new Error("tpl_log_manager_get_dates_async was not successfull!");
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
  gboolean successful = tpl_log_manager_get_messages_for_date_finish(
	self->logmanager, result, &gmessages, &error);

  if (error)
  {
	throw new Error(error);
  }

  // This is placed as second, just as a failback. Otherwise it would prevent
  // more detailed exceptions to be thrown...
  if (!successful)
  {
	throw new Error("tpl_log_manager_get_messages_for_date_async was not successfull!");
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



void KeywordQuery::perform(QString keyword)
{
  // Perform the call...
  tpl_log_manager_search_async(this->logmanager, keyword.toAscii(),
	(GAsyncReadyCallback)this->callback, this);
}

void KeywordQuery::callback(GObject *obj, GAsyncResult *result,
									KeywordQuery *self)
{
  (void)obj;

  GList *ghits, *i; GError *error = NULL;

  // Check whether everything went fine, and retrieves data...
  gboolean successful =
	tpl_log_manager_search_finish(self->logmanager, result, &ghits, &error);

  if (error)
  {
	throw new Error(error);
  }

  // This is placed as second, just as a failback. Otherwise it would prevent
  // more detailed exceptions to be thrown...
  if (!successful)
  {
	throw new Error("tpl_log_manager_search_async was not successfull!");
  }

  TplLogSearchHit *ghit;

  for (i = ghits; i; i = i->next)
  {
	ghit= (TplLogSearchHit*)i->data;
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

  GList *ghits, *i; GError *error = NULL;

  // Check whether everything went fine, and retrieves data...
  gboolean successful =
	tpl_log_manager_get_chats_finish(self->logmanager, result, &ghits, &error);

  if (error)
  {
	throw new Error(error);
  }

  // This is placed as second, just as a failback. Otherwise it would prevent
  // more detailed exceptions to be thrown...
  if (!successful)
  {
	throw new Error("tpl_log_manager_get_chats_async was not successfull!");
  }

//   TplLogSearchHit *ghit;
// 
//   for (i = ghits; i; i = i->next)
//   {
// 	ghit= (TplLogSearchHit*)i->data;
// 	self->hits << Hit(ghit);
//   } FIXME

  // Free search results...
  tpl_log_manager_search_free(ghits);

  emit self->completed(self->chats);
}



Error::Error(GError *gerror, bool dontfree)
{
  this->_message = QString(gerror->message);
  this->_code = gerror->code;

  if (!dontfree)
  {
	g_error_free(gerror);
  }
}

Error::Error(QString message, int code) : _message(message), _code(code)
{
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

gboolean (*TplLogMessageFilter)(TplEntry *message, gpointer user_data);

#endif