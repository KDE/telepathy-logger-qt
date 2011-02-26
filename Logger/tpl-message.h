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

#include <Logger/Correspondant>

#include <QtCore/QString>

namespace Logger
{

class MessagePrivateData;

class Message
{
public:
    Message();
    ~Message();

    QString accountpath();
    QString channel();
    QString chatid();
    QString logid();

    uint direction(); //FIXME
    Correspondant sender();
    Correspondant receiver();

    long timestamp();

    enum Direction {  undefined = 0,
        incoming, /* = TPL_ENTRY_DIRECTION_IN,*/
        outcoming /*= TPL_ENTRY_DIRECTION_OUT*/
    };

private:
    friend class MessagesForDateQuery;
    MessagePrivateData *d;
};

} //namespace

#endif // __MESSAGE__