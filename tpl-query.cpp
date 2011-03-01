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

#include <telepathy-logger/log-manager.h>

#include <Logger/tpl-query.h>
#include <Logger/tpl-error.h>

#include <tpl-query-private.h>

#include <QStringList>

using namespace Logger;

Query::Query(const QString &dbusid, bool idIsEscaped)
{
    g_type_init();

    QString quotedDbusID;

    // Escape string as if it were a valid C indentifier...
    if (!idIsEscaped) {
        QStringList chunks = dbusid.split("/");

        // A manager/protocol/username triplet is required
        if (chunks.count() != 3) {
            throw Error("Provided DBus Telepathy id does not contain three "
                        "'/' separed elements");
        }

        gchar *escapedUserName = tp_escape_as_identifier(chunks[2].toAscii());

        quotedDbusID = QString("%1/%2/%3").arg(chunks[0]).arg(chunks[1]).
                         arg(escapedUserName);

        g_free(escapedUserName);
    }

    try {
        this->d = new QueryPrivateData(quotedDbusID);
    }
    catch (const std::bad_alloc &e) {
        throw Error("Could not allocate memory for Query private data!");
    }
}

Query::~Query()
{
    delete this->d; // REVIEW heap or stack?
}
