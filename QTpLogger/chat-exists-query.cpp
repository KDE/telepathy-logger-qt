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

#include "chat-exists-query.h"

#include <telepathy-logger/log-manager.h>

#include "query-private.h"

using namespace QTpLogger;

void ChatExistsQuery::perform(const QString &chatname, bool isChatRoom)
{
    // TODO
    /*
    Q_EMIT completed(tpl_log_manager_exists(this->d->logmanager(), this->d->account(),
                                          chatname.toAscii(), isChatRoom));
                                          */
}
