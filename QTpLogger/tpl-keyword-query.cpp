/*
 * Copyright (C) 2011 Stefano Sanfilippo <stefano.k.sanfilippo@gmail.com>
 *
 * @author Stefano Sanfilippo <stefano.k.sanfilippo@gmail.com>
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

#include <QTpLogger/tpl-keyword-query.h>

#include <telepathy-logger/log-manager.h>

#include <tpl-query-private.h>
#include <tpl-entry-private.h>
#include <tpl-error.h>

#include <tpl-query-callback-template.h>

using namespace QTpLogger;

KeywordQuery::KeywordQuery(const QString &dbusid) : Query(dbusid)
{
}

void KeywordQuery::perform(const QString &keyword)
{
    // Perform the call...
    tpl_log_manager_search_async(this->d->logmanager(), keyword.toAscii(),
                                 (GAsyncReadyCallback)this->callback, this);
}

void KeywordQuery::callback(void *logmanager, void *result, KeywordQuery *self)
{
    TPL_QUERY_FILL_DATA (logmanager, result, tpl_log_manager_search_finish,
                         TplLogSearchHit, SearchHitPrivate, SearchHit,
                         self->hits);

    // Notify
    Q_EMIT self->completed(self->hits);
}
