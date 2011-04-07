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
#include <TelepathyQt4Logger/Entity>
#include <TelepathyQt4Logger/LogManager>
#include <TelepathyQt4Logger/PendingEntities>
#include <TelepathyQt4Logger/utils.h>
#include <TelepathyQt4Logger/_gen/pending-entities.moc.hpp>
#include <glib/gerror.h>
#include <glib/gdate.h>
#include <telepathy-logger/log-manager.h>

using namespace Tpl;

struct TELEPATHY_QT4_LOGGER_NO_EXPORT PendingEntities::Private
{
    LogManagerPtr manager;
    Tp::AccountPtr account;
    EntityPtrList entities;

    static void callback(void *logManager, void *result, PendingEntities *self);
};

PendingEntities::PendingEntities(const LogManagerPtr & manager, const Tp::AccountPtr & account)
    : PendingOperation(),
      mPriv(new Private())
{
    mPriv->manager = manager;
    mPriv->account = account;
}

PendingEntities::~PendingEntities()
{
    delete mPriv;
}

void PendingEntities::start()
{
    tpl_log_manager_get_entities_async(mPriv->manager,
        Utils::instance()->tpAccount(mPriv->account),
        (GAsyncReadyCallback) Private::callback,
        this);
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

void PendingEntities::Private::callback(void *logManager, void *result, PendingEntities *self)
{
    if (!TPL_IS_LOG_MANAGER(logManager)) {
        self->setFinishedWithError(TP_QT4_ERROR_INVALID_ARGUMENT, "Invalid log manager in callback");
        return;
    }

    if (!G_IS_ASYNC_RESULT(result)) {
        self->setFinishedWithError(TP_QT4_ERROR_INVALID_ARGUMENT, "Invalid async result in callback");
        return;
    }

    GList *entities = NULL;
    GError *error = NULL;
    gboolean success = tpl_log_manager_get_entities_finish(TPL_LOG_MANAGER(logManager), G_ASYNC_RESULT(result), &entities, &error);
    if (error) {
        self->setFinishedWithError(TP_QT4_ERROR_INVALID_ARGUMENT, error->message);
        g_error_free(error);
        return;
    }

    if (!success) {
        self->setFinishedWithError(TP_QT4_ERROR_INVALID_ARGUMENT, "Query failed without specific error");
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
