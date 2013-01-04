/*
 * This file is part of TelepathyLoggerQt4
 *
 * Copyright (C) 2011 Collabora Ltd. <http://www.collabora.co.uk/>
 * Copyright (C) 2012 David Edmundson <kde@davidedmundson.co.uk>
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

#include <TelepathyLoggerQt4/PendingEntities>

#include <QtCore/QDebug>
#include <TelepathyQt/Account>
#include <TelepathyLoggerQt4/Entity>
#include <TelepathyLoggerQt4/LogManager>
#include <TelepathyLoggerQt4/utils.h>
#include <TelepathyLoggerQt4/_gen/pending-entities.moc.hpp>
#include <glib.h>
#include <telepathy-logger/log-manager.h>

using namespace Tpl;

struct TELEPATHY_LOGGER_QT4_NO_EXPORT PendingEntities::Private
{
    LogManagerPtr manager;
    Tp::AccountPtr account;
    TpAccount *tpAccount;
    EntityPtrList entities;

    static void onAccountPrepared(void *logManager, void *result, PendingEntities *self);
    static void callback(void *logManager, void *result, PendingEntities *self);
};

PendingEntities::PendingEntities(const LogManagerPtr & manager, const Tp::AccountPtr & account)
    : PendingOperation(),
      mPriv(new Private())
{
    mPriv->manager = manager;
    mPriv->account = account;
    mPriv->tpAccount = 0;
}

PendingEntities::~PendingEntities()
{
    delete mPriv;
}

void PendingEntities::start()
{
    mPriv->tpAccount = Utils::instance()->tpAccount(mPriv->account);
    if (!mPriv->tpAccount) {
        setFinishedWithError(TP_QT_ERROR_INVALID_ARGUMENT, "Invalid account");
        return;
    }

    GQuark features[] = { TP_ACCOUNT_FEATURE_CORE, 0 };
    tp_proxy_prepare_async(mPriv->tpAccount, features, (GAsyncReadyCallback) Private::onAccountPrepared, this);
}

void PendingEntities::Private::onAccountPrepared(void *logManager, void *result, PendingEntities *self)
{
    tpl_log_manager_get_entities_async(self->mPriv->manager,
        self->mPriv->tpAccount,
        (GAsyncReadyCallback) Private::callback,
        self);
}

EntityPtrList PendingEntities::entities() const
{
    if (!isFinished()) {
        qWarning() << "PendingEntities::entities called before finished, returning empty";
        return EntityPtrList();
    } else if (!isValid()) {
        qWarning() << "PendingEntities::entities called when not valid, returning empty";
        return EntityPtrList();
    }

    return mPriv->entities;
}

Tp::AccountPtr PendingEntities::account() const
{
    return mPriv->account;
}


void PendingEntities::Private::callback(void *logManager, void *result, PendingEntities *self)
{
    if (!TPL_IS_LOG_MANAGER(logManager)) {
        self->setFinishedWithError(TP_QT_ERROR_INVALID_ARGUMENT, "Invalid log manager in callback");
        return;
    }

    if (!G_IS_ASYNC_RESULT(result)) {
        self->setFinishedWithError(TP_QT_ERROR_INVALID_ARGUMENT, "Invalid async result in callback");
        return;
    }

    GList *entities = NULL;
    GError *error = NULL;
    gboolean success = tpl_log_manager_get_entities_finish(TPL_LOG_MANAGER(logManager), G_ASYNC_RESULT(result), &entities, &error);
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
    for (i = entities; i; i = i->next) {
        TplEntity * item = (TplEntity *) i->data;
        self->mPriv->entities << EntityPtr::wrap(item, true);
    }

    g_list_foreach(entities, (GFunc) g_object_unref, NULL);
    g_list_free(entities);

    self->setFinished();
}
