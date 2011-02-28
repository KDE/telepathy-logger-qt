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

#include <tpl-message-private.h>
#include <tpl-correspondant-private.h>

using namespace Logger;

bool MessagePrivateData::first_object = true;

QHash<TplEntryDirection, Message::Direction>
    MessagePrivateData::tplToMessageDirectionHash;

MessagePrivateData::MessagePrivateData(TplEntryText *tpmessage)
{
    MessagePrivateData((TplEntry*)tpmessage);
}

MessagePrivateData::MessagePrivateData(TplEntry *tpmessage)
{
    if (this->first_object)
    {
        // Initialises lookup hash (matches Message and TplEntry type codes)
        tplToMessageDirectionHash[TPL_ENTRY_DIRECTION_NONE] = Message::undefined;
        tplToMessageDirectionHash[TPL_ENTRY_DIRECTION_OUT] = Message::outcoming;
        tplToMessageDirectionHash[TPL_ENTRY_DIRECTION_IN] = Message::incoming;

        this->first_object = false;
    }

#if 0
TODO add Message specific props:
   "message"          gchar*
   "message-type"     guint
   "pending-msg-id"   gint
#endif

gchar *gaccountpath, *gchannel, *gchatid, *glogid;
    TplEntity *gsender, *greceiver;
    TplEntryDirection gdirection;

    g_object_get(tpmessage,
                 "account", &this->_account, "account-path", &gaccountpath,
                 "channel-path", &gchannel, "chat-id", &gchatid,
                 "direction", &gdirection, "log-id", &glogid,
                 "receiver", &greceiver, "sender", &gsender,
                 "timestamp", &this->_timestamp, NULL);

    this->_accountpath = QString(gaccountpath);
    this->_channel = QString(gchannel);
    this->_chatid = QString(gchatid);
    this->_logid = QString(glogid);

    // If the value is unknown, QHash automatically returns 0 == Message::undefined
    this->_direction = tplToMessageDirectionHash[gdirection];

    CorrespondantPrivateData *psender = new CorrespondantPrivateData(gsender);
    CorrespondantPrivateData *preceiver = new CorrespondantPrivateData(greceiver);
    this->_receiver = Correspondant(preceiver);
    this->_sender = Correspondant(psender);

    g_free(gaccountpath);
    g_free(gchannel);
    g_free(gchatid);
    g_free(glogid);

    g_object_unref(gsender);
    g_object_unref(greceiver);
}

MessagePrivateData::~MessagePrivateData()
{
    g_object_unref(this->_account);
}

TpAccount* MessagePrivateData::account() const
{
    return this->_account;
}

long MessagePrivateData::timestamp() const
{
    return this->_timestamp;
}

QString MessagePrivateData::accountpath() const
{
    return this->_accountpath;
}

QString MessagePrivateData::channel() const
{
    return this->_channel;
}

QString MessagePrivateData::chatid() const
{
    return this->_chatid;
}

QString MessagePrivateData::logid() const
{
    return this->_logid;
}

Message::Direction MessagePrivateData::direction() const
{
    return this->_direction;
}

Correspondant MessagePrivateData::sender() const
{
    return this->_sender;
}

Correspondant MessagePrivateData::receiver() const
{
    return this->_receiver;
}