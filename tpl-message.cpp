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

// #include <Logger/tpl-correspondant.h>

using namespace Logger;

Message::Message()
{
    this->d = new MessagePrivateData;
}

Message::~Message()
{
    delete this->d;
}

QString Message::accountpath()
{
    return this->d->accountpath;
}

QString Message::channel()
{
    return this->d->channel;
}

QString Message::chatid()
{
    return this->d->chatid;
}

QString Message::logid()
{
    return this->d->logid;
}

uint Message::direction()
{
    return this->d->direction;
}

Correspondant Message::sender()
{
    return this->d->sender;
}

Correspondant Message::receiver()
{
    return this->d->receiver;
}

long Message::timestamp()
{
    return this->d->timestamp;
}
