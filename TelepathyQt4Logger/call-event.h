/*
 * This file is part of TelepathyQt4Logger
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
#ifndef _TelepathyQt4Logger_call_event_h_HEADER_GUARD_
#define _TelepathyQt4Logger_call_event_h_HEADER_GUARD_

#ifndef IN_TELEPATHY_QT4_LOGGER_HEADER
#error IN_TELEPATHY_QT4_LOGGER_HEADER
#endif

#include <TelepathyQt4Logger/Event>

namespace Tpl
{

/*! \headerfile call-event.h <TelepathyQt4Logger/CallEvent>
 * \brief Wrapper class for TplCallEvent
 */
class TELEPATHY_QT4_LOGGER_EXPORT CallEvent : public Event
{
    QTELEPATHYQT4LOGGER_WRAPPER(CallEvent)
public:
    QTime duration() const;
    EntityPtr endActor() const;
    CallEndReason endReason() const;
    QString detailedEndReason() const;
};

} //namespace

QTELEPATHYQT4LOGGER_REGISTER_TYPE(Tpl::CallEvent)

#endif
