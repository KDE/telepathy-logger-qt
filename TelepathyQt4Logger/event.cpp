/*
 * This file is part of TelepathyQt4Logger
 *
 * Copyright (C) 2011 Stefano Sanfilippo <stefano.k.sanfilippo@gmail.com>
 * Copyright (C) 2011 Collabora Ltd. <http://www.collabora.co.uk/>
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

#include <TelepathyQt4/Account>
#include <TelepathyQt4Logger/Event>
#include <TelepathyQt4Logger/utils.h>
#include <telepathy-logger/event.h>

using namespace Tpl;

QDateTime Event::timestamp() const
{
    gint64 t = tpl_event_get_timestamp(object<TplEvent>());
    QDateTime dateTime;
    dateTime.setTime_t(t);
    return dateTime;
}

QString Event::accountPath() const
{
    const gchar *s = tpl_event_get_account_path(object<TplEvent>());
    QString str = QString::fromUtf8(s);
    g_free((gpointer) s);
    return str;
}

Tp::AccountPtr Event::account() const
{
    TpAccount *account = tpl_event_get_account(object<TplEvent>());
    return Utils::instance()->accountPtr(account);
}

EntityPtr Event::sender() const
{
    TplEntity *entity = tpl_event_get_sender(object<TplEvent>());
    return EntityPtr::wrap(entity, false);
}

EntityPtr Event::receiver() const
{
    TplEntity *entity = tpl_event_get_receiver(object<TplEvent>());
    return EntityPtr::wrap(entity, false);
}

bool Event::operator==(Event& rhs) const
{
    return tpl_event_equal (object<TplEvent>(), rhs.object<TplEvent>());
}