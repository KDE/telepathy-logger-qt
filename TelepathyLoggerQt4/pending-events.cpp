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

#include <TelepathyLoggerQt4/PendingEvents>

#include <QtCore/QDebug>
#include <TelepathyQt/Account>
#include <TelepathyLoggerQt4/LogManager>
#include <TelepathyLoggerQt4/LogWalker>
#include <TelepathyLoggerQt4/CallEvent>
#include <TelepathyLoggerQt4/Entity>
#include <TelepathyLoggerQt4/TextEvent>
#include <TelepathyLoggerQt4/utils.h>
#include <TelepathyLoggerQt4/_gen/pending-events.moc.hpp>
#include <glib.h>
#include <telepathy-logger/log-manager.h>
#include <telepathy-logger/event.h>
#include <telepathy-logger/text-event.h>
#include <telepathy-logger/call-event.h>

using namespace Tpl;

struct TELEPATHY_LOGGER_QT4_NO_EXPORT PendingEvents::Private
{
    LogManagerPtr manager;
    LogWalkerPtr logWalker;
    Tp::AccountPtr account;
    TpAccount *tpAccount;
    EntityPtr entity;
    EventTypeMask typeMask;
    QDate date;

    bool filtered;
    uint numEvents;
    LogEventFilter filterFunction;
    void *filterFunctionUserData;

    EventPtrList events;

    static void onAccountPrepared(void *logManager, void *result, PendingEvents *self);
    static void callback(void *caller, void *result, PendingEvents *self);
    static gboolean eventFilterMethod(TplEvent *event, gpointer *user_data);
    static void storeAndFreeEvent(TplEvent *event, PendingEvents *self);
};

PendingEvents::PendingEvents(const LogManagerPtr & manager, const Tp::AccountPtr & account,
    const EntityPtr & entity, EventTypeMask typeMask, const QDate &date)
    : PendingOperation(),
      mPriv(new Private())
{
    mPriv->manager = manager;
    mPriv->account = account;
    mPriv->tpAccount = 0;
    mPriv->entity = entity;
    mPriv->typeMask = typeMask;
    mPriv->date = date;
    mPriv->filtered = false;
    mPriv->filterFunction = 0;
    mPriv->filterFunctionUserData = 0;
}

PendingEvents::PendingEvents(const LogManagerPtr & manager, const Tp::AccountPtr & account,
    const EntityPtr & entity, EventTypeMask typeMask, uint numEvents,
    LogEventFilter filterFunction, void *filterFunctionUserData)
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

PendingEvents::PendingEvents(const LogWalkerPtr& logWalker, uint numEvents)
{
    mPriv->logWalker = logWalker;
    mPriv->numEvents = numEvents;
    mPriv->typeMask = Tpl::EventTypeMaskAny;
    mPriv->filtered = false;
    mPriv->filterFunction = 0;
    mPriv->filterFunctionUserData = 0;
}

PendingEvents::~PendingEvents()
{
    delete mPriv;
}

void PendingEvents::start()
{
    if (mPriv->account) {
        mPriv->tpAccount = Utils::instance()->tpAccount(mPriv->account);
        if (!mPriv->tpAccount) {
            setFinishedWithError(TP_QT_ERROR_INVALID_ARGUMENT, "Invalid account");
            return;
        }

        GQuark features[] = { TP_ACCOUNT_FEATURE_CORE, 0 };
        tp_proxy_prepare_async(mPriv->tpAccount, features, (GAsyncReadyCallback) Private::onAccountPrepared, this);
    } else if (mPriv->logWalker) {
        tpl_log_walker_get_events_async(mPriv->logWalker, mPriv->numEvents, (GAsyncReadyCallback) Private::callback, this);
    }
}

void PendingEvents::Private::onAccountPrepared(void *logManager, void *result, PendingEvents *self)
{
    if (self->mPriv->filtered) {
        tpl_log_manager_get_filtered_events_async(self->mPriv->manager,
            self->mPriv->tpAccount,
            self->mPriv->entity,
            self->mPriv->typeMask,
            self->mPriv->numEvents,
            self->mPriv->filterFunction ? (TplLogEventFilter) Private::eventFilterMethod : 0,
            self,
            (GAsyncReadyCallback) Private::callback,
            self);
    } else {
        GDate *gdate = g_date_new_dmy(
            self->mPriv->date.day(),
            (GDateMonth) self->mPriv->date.month(),
            self->mPriv->date.year());
        tpl_log_manager_get_events_for_date_async(self->mPriv->manager,
            self->mPriv->tpAccount,
            self->mPriv->entity,
            self->mPriv->typeMask,
            gdate,
            (GAsyncReadyCallback) Private::callback,
            self);
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

void PendingEvents::Private::callback(void *caller, void *result, PendingEvents *self)
{
    if (!G_IS_ASYNC_RESULT(result)) {
        self->setFinishedWithError(TP_QT_ERROR_INVALID_ARGUMENT, "Invalid async result in callback");
        return;
    }

    GList *events = NULL;
    GError *error = NULL;
    gboolean success = FALSE;
    if (TPL_IS_LOG_MANAGER(caller)) {
        if (self->mPriv->filtered) {
            success = tpl_log_manager_get_filtered_events_finish(TPL_LOG_MANAGER(caller), G_ASYNC_RESULT(result), &events, &error);
        } else {
            success = tpl_log_manager_get_events_for_date_finish(TPL_LOG_MANAGER(caller), G_ASYNC_RESULT(result), &events, &error);
        }
    } else if (TPL_IS_LOG_WALKER(caller)) {
        success = tpl_log_walker_get_events_finish(TPL_LOG_WALKER(caller), G_ASYNC_RESULT(result), &events, &error);
    } else {
        self->setFinishedWithError(TP_QT_ERROR_INVALID_ARGUMENT, "Invalid callback caller");
        return;
    }

    if (error) {
        self->setFinishedWithError(TP_QT_ERROR_INVALID_ARGUMENT, error->message);
        g_error_free(error);
        return;
    }

    if (!success) {
        self->setFinishedWithError(TP_QT_ERROR_INVALID_ARGUMENT, "Query failed without specific error");
        return;
    }

    g_list_foreach(events, (GFunc) &storeAndFreeEvent, self);
    g_list_free(events);

    self->setFinished();
}

void PendingEvents::Private::storeAndFreeEvent(TplEvent *tplEvent, PendingEvents *self)
{
    if (TPL_IS_TEXT_EVENT(tplEvent)) {
        TextEventPtr eventPtr = TextEventPtr::wrap(TPL_TEXT_EVENT(tplEvent), true);
        self->mPriv->events << eventPtr;
    } else if (TPL_IS_CALL_EVENT(tplEvent)) {
        CallEventPtr eventPtr  = CallEventPtr::wrap(TPL_CALL_EVENT(tplEvent), true);
        self->mPriv->events << eventPtr;
    } else if (TPL_IS_EVENT(tplEvent)) {
        EventPtr eventPtr = EventPtr::wrap(TPL_EVENT(tplEvent), true);
        self->mPriv->events << eventPtr;
    }

    g_object_unref(tplEvent);
}

gboolean PendingEvents::Private::eventFilterMethod(TplEvent *event, gpointer *user_data)
{
    PendingEvents *self = (PendingEvents *) user_data;
    if (!self) {
        return FALSE;
    }

    return self->mPriv->filterFunction(EventPtr::wrap(event, true), self->mPriv->filterFunctionUserData);
}
