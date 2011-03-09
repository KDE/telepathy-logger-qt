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
#include <TelepathyQt4/AccountManager>
#include <TelepathyQt4Logger/Entity>
#include <TelepathyQt4Logger/LogManager>
#include <TelepathyQt4Logger/PendingDates>
#include <TelepathyQt4Logger/PendingEntities>
#include <TelepathyQt4Logger/PendingEvents>
#include <TelepathyQt4Logger/PendingSearch>
#include <TelepathyQt4Logger/utils.h>
#include <telepathy-logger/log-manager.h>
#include <telepathy-glib/account.h>

using namespace Tpl;

LogManagerPtr LogManager::instance()
{
    TplLogManager *manager = tpl_log_manager_dup_singleton();
    return LogManagerPtr::wrap(manager, false);
}

bool LogManager::exists(Tp::AccountPtr account, EntityPtr target, EventTypeMask type) const
{
    TpAccount *tpAccount = Utils::instance()->tpAccount(account);
    return tpl_log_manager_exists(object<TplLogManager>(), tpAccount, target, (gint) type);
}

PendingDates *LogManager::queryDates(Tp::AccountPtr account, EntityPtr entity,
        EventTypeMask typeMask)
{
    return new PendingDates(LogManagerPtr(this), account, entity, typeMask);
}

PendingEvents *LogManager::queryEvents(Tp::AccountPtr account, EntityPtr entity,
        EventTypeMask typeMask, QDate date)
{
    return new PendingEvents(LogManagerPtr(this), account, entity, typeMask, date);
}

PendingEvents *LogManager::queryFilteredEvents(Tp::AccountPtr account, EntityPtr entity,
        EventTypeMask typeMask, uint numEvents, LogEventFilter filterFunction, void *filterFunctionUserData)
{
    return new PendingEvents(LogManagerPtr(this), account, entity, typeMask, numEvents,
        filterFunction, filterFunctionUserData);
}

PendingEntities *LogManager::queryEntities(Tp::AccountPtr account)
{
    return new PendingEntities(LogManagerPtr(this), account);
}

PendingSearch *LogManager::search(const QString &text, EventTypeMask typeMask)
{
    return new PendingSearch(LogManagerPtr(this), text, typeMask);
}
