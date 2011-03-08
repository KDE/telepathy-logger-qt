/*
 * Copyright (C) 2011 Stefano Sanfilippo <stefano.k.sanfilippo@gmail.com>
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

#include "query-private.h"

#include <telepathy-glib/account.h>
#include <QGlib/Error>

using namespace Tpl;

TplLogManager *QueryPrivate::_logmanager = tpl_log_manager_dup_singleton();

QueryPrivate::QueryPrivate(const QString &quotedDbusID)
{
    GError *error = NULL;

    TpDBusDaemon *daemon = tp_dbus_daemon_dup(&error);

    if (error) throw QGlib::Error(error);

    error = NULL;

    QString path = QString(TP_ACCOUNT_OBJECT_PATH_BASE"%1").arg(quotedDbusID);

    TpAccount *account = tp_account_new(daemon, path.toAscii(), &error);

    if (error) throw QGlib::Error(error);

    if (!account) {
        throw QGlib::Error(TPL_LOG_MANAGER_ERROR, 0, "Account returned by "
                                                     "tp_account_new is NULL!");
    }

    tp_account_prepare_async(account, NULL, //NULL, NULL);
                             (GAsyncReadyCallback)this->setreadycb, this);

    // Get rid of the bus proxy...
    g_object_unref(daemon);

    this->_account = account;
}

QueryPrivate::~QueryPrivate()
{
    g_object_unref(this->_account);
}

void QueryPrivate::setreadycb(GObject *obj, GAsyncResult *result, QueryPrivate *self)
{
    (void)obj;

    GError *error = NULL;

    if (!tp_account_prepare_finish(self->_account, result, &error)) {
        throw QGlib::Error(error);
    }

    if (!tp_account_is_valid(self->_account)) {
        throw QGlib::Error(TPL_LOG_MANAGER_ERROR, 0, "Selected account "
                                                     "is not valid!");
    }
}

TplLogManager* QueryPrivate::logmanager() const
{
    return this->_logmanager;
}

TpAccount* QueryPrivate::account() const
{
    return this->_account;
}
