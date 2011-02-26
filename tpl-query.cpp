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

#include <Logger/tpl-query.h>
#include <Logger/tpl-error.h>

#include <QStringList>

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
