/*
 * This file is part of TelepathyLoggerQt
 *
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
#ifndef _TelepathyLoggerQt_call_event_h_HEADER_GUARD_
#define _TelepathyLoggerQt_call_event_h_HEADER_GUARD_

#include "event.h"
#include <TelepathyLoggerQt_export.h>

#include <TelepathyQt/Constants>

namespace Tpl
{

/*!
 * \headerfile call-event.h <TelepathyLoggerQt/CallEvent>
 * \brief Represents a call log event.
 */
class TELEPATHY_LOGGER_QT_EXPORT CallEvent : public Event
{
public:

    /*!
     * \brief Returns the call duration
     */
    QTime duration() const;

    /*!
     * \brief Returns the actor that caused the call to end
     */
    EntityPtr endActor() const;

    /*!
     * \brief Returns the reason for which this call was ended.
     */
    Tp::CallStateChangeReason endReason() const;

    /*!
     * \brief Returns a string representing a D-Bus error that gives more details about
     *         the end reason
     */
    QString detailedEndReason() const;

private:
    QTELEPATHYLOGGERQT_WRAPPER(CallEvent, Event)
};

} //namespace

#endif
