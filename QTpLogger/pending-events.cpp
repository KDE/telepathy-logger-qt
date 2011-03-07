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

#include "pending-events.h"
#include "entity.h"
#include "constants.h"
#include "text-event.h"
#include "event.h"
#include <TelepathyQt4/Account>
#include "telepathy-logger/log-manager.h"
#include "telepathy-logger/event.h"
#include "telepathy-logger/text-event.h"
#include <QDebug>
#include <glib/gerror.h>
#include <glib/gdate.h>

using namespace QTpLogger;

struct QTPLOGGER_NO_EXPORT PendingEvents::Private
{
    LogManagerPtr manager;
    Tp::AccountPtr account;
    EntityPtr entity;
    EventTypeMask typeMask;
    QDate date;
    EventList events;

    static void callback(void *logManager, void *result, PendingEvents *self);
};

PendingEvents::PendingEvents(LogManagerPtr manager, Tp::AccountPtr account, EntityPtr entity, EventTypeMask typeMask, const QDate &date)
    : PendingOperation(),
      mPriv(new Private())
{
    mPriv->manager = manager;
    mPriv->account = account;
    mPriv->entity = entity;
    mPriv->typeMask = typeMask;
    mPriv->date = date;
}

PendingEvents::~PendingEvents()
{
    delete mPriv;
}

void PendingEvents::start()
{
    // TODO what to do with AccountPtr
    GDate *gdate = g_date_new_dmy(
        mPriv->date.day(),
        (GDateMonth) mPriv->date.month(),
        mPriv->date.year());
    tpl_log_manager_get_events_for_date_async(mPriv->manager,
        0, // mPriv->account
        mPriv->entity,
        mPriv->typeMask,
        gdate,
        (GAsyncReadyCallback) Private::callback,
        this);
    g_date_free(gdate);
}

EventList PendingEvents::events() const
{
    if (!isFinished()) {
        qWarning() << "PendingEvents::events called before finished, returning empty";
        return EventList();
    } else if (!isValid()) {
        qWarning() << "PendingEvents::events called when not valid, returning empty";
        return EventList();
    }

    return mPriv->events;
}

void PendingEvents::Private::callback(void *logManager, void *result, PendingEvents *self)
{
    if (!TPL_IS_LOG_MANAGER(logManager)) {
        self->setFinishedWithError(QTPLOGGER_ERROR_INVALID_ARGUMENT, "Invalid log manager in callback");
        return;
    }

    if (!G_IS_ASYNC_RESULT(result)) {
        self->setFinishedWithError(QTPLOGGER_ERROR_INVALID_ARGUMENT, "Invalid async result in callback");
        return;
    }

    GList *events = NULL;
    GError *error = NULL;
    gboolean success = tpl_log_manager_get_events_for_date_finish(TPL_LOG_MANAGER(logManager), G_ASYNC_RESULT(result), &events, &error);
    if (error) {
        self->setFinishedWithError(QTPLOGGER_ERROR_INVALID_ARGUMENT, error->message);
        g_error_free(error);
        return;
    }

    if (!success) {
        self->setFinishedWithError(QTPLOGGER_ERROR_INVALID_ARGUMENT, "Query failed without specific error");
        return;
    }

    GList *i;
    for (i = events; i; i = i->next) {
        TplEvent * item = (TplEvent *) i->data;
        if (TPL_IS_TEXT_EVENT(item)) {
            TextEventPtr eventPtr = TextEventPtr::wrap(TPL_TEXT_EVENT(item), false);
            self->mPriv->events << eventPtr;
        } else if (TPL_IS_EVENT(item)) {
            EventPtr eventPtr = EventPtr::wrap(TPL_EVENT(item), false);
            self->mPriv->events << eventPtr;
        }
    }

    //g_list_foreach(dates, (GFunc) , NULL);
    g_list_free(events);

    self->setFinished();
}
