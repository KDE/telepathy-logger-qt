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

#include <Logger/tpl-message.h>
#include <Logger/tpl-correspondant.h>

using namespace Logger;

Message::Message(TplEntryText *tpmessage)
{
    gchar *gaccountpath, *gchannel, *gchatid, *glogid;
    TplEntity *gsender, *greceiver;

    g_object_get(tpmessage,
                 "account", &this->_account, "account-path", &gaccountpath,
                 "channel-path", &gchannel, "chat-id", &gchatid,
                 "direction", &this->_direction, "log-id", &glogid,
                 "receiver", &greceiver, "sender", &gsender,
                 "timestamp", &this->_timestamp, NULL);

    this->_accountpath = QString(gaccountpath);
    this->_channel = QString(gchannel);
    this->_chatid = QString(gchatid);
    this->_logid = QString(glogid);
    this->_receiver = Correspondant(greceiver);
    this->_sender = Correspondant(gsender);
}

Message::Message(TplEntry *tpmessage)
{
}

Correspondant::Correspondant(TplEntity *chat)
{
    gchar *galias, *gid, *gavatar;

    g_object_get(chat,
                 "alias", &galias, "identifier", &gid,
                 "avatar-token", &gavatar, "entity-type", &this->type, NULL);

    this->alias = QString(galias);
    this->id = QString(id);
    this->avatar = QString(gavatar);
}


QString Message::accountpath()
{
    return this->_accountpath;
}

QString Message::channel()
{
    return this->_channel;
}

QString Message::chatid()
{
    return this->_chatid;
}

QString Message::logid()
{
    return this->_logid;
}

Message::Direction Message::direction()
{
    return this->_direction;
}

Correspondant Message::sender()
{
    return this->_sender;
}

Correspondant Message::receiver()
{
    return this->_receiver;
}

TpAccount* Message::account()
{
    return this->_account;
}

long Message::timestamp()
{
    return this->_timestamp;
}
