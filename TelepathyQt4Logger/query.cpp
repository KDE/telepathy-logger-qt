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

#include "query.h"
#include "query-private.h"

#include <telepathy-logger/log-manager.h>

#include <QtCore/QStringList>
#include <QGlib/Error>

using namespace Tpl;

Query::Query(const QString &dbusid, bool idIsEscaped)
{
    g_type_init();

    QString quotedDbusID = idIsEscaped? dbusid: escapeDBusID(dbusid);

    try {
        this->d = new QueryPrivate(quotedDbusID);
    }
    catch (const std::bad_alloc &e) {
        throw QGlib::Error(TPL_LOG_MANAGER_ERROR, 0, "Could not allocate memory"
                                                     " for Query private data!");
    }
}

Query::~Query()
{
    delete this->d; // REVIEW heap or stack?
}

// Escape string as if it were a valid C indentifier...
QString Query::escapeDBusID(const QString &id)
{
    QStringList chunks = id.split("/");

    // A manager/protocol/username triplet is required
    if (chunks.count() != 3) {
        throw QGlib::Error(TPL_LOG_MANAGER_ERROR, 0,
                            "Provided DBus Telepathy id does not contain "
                            "three '/' separed elements");
    }

    gchar *gEscapedString = tp_escape_as_identifier(chunks[2].toAscii());

    QString escapedString = QString("%1/%2/%3").arg(chunks[0]).arg(chunks[1]).
                                                arg(gEscapedString);

    g_free(gEscapedString);

    return escapedString;
}
