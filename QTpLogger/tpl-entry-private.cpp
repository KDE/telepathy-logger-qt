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

#include "tpl-entry-private.h"

using namespace QTpLogger;

bool EntryPrivate::first_object = true;

// TODO port to tp-logger 0.2
/*
QHash<TplEntryDirection, Entry::Direction>
	EntryPrivate::tplToMessageDirectionHash;
        */

EntryPrivate::EntryPrivate(TplEntry *tpmessage)
{
    if (this->first_object)
    {
        // TODO port to tp-logger 0.2
        /*
        // Initialises lookup hash (matches Message and TplEntry type codes)
        tplToMessageDirectionHash[TPL_ENTRY_DIRECTION_NONE] = Entry::undefined;
        tplToMessageDirectionHash[TPL_ENTRY_DIRECTION_OUT] = Entry::outcoming;
        tplToMessageDirectionHash[TPL_ENTRY_DIRECTION_IN] = Entry::incoming;
        */

        this->first_object = false;
    }

    gchar *gaccountpath;
    gchar *gchannel;
    gchar *gchatid;
    gchar *glogid;
    TplEntity *gsender;
    TplEntity *greceiver;
    // TODO port to tp-logger 0.2
    //TplEntryDirection gdirection;

    g_object_get(tpmessage,
                 "account", &this->_account, "account-path", &gaccountpath,
                 "channel-path", &gchannel, "chat-id", &gchatid,
                 // TODO port to tp-logger 0.2
                 //"direction", &gdirection, "log-id", &glogid,
                 "receiver", &greceiver, "sender", &gsender,
                 "timestamp", &this->_timestamp, NULL);

    this->_accountpath = QString(gaccountpath);
    this->_channel = QString(gchannel);
    this->_chatid = QString(gchatid);
    this->_logid = QString(glogid);

    // If the value is unknown, QHash automatically returns 0 == Message::undefined
    // TODO port to tp-logger 0.2
    //this->_direction = tplToMessageDirectionHash[gdirection];

    // TODO update to tp-logger 0.2
    //EntityPrivate *psender = new EntityPrivate(gsender);
    //EntityPrivate *preceiver = new EntityPrivate(greceiver);
    //this->_receiver = Entity(preceiver);
    //this->_sender = Entity(psender);

    g_free(gaccountpath);
    g_free(gchannel);
    g_free(gchatid);
    g_free(glogid);

    g_object_unref(gsender);
    g_object_unref(greceiver);
}

EntryPrivate::~EntryPrivate()
{
    g_object_unref(this->_account);
}

TpAccount* EntryPrivate::account() const
{
    return this->_account;
}

long EntryPrivate::timestamp() const
{
    return this->_timestamp;
}

QString EntryPrivate::accountpath() const
{
    return this->_accountpath;
}

QString EntryPrivate::channel() const
{
    return this->_channel;
}

QString EntryPrivate::chatid() const
{
    return this->_chatid;
}

QString EntryPrivate::logid() const
{
    return this->_logid;
}

Entry::Direction EntryPrivate::direction() const
{
    return this->_direction;
}

Entity *EntryPrivate::sender() const
{
    // TODO update to tp-logger 0.2
    //return this->_sender;
    return 0;
}

Entity *EntryPrivate::receiver() const
{
    // TODO update to tp-logger 0.2
    //return this->_receiver;
    return 0;
}
