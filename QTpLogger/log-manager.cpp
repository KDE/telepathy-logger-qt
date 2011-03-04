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
#include "pending-dates-query.h"
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

#if 0
PendingDatesQuery *LogManager::queryDates(Tp::AccountPtr account, EntityPtr entity, EventTypeMask typeMask)
{
    return new PendingDatesQuery(LogManagerPtr(this), account, entity, typeMask);
}

PendingEventsQuery LogManager::queryEvents() const
{
    /*
    void tpl_log_manager_get_events_for_date_async (TplLogManager *manager,
        TpAccount *account,
        TplEntity *target,
        gint type_mask,
        const GDate *date,
        GAsyncReadyCallback callback,
        gpointer user_data);
    */
    return 0;
}

PendingEventsQuery LogManager::queryFilteredEvents() const
{
    /*
    void tpl_log_manager_get_filtered_events_async (TplLogManager *manager,
        TpAccount *account,
        TplEntity *target,
        gint type_mask,
        guint num_events,
        TplLogEventFilter filter,
        gpointer filter_user_data,
        GAsyncReadyCallback callback,
        gpointer user_data);
    */
    return 0;
}

PendingEntitiesQuery LogManager::queryEntities()
{
    /*
    void tpl_log_manager_get_entities_async (TplLogManager *self,
        TpAccount *account,
        GAsyncReadyCallback callback,
        gpointer user_data);
    */
    return 0;
}

PendingSearch LogManager::search()
{
    /*
    void tpl_log_manager_search_async (TplLogManager *manager,
        const gchar *text,
        gint type_mask,
        GAsyncReadyCallback callback,
        gpointer user_data);
    */
}
#endif
