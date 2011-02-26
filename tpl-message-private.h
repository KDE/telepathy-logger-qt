#ifndef __MESSAGE_PRIVATE__
#define __MESSAGE_PRIVATE__

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

#include <telepathy-logger/entry-text.h>

namespace Logger
{

class Message;

class MessagePrivateData
{
public:
    MessagePrivateData(TplEntry *tpmessage);
    MessagePrivateData(TplEntryText *tpmessage);
    MessagePrivateData();
    ~MessagePrivateData();

    TpAccount* account;
    long timestamp;

    QString accountpath, channel, chatid, logid;
    Message::Direction direction;
    Correspondant sender, receiver;

//   "message"                  gchar*
//   "message-type"             guint
//   "pending-msg-id"           gint
/*    enum Direction {
    undefined, = 0,
    incoming,  = TPL_ENTRY_DIRECTION_IN,
    outcoming = TPL_ENTRY_DIRECTION_OUT*/
};

} //namespace

#endif // __MESSAGE_PRIVATE__