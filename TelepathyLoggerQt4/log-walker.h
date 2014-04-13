/*
 * This file is part of TelepathyLoggerQt4
 *
 * Copyright (C) 2013 Dan Vrátil <dvratil@redhat.com>
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

#ifndef _TelepathyLoggerQt4_log_walker_h_HEADER_GUARD_
#define _TelepathyLoggerQt4_log_walker_h_HEADER_GUARD_

#ifndef IN_TELEPATHY_LOGGER_QT4_HEADER
#error IN_TELEPATHY_LOGGER_QT4_HEADER
#endif

#include <TelepathyLoggerQt4/Types>
#include <TelepathyLoggerQt4/Object>

namespace Tpl {

class PendingOperation;
class PendingEvents;

/*!
 * \headerfile log-manager.h <TelepathyLoggerQt4/LogWalker>
 * \brief The LogWalker object allows the user to sequentially iterate over the logs.
 *
 * \since 0.8.0
 */
class TELEPATHY_LOGGER_QT4_EXPORT LogWalker: public Tpl::Object
{
  public:
    /**
     * \brief Whether the walker points to the most recent event in logs.
     *
     * Determines whether this walker is pointing at the most recent event in
     * the logs. This is the case when it has not yet returned any
     * events or has been rewound completely.
     *
     * \return Returns true if walker is pointing at the most recent event,
     * otherwise false.
     */
    bool isStart();

    /**
     * \brief Whether the walker has run out of events.
     *
     *
     * Determines whether this walker has run out of events. This is the case
     * when it has returned all the events from the logs.
     *
     * \return Returns true if walker has run out of events, otherwise false.
     */
    bool isEnd();

    /**
     * \brief Retrieves \p numEvents of most recent events.
     *
     * Walks the logs to retrieve the next most recent \p numEvents events.
     *
     * \param numEvents Amount of events to retrieve
     * \return Returns PendingOperation that will emit finished() when requested
     * events are received or an error occurs.
     */
    PendingEvents* queryEvents(uint numEvents);

    /**
     * \brief Moves the walker back by the last \p numEvents events.
     *
     * Move the walker back by the last \p numEvents events that were
     * received by getEvents().
     *
     * \param numEvents Amount of events to rewind by
     * \return Returns a PendingOperation that will emit finished() when
     * walker has been rewound of an error occurs.
     */
    PendingOperation* rewind(uint numEvents);

  private:
    QTELEPATHYLOGGERQT4_WRAPPER(LogWalker)
};

} //Tpl

#endif
