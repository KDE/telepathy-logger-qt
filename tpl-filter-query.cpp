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

#include <Logger/tpl-keyword-query.h>

#include <tpl-query-private.h>
#include <tpl-message-private.h>
#include <tpl-error.h>

using namespace Logger;

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
