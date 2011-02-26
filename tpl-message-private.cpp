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

using namespace Logger;

MessagePrivateData::MessagePrivateData(TplEntryText *tpmessage)
{
    MessagePrivateData((TplEntry*)tpmessage);
}

MessagePrivateData::MessagePrivateData(TplEntry *tpmessage)
{
    gchar *gaccountpath, *gchannel, *gchatid, *glogid;
    TplEntity *gsender, *greceiver;

    g_object_get(tpmessage,
                 "account", &this->account, "account-path", &gaccountpath,
                 "channel-path", &gchannel, "chat-id", &gchatid,
                 "direction", &this->direction, "log-id", &glogid,
                 "receiver", &greceiver, "sender", &gsender,
                 "timestamp", &this->timestamp, NULL);

    this->accountpath = QString(gaccountpath);
    this->channel = QString(gchannel);
    this->chatid = QString(gchatid);
    this->logid = QString(glogid);
//     this->receiver = Correspondant(greceiver); FIXME
//     this->sender = Correspondant(gsender);

    g_free(gaccountpath);
    g_free(gchannel);
    g_free(gchatid);
    g_free(glogid);

    g_object_unref(gsender);
    g_object_unref(greceiver);
}

MessagePrivateData::~MessagePrivateData()
{
    g_object_unref(this->account);
}