/*
 * This file is part of TelepathyLoggerQt
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

#include <TelepathyLoggerQt/LogManager>

#include <TelepathyQt/Account>
#include <TelepathyQt/AccountManager>
#include <TelepathyLoggerQt/Entity>
#include <TelepathyLoggerQt/LogWalker>
#include <TelepathyLoggerQt/PendingDates>
#include <TelepathyLoggerQt/PendingEntities>
#include <TelepathyLoggerQt/PendingEvents>
#include <TelepathyLoggerQt/PendingSearch>
#include <TelepathyLoggerQt/pending-clear.h>
#include <TelepathyLoggerQt/utils.h>

#include <telepathy-logger/log-manager.h>
#include <telepathy-glib/account.h>

#include <_gen/cli-logger-body.hpp>
#include <_gen/cli-logger.moc.hpp>

using namespace Tpl;

LogManagerPtr LogManager::instance()
{
    TplLogManager *manager = tpl_log_manager_dup_singleton();
    return LogManagerPtr::wrap(manager, false);
}

Tp::AccountManagerPtr LogManager::accountManagerPtr() const
{
    return Utils::instance()->accountManagerPtr();
}

void LogManager::setAccountManagerPtr(const Tp::AccountManagerPtr & accountManager)
{
    Utils::instance()->setAccountManagerPtr(accountManager);
}

bool LogManager::exists(const Tp::AccountPtr & account, const EntityPtr & target,
        EventTypeMask type) const
{
    TpAccount *tpAccount = Utils::instance()->tpAccount(account);
    return tpl_log_manager_exists(object<TplLogManager>(), tpAccount, target, (gint) type);
}

PendingDates *LogManager::queryDates(const Tp::AccountPtr & account, const EntityPtr & entity,
        EventTypeMask typeMask)
{
    return new PendingDates(LogManagerPtr(this), account, entity, typeMask);
}

PendingEvents *LogManager::queryEvents(const Tp::AccountPtr & account, const EntityPtr & entity,
        EventTypeMask typeMask, const QDate & date)
{
    return new PendingEvents(LogManagerPtr(this), account, entity, typeMask, date);
}

PendingEvents *LogManager::queryFilteredEvents(const Tp::AccountPtr & account,
        const EntityPtr & entity, EventTypeMask typeMask, uint numEvents,
        LogEventFilter filterFunction, void *filterFunctionUserData)
{
    return new PendingEvents(LogManagerPtr(this), account, entity, typeMask, numEvents,
        filterFunction, filterFunctionUserData);
}

PendingEntities *LogManager::queryEntities(const Tp::AccountPtr & account)
{
    return new PendingEntities(LogManagerPtr(this), account);
}

PendingSearch *LogManager::search(const QString &text, EventTypeMask typeMask)
{
    return new PendingSearch(LogManagerPtr(this), text, typeMask);
}

PendingOperation *LogManager::clearHistory()
{
    PendingClear *operation = new PendingClear();

    operation->clearLog();

    return operation;
}

PendingOperation *LogManager::clearAccountHistory(const Tp::AccountPtr &account)
{
    PendingClear *operation = new PendingClear();

    operation->clearAccount(account);

    return operation;
}

PendingOperation *LogManager::clearEntityHistory(const Tp::AccountPtr &account, const EntityPtr &entity)
{
    PendingClear *operation = new PendingClear();

    if (entity->entityType() == EntityTypeContact) {
        operation->clearContact(account, entity->identifier());
    } else if (entity->entityType() == EntityTypeRoom) {
        operation->clearRoom(account, entity->identifier());
    } else {
        return 0;
    }

    return operation;
}

LogWalkerPtr LogManager::queryWalkFilteredEvents(const Tp::AccountPtr& account,
        const EntityPtr& entity, EventTypeMask typeMask, LogEventFilter filterFunction,
        void* filterFunctionUserData)
{
    TpAccount *tpAccount = Utils::instance()->tpAccount(account);
    if (!tpAccount) {
        qWarning() << "Invalid account";
        return LogWalkerPtr();
    }

    TplLogWalker *tpWalker = tpl_log_manager_walk_filtered_events(
                                LogManagerPtr(this), tpAccount, entity,
                                (gint) typeMask, (TplLogEventFilter) filterFunction,
                                filterFunctionUserData);
    return LogWalkerPtr::wrap(tpWalker, false);
}

#if TPLQT_TPL_IS_VERSION(0, 9, 0)
bool LogManager::disableForEntity(const Tp::AccountPtr &account, const EntityPtr &entity)
{
    TpAccount *tpAccount = Utils::instance()->tpAccount(account);
    if (!tpAccount) {
        qWarning() << "Invalid account";
        return false;
    }
    tpl_log_manager_disable_for_entity(LogManagerPtr(this), tpAccount, entity);

    return true;
}

bool LogManager::enableForEntity(const Tp::AccountPtr &account, const EntityPtr &entity)
{
    TpAccount *tpAccount = Utils::instance()->tpAccount(account);
    if (!tpAccount) {
        qWarning() << "Invalid account";
        return false;
    }
    tpl_log_manager_enable_for_entity(LogManagerPtr(this), tpAccount, entity);

    return true;
}

bool LogManager::isDisabledForEntity(const Tp::AccountPtr &account, const EntityPtr &entity)
{
    TpAccount *tpAccount = Utils::instance()->tpAccount(account);
    if (!tpAccount) {
        qWarning() << "Invalid account";
        return false;
    }

    return tpl_log_manager_is_disabled_for_entity(LogManagerPtr(this), tpAccount, entity);
}
#endif
