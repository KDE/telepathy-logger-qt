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

#include "entry-text-private.h"

#include <glib.h>

using namespace QTpLogger;

EntryTextPrivate::EntryTextPrivate(TplEntryText *tpmessage) :
    EntryPrivate(reinterpret_cast<TplEntry*>(tpmessage))
{
    gchar *gmessage;

    g_object_get(tpmessage,
                 "message", &gmessage, "message-type", &this->_type,
                 "pending-msg-id", &this->_pendingID, NULL);

    this->_message = QString(gmessage);

    g_free(gmessage);
}

QString EntryTextPrivate::message() const
{
    return this->_message;
}

uint EntryTextPrivate::type() const
{
    return this->_type;
}

int EntryTextPrivate::pendingID() const
{
    return this->_pendingID;
}