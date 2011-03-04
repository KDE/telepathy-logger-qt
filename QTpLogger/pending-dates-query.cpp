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
#include "global.h"
#include <TelepathyQt4/Account>

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
    : PendingOperation(manager.operator ->()),
      mPriv(new Private)
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
        (GAsyncReadyCallback) callback,
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
    TPL_QUERY_FILL_DATA_QT_NATIVE (logManager, result,
                                   tpl_log_manager_get_dates_finish,
                                   GDate, QGDate, self->mPriv->dates);

    self->setFinished();
}

void ConversationDatesQuery::callback(void *logmanager, void *result,
                                      ConversationDatesQuery* self)
{
    // This is different: QDate is a Qt type :)
    // Notify
    Q_EMIT self->completed(self->dates);
}
