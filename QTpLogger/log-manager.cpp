/*
 * Copyright (C) 2011 Stefano Sanfilippo <stefano.k.sanfilippo@gmail.com>
 * Copyright (C) 2011 Collabora Ltd. <http://www.collabora.co.uk/>
 *
 * @author Stefano Sanfilippo <stefano.k.sanfilippo@gmail.com>
 * @author Mateu Batle <mateu.batle@collabora.co.uk>
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

#include "log-manager.h"
#include "entity.h"
#include "pending-dates.h"
#include "pending-entities.h"
#include "pending-events.h"
#include "pending-search.h"
#include <telepathy-logger/log-manager.h>
#include <TelepathyQt4/Account>

using namespace QTpLogger;

LogManagerPtr LogManager::instance()
{
    TplLogManager *manager = tpl_log_manager_dup_singleton();
    return LogManagerPtr::wrap(manager, false);
}

bool LogManager::exists(Tp::AccountPtr account, EntityPtr target, EventTypeMask type) const
{
    // TOOD convert Tp::AccountPtr in TplAccount ?
    return tpl_log_manager_exists(object<TplLogManager>(), 0, target, (gint) type);
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
