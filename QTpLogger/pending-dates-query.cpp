/*
 * This file is part of QTpLogger
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

#include "pending-dates-query.h"
#include "entity.h"
#include "constants.h"
#include <TelepathyQt4/Account>
#include "telepathy-logger/log-manager.h"
#include <QDebug>
#include <glib/gerror.h>
#include <glib/gdate.h>

using namespace QTpLogger;

struct QTPLOGGER_NO_EXPORT PendingDatesQuery::Private
{
    LogManagerPtr manager;
    Tp::AccountPtr account;
    EntityPtr entity;
    EventTypeMask typeMask;
    QDateList dates;

    static void callback(void *logManager, void *result, PendingDatesQuery *self);
};

PendingDatesQuery::PendingDatesQuery(LogManagerPtr manager, Tp::AccountPtr account, EntityPtr entity, EventTypeMask typeMask)
    : PendingOperation(),
      mPriv(new Private())
{
    mPriv->manager = manager;
    mPriv->account = account;
    mPriv->entity = entity;
    mPriv->typeMask = typeMask;
}

PendingDatesQuery::~PendingDatesQuery()
{
    delete mPriv;
}

void PendingDatesQuery::start()
{
    // TODO what to do with AccountPtr
    tpl_log_manager_get_dates_async(mPriv->manager,
        0, // mPriv->account
        mPriv->entity,
        mPriv->typeMask,
        (GAsyncReadyCallback) Private::callback,
        this);
}

QDateList PendingDatesQuery::dates() const
{
    if (!isFinished()) {
        qWarning() << "PendingDatesQuery::dates called before finished, returning empty";
        return QDateList();
    } else if (!isValid()) {
        qWarning() << "PendingDatesQuery::dates called when not valid, returning empty";
        return QDateList();
    }

    return mPriv->dates;
}

void PendingDatesQuery::Private::callback(void *logManager, void *result, PendingDatesQuery *self)
{
    if (!TPL_IS_LOG_MANAGER(logManager)) {
        self->setFinishedWithError(QTPLOGGER_ERROR_INVALID_ARGUMENT, "Invalid log manager in callback");
        return;
    }

    if (!G_IS_ASYNC_RESULT(result)) {
        self->setFinishedWithError(QTPLOGGER_ERROR_INVALID_ARGUMENT, "Invalid async result in callback");
        return;
    }

    GList *dates = NULL;
    GError *error = NULL;
    gboolean success = tpl_log_manager_get_dates_finish(TPL_LOG_MANAGER(logManager), G_ASYNC_RESULT(result), &dates, &error);
    if (error) {
        self->setFinishedWithError(QTPLOGGER_ERROR_INVALID_ARGUMENT, error->message);
        return;
    }

    if (!success) {
        self->setFinishedWithError(QTPLOGGER_ERROR_INVALID_ARGUMENT, "Query failed with specific error");
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