/*
 * This file is part of TelepathyLoggerQt4
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
#ifndef _TelepathyLoggerQt4_text_event_h_HEADER_GUARD_
#define _TelepathyLoggerQt4_text_event_h_HEADER_GUARD_

#ifndef IN_TELEPATHY_LOGGER_QT4_HEADER
#error IN_TELEPATHY_LOGGER_QT4_HEADER
#endif

#include <TelepathyLoggerQt4/Event>
#include <TelepathyQt/Constants>

namespace Tpl
{

/*! \headerfile text-event.h <TelepathyLoggerQt4/TextEvent>
 * \brief Wrapper class for TplTextEvent
 */
class TELEPATHY_LOGGER_QT4_EXPORT TextEvent : public Event
{
    QTELEPATHYLOGGERQT4_WRAPPER(TextEvent)
public:
    Tp::ChannelTextMessageType messageType() const;
    QString message() const;
    QString messageToken() const;
};

} //namespace

QTELEPATHYLOGGERQT4_REGISTER_TYPE(Tpl::TextEvent)

#endif
