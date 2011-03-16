/*
 * This file is part of TelepathyQt4Logger
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

#include <QDebug>
#include <TelepathyQt4/Account>
#include <TelepathyQt4Logger/Constants>
#include <TelepathyQt4Logger/Entity>
#include <TelepathyQt4Logger/PendingEvents>
#include <TelepathyQt4Logger/TextEvent>
#include <TelepathyQt4Logger/utils.h>
#include <TelepathyQt4Logger/_gen/pending-events.moc.hpp>
#include <glib/gerror.h>
#include <glib/gdate.h>
#include <telepathy-logger/log-manager.h>
#include <telepathy-logger/event.h>
#include <telepathy-logger/text-event.h>

using namespace Tpl;

struct TELEPATHY_QT4_LOGGER_NO_EXPORT PendingEvents::Private
{
    LogManagerPtr manager;
    Tp::AccountPtr account;
    EntityPtr entity;
    EventTypeMask typeMask;
    QDate date;

    bool filtered;
    uint numEvents;
    LogEventFilter filterFunction;
    void *filterFunctionUserData;

    EventPtrList events;

    static void callback(void *logManager, void *result, PendingEvents *self);
    static gboolean eventFilterMethod(TplEvent *event, gpointer *user_data);
};

PendingEvents::PendingEvents(LogManagerPtr manager, Tp::AccountPtr account, EntityPtr entity,
    EventTypeMask typeMask, const QDate &date)
    : PendingOperation(),
      mPriv(new Private())
{
    mPriv->manager = manager;
    mPriv->account = account;
    mPriv->entity = entity;
    mPriv->typeMask = typeMask;
    mPriv->date = date;
    mPriv->filtered = false;
    mPriv->filterFunction = 0;
    mPriv->filterFunctionUserData = 0;
}

PendingEvents::PendingEvents(LogManagerPtr manager, Tp::AccountPtr account, EntityPtr entity,
    EventTypeMask typeMask, uint numEvents, LogEventFilter filterFunction, void *filterFunctionUserData)
    : PendingOperation(),
      mPriv(new Private())
{
    mPriv->manager = manager;
    mPriv->account = account;
    mPriv->entity = entity;
    mPriv->typeMask = typeMask;
    mPriv->filtered = true;
    mPriv->numEvents = numEvents;
    mPriv->filterFunction = filterFunction;
    mPriv->filterFunctionUserData = filterFunctionUserData;
}

PendingEvents::~PendingEvents()
{
    delete mPriv;
}

void PendingEvents::start()
{
    if (mPriv->filtered) {
        tpl_log_manager_get_filtered_events_async(mPriv->manager,
            Utils::instance()->tpAccount(mPriv->account),
            mPriv->entity,
            mPriv->typeMask,
            mPriv->numEvents,
            (TplLogEventFilter) Private::eventFilterMethod,
            this,
            (GAsyncReadyCallback) Private::callback,
            this);
    } else {
        GDate *gdate = g_date_new_dmy(
            mPriv->date.day(),
            (GDateMonth) mPriv->date.month(),
            mPriv->date.year());
        tpl_log_manager_get_events_for_date_async(mPriv->manager,
            Utils::instance()->tpAccount(mPriv->account),
            mPriv->entity,
            mPriv->typeMask,
            gdate,
            (GAsyncReadyCallback) Private::callback,
            this);
        g_date_free(gdate);
    }
}

EventPtrList PendingEvents::events() const
{
    if (!isFinished()) {
        qWarning() << "PendingEvents::events called before finished, returning empty";
        return EventPtrList();
    } else if (!isValid()) {
        qWarning() << "PendingEvents::events called when not valid, returning empty";
        return EventPtrList();
    }

    return mPriv->events;
}

void PendingEvents::Private::callback(void *logManager, void *result, PendingEvents *self)
{
    if (!TPL_IS_LOG_MANAGER(logManager)) {
        self->setFinishedWithError(TELEPATHY_QT4_LOGGER_ERROR_INVALID_ARGUMENT, "Invalid log manager in callback");
        return;
    }

    if (!G_IS_ASYNC_RESULT(result)) {
        self->setFinishedWithError(TELEPATHY_QT4_LOGGER_ERROR_INVALID_ARGUMENT, "Invalid async result in callback");
        return;
    }

    GList *events = NULL;
    GError *error = NULL;
    gboolean success = FALSE;

    if (self->mPriv->filtered) {
        success = tpl_log_manager_get_filtered_events_finish(TPL_LOG_MANAGER(logManager), G_ASYNC_RESULT(result), &events, &error);
    } else {
        success = tpl_log_manager_get_events_for_date_finish(TPL_LOG_MANAGER(logManager), G_ASYNC_RESULT(result), &events, &error);
    }

    if (error) {
        self->setFinishedWithError(TELEPATHY_QT4_LOGGER_ERROR_INVALID_ARGUMENT, error->message);
        g_error_free(error);
        return;
    }

    if (!success) {
        self->setFinishedWithError(TELEPATHY_QT4_LOGGER_ERROR_INVALID_ARGUMENT, "Query failed without specific error");
        return;
    }

    GList *i;
    for (i = events; i; i = i->next) {
        TplEvent * item = (TplEvent *) i->data;
        if (TPL_IS_TEXT_EVENT(item)) {
            TextEventPtr eventPtr = TextEventPtr::wrap(TPL_TEXT_EVENT(item), true);
            self->mPriv->events << eventPtr;
        } else if (TPL_IS_EVENT(item)) {
            EventPtr eventPtr = EventPtr::wrap(TPL_EVENT(item), true);
            self->mPriv->events << eventPtr;
        }
    }

    //g_list_foreach(dates, (GFunc) , NULL);
    g_list_free(events);

    self->setFinished();
}

gboolean PendingEvents::Private::eventFilterMethod(TplEvent *event, gpointer *user_data)
{
    PendingEvents *self = (PendingEvents *) user_data;
    if (!self) {
        return FALSE;
    }

    return self->mPriv->filterFunction(EventPtr::wrap(event, false), self->mPriv->filterFunctionUserData);
}
