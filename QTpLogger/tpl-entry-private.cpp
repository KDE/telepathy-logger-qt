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

#include <QTpLogger/tpl-entry.h>

#include <tpl-entry-private.h>
#include <tpl-entity-private.h>

using namespace Logger;

bool EntryPrivate::first_object = true;

QHash<TplEntryDirection, Entry::Direction>
	EntryPrivate::tplToMessageDirectionHash;

EntryPrivate::EntryPrivate(TplEntryText *tpmessage)
{
	EntryPrivate((TplEntry*)tpmessage);
}

EntryPrivate::EntryPrivate(TplEntry *tpmessage)
{
    if (this->first_object)
    {
        // Initialises lookup hash (matches Message and TplEntry type codes)
		tplToMessageDirectionHash[TPL_ENTRY_DIRECTION_NONE] = Entry::undefined;
		tplToMessageDirectionHash[TPL_ENTRY_DIRECTION_OUT] = Entry::outcoming;
		tplToMessageDirectionHash[TPL_ENTRY_DIRECTION_IN] = Entry::incoming;

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

	EntityPrivate *psender = new EntityPrivate(gsender);
	EntityPrivate *preceiver = new EntityPrivate(greceiver);
	this->_receiver = Entity(preceiver);
	this->_sender = Entity(psender);

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

Entity EntryPrivate::sender() const
{
    return this->_sender;
}

Entity EntryPrivate::receiver() const
{
    return this->_receiver;
}
