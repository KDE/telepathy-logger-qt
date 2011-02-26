#ifndef __MESSAGE__
#define __MESSAGE__

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

// XXX
#include <telepathy-logger/entry-text.h>

#include <Logger/Correspondant>

#include <QtCore/QString>

namespace Logger
{

class Message
{
public:
    Message(TplEntry *tpmessage);
    Message(TplEntryText *tpmessage);
    Message() {};

//   "message"                  gchar*
//   "message-type"             guint
//   "pending-msg-id"           gint

    enum Direction {
        undefined = 0,
        incoming  = TPL_ENTRY_DIRECTION_IN,
        outcoming = TPL_ENTRY_DIRECTION_OUT
    };

    QString accountpath();
    QString channel();
    QString chatid();
    QString logid();

    Direction direction();
    Correspondant sender();
    Correspondant receiver();

    TpAccount* account();
    long timestamp();

private:
//     MessagePrivate data;

    TpAccount* _account;
    long _timestamp;

    QString _accountpath, _channel, _chatid, _logid;
    Direction _direction;
    Correspondant _sender, _receiver;
};

} //namespace

#endif // __MESSAGE__