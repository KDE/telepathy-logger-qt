/*
 * This file is part of TelepathyQt4Logger
 *
 * Copyright (C) 2011 Stefano Sanfilippo <stefano.k.sanfilippo@gmail.com>
 * Copyright (C) 2011 Collabora Ltd. <http://www.collabora.co.uk/>
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
#ifndef _TelepathyQt4Logger_text_event_h_HEADER_GUARD_
#define _TelepathyQt4Logger_text_event_h_HEADER_GUARD_

#include <QGlib/Object>
#include <TelepathyQt4/Constants>
#include <TelepathyQt4Logger/Event>
#include <TelepathyQt4Logger/Types>

namespace Tpl
{

/*! \headerfile text-event.h <TelepathyQt4Logger/TextEvent>
 * \brief Wrapper class for TplTextEvent
 */
class TextEvent : public Event
{
    QTELEPATHYQT4LOGGER_WRAPPER(TextEvent)
public:
    Tp::ChannelTextMessageType messageType();
    QString message();
};

} //namespace

QTELEPATHYQT4LOGGER_REGISTER_TYPE(Tpl::TextEvent)

#endif
