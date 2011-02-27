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

#include <Logger/tpl-message.h>
#include <Logger/tpl-correspondant.h>

#include <QtCore/QHash>

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

    TpAccount* account() const;
    long timestamp() const;

    QString accountpath() const;
    QString channel() const;
    QString chatid() const;
    QString logid() const;
    Message::Direction direction() const;
    Correspondant sender() const;
    Correspondant receiver() const;

private:
    TpAccount* _account;
    long _timestamp;

    QString _accountpath;
    QString _channel;
    QString _chatid;
    QString _logid;
    Message::Direction _direction;
    Correspondant _sender;
    Correspondant _receiver;

    static QHash<TplEntryDirection, Message::Direction> tplToMessageDirectionHash;
    static bool first_object;
};

} //namespace

#endif // __MESSAGE_PRIVATE__