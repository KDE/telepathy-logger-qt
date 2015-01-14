/*
 * This file is part of TelepathyLoggerQt
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
#ifndef _TelepathyLoggerQt_text_event_h_HEADER_GUARD_
#define _TelepathyLoggerQt_text_event_h_HEADER_GUARD_

#ifndef IN_TELEPATHY_LOGGER_QT_HEADER
#error IN_TELEPATHY_LOGGER_QT_HEADER
#endif

#include <TelepathyLoggerQt/Event>
#include <TelepathyQt/Constants>

namespace Tpl
{

/*!
 * \headerfile text-event.h <TelepathyLoggerQt/TextEvent>
 * \brief A subclass of Event representing a text log event.
 */
class TELEPATHY_LOGGER_QT_EXPORT TextEvent : public Event
{
public:

    /*!
     * \brief Returns the type of the message.
     */
    Tp::ChannelTextMessageType messageType() const;

    /*!
     * \brief Returns content of the message.
     */
    QString message() const;

    /*!
     * \brief Returns token of the message.
     */
    QString messageToken() const;

    /*!
     * \brief Returns timestamp when the message was sent or delivered.
     */
    QDateTime editTimestamp() const;

    /*!
     * \brief Returns token of a message this message supersedes.
     */
    QString supersedesToken() const;

    /*!
     * \brief Returns a list of messages this messages supersedes.
     */
    QList<TextEventPtr> supersedes() const;

private:
    QTELEPATHYLOGGERQT_WRAPPER(TextEvent, Event)
};

} //namespace

#endif
