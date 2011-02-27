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

    enum Direction { undefined = 0, incoming, outcoming };

    QString accountpath() const;
    QString channel() const;
    QString chatid() const;
    QString logid() const;

    Direction direction() const;
    Correspondant sender() const;
    Correspondant receiver() const;

    long timestamp() const;

private:
    Message(MessagePrivateData *_d) : d(_d) {}

    friend class MessagesForDateQuery;

    MessagePrivateData *d;
};

} //namespace

#endif // __MESSAGE__