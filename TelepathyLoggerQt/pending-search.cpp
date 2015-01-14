/*
 * This file is part of TelepathyLoggerQt
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

#include <TelepathyLoggerQt/PendingSearch>

#include <QtCore/QDebug>
#include <TelepathyQt/Account>
#include <TelepathyLoggerQt/Entity>
#include <TelepathyLoggerQt/LogManager>
#include <TelepathyLoggerQt/SearchHit>
#include <TelepathyLoggerQt/utils.h>
#include <TelepathyLoggerQt/_gen/pending-search.moc.hpp>
#include <glib.h>
#include <telepathy-logger/log-manager.h>

using namespace Tpl;

struct TELEPATHY_LOGGER_QT_NO_EXPORT PendingSearch::Private
{
    LogManagerPtr manager;
    QString text;
    EventTypeMask typeMask;
    SearchHitList hits;

    static void callback(void *logManager, void *result, PendingSearch *self);
};

PendingSearch::PendingSearch(const LogManagerPtr & manager, const QString &text, EventTypeMask typeMask)
    : PendingOperation(),
      mPriv(new Private())
{
    mPriv->manager = manager;
    mPriv->text = text;
    mPriv->typeMask = typeMask;
}

PendingSearch::~PendingSearch()
{
    delete mPriv;
}

void PendingSearch::start()
{
    tpl_log_manager_search_async(
        TPLoggerQtWrapper::unwrap<TplLogManager, LogManager>(mPriv->manager),
        mPriv->text.toUtf8(),
        mPriv->typeMask,
        (GAsyncReadyCallback) Private::callback,
        this);
}

SearchHitList PendingSearch::hits() const
{
    if (!isFinished()) {
        qWarning() << "PendingSearch::dates called before finished, returning empty";
        return SearchHitList();
    } else if (!isValid()) {
        qWarning() << "PendingSearch::dates called when not valid, returning empty";
        return SearchHitList();
    }

    return mPriv->hits;
}

void PendingSearch::Private::callback(void *logManager, void *result, PendingSearch *self)
{
    if (!TPL_IS_LOG_MANAGER(logManager)) {
        self->setFinishedWithError(TP_QT_ERROR_INVALID_ARGUMENT, "Invalid log manager in callback");
        return;
    }

    if (!G_IS_ASYNC_RESULT(result)) {
        self->setFinishedWithError(TP_QT_ERROR_INVALID_ARGUMENT, "Invalid async result in callback");
        return;
    }

    GList *hits = NULL;
    GError *error = NULL;
    gboolean success = tpl_log_manager_search_finish(TPL_LOG_MANAGER(logManager), G_ASYNC_RESULT(result), &hits, &error);
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
    int count = 0;
    for (i = hits; i; i = i->next) {
        TplLogSearchHit *item = (TplLogSearchHit *) i->data;
        qDebug() << "hit " << count++ << "account=" << item->account
                  << "date=" << g_date_get_year(item->date) << g_date_get_month(item->date) << g_date_get_day(item->date)
                  << "target=" << item->target
                  << tpl_entity_get_identifier(item->target)
                  << "/" << tpl_entity_get_alias(item->target)
                  << "/" << tpl_entity_get_entity_type(item->target)
                  << "/" << tpl_entity_get_avatar_token(item->target);
	QDate date;
	if (item->date) {
	    date.setDate(item->date->year, item->date->month, item->date->day);
	}
        SearchHit hit(Utils::instance()->accountPtr(item->account), TPLoggerQtWrapper::wrap<TplEntity, Entity>(item->target, true), date);
        self->mPriv->hits << hit;
    }

    tpl_log_manager_search_free(hits);

    self->setFinished();
}
