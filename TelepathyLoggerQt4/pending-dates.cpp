/*
 * This file is part of TelepathyLoggerQt4
 *
 * Copyright (C) 2011 Collabora Ltd. <http://www.collabora.co.uk/>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <TelepathyLoggerQt4/PendingDates>

#include <QtCore/QDebug>
#include <TelepathyQt/Account>
#include <TelepathyLoggerQt4/Entity>
#include <TelepathyLoggerQt4/LogManager>
#include <TelepathyLoggerQt4/utils.h>
#include <TelepathyLoggerQt4/_gen/pending-dates.moc.hpp>
#include <glib/gerror.h>
#include <glib/gdate.h>
#include <telepathy-logger/log-manager.h>

using namespace Tpl;

struct TELEPATHY_LOGGER_QT4_NO_EXPORT PendingDates::Private
{
    LogManagerPtr manager;
    Tp::AccountPtr account;
    TpAccount *tpAccount;
    EntityPtr entity;
    EventTypeMask typeMask;
    QDateList dates;

    static void onAccountPrepared(void *logManager, void *result, PendingDates *self);
    static void callback(void *logManager, void *result, PendingDates *self);
};

PendingDates::PendingDates(const LogManagerPtr & manager, const Tp::AccountPtr & account,
    const EntityPtr & entity, EventTypeMask typeMask)
    : PendingOperation(),
      mPriv(new Private())
{
    mPriv->manager = manager;
    mPriv->account = account;
    mPriv->tpAccount = 0;
    mPriv->entity = entity;
    mPriv->typeMask = typeMask;
}

PendingDates::~PendingDates()
{
    delete mPriv;
}

void PendingDates::start()
{
    mPriv->tpAccount = Utils::instance()->tpAccount(mPriv->account);
    if (!mPriv->tpAccount) {
        setFinishedWithError(TP_QT_ERROR_INVALID_ARGUMENT, "Invalid account");
        return;
    }

    GQuark features[] = { TP_ACCOUNT_FEATURE_CORE, 0 };
    tp_account_prepare_async(mPriv->tpAccount, features, (GAsyncReadyCallback) Private::onAccountPrepared, this);
}

void PendingDates::Private::onAccountPrepared(void *logManager, void *result, PendingDates *self)
{
    tpl_log_manager_get_dates_async(self->mPriv->manager,
        self->mPriv->tpAccount,
        self->mPriv->entity,
        self->mPriv->typeMask,
        (GAsyncReadyCallback) Private::callback,
        self);
}

QDateList PendingDates::dates() const
{
    if (!isFinished()) {
        qWarning() << "PendingDates::dates called before finished, returning empty";
        return QDateList();
    } else if (!isValid()) {
        qWarning() << "PendingDates::dates called when not valid, returning empty";
        return QDateList();
    }

    return mPriv->dates;
}

const Tp::AccountPtr PendingDates::account() const
{
    if (!isFinished()) {
        qWarning() << "PendingDates::account called before finished";
    } else if (!isValid()) {
        qWarning() << "PendingDates::account called when not valid";
    }

    return mPriv->account;
}

const EntityPtr PendingDates::entity() const
{
    if (!isFinished()) {
        qWarning() << "PendingDates::entity called before finished";
    } else if (!isValid()) {
        qWarning() << "PendingDates::entity called when not valid";
    }

    return mPriv->entity;
}


void PendingDates::Private::callback(void *logManager, void *result, PendingDates *self)
{
    if (!TPL_IS_LOG_MANAGER(logManager)) {
        self->setFinishedWithError(TP_QT_ERROR_INVALID_ARGUMENT, "Invalid log manager in callback");
        return;
    }

    if (!G_IS_ASYNC_RESULT(result)) {
        self->setFinishedWithError(TP_QT_ERROR_INVALID_ARGUMENT, "Invalid async result in callback");
        return;
    }

    GList *dates = NULL;
    GError *error = NULL;
    gboolean success = tpl_log_manager_get_dates_finish(TPL_LOG_MANAGER(logManager), G_ASYNC_RESULT(result), &dates, &error);
    if (error) {
        self->setFinishedWithError(TP_QT_ERROR_INVALID_ARGUMENT, error->message);
        g_error_free(error);
        return;
    }

    if (!success) {
        self->setFinishedWithError(TP_QT_ERROR_INVALID_ARGUMENT, "Query failed without specific error");
        return;
    }

    GList *i;
    for (i = dates; i; i = i->next) {
        GDate * item = (GDate *) i->data;
        self->mPriv->dates << QDate(item->year, item->month, item->day);
    }

    g_list_foreach(dates, (GFunc) g_date_free, NULL);
    g_list_free(dates);

    self->setFinished();
}
