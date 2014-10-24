/*
 * This file is part of TelepathyLoggerQt
 *
 * Copyright (C) 2011 Stefano Sanfilippo <stefano.k.sanfilippo@gmail.com>
 * Copyright (C) 2011 Collabora Ltd. <http://www.collabora.co.uk/>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */
#ifndef _TelepathyLoggerQt_pending_events_h_HEADER_GUARD_
#define _TelepathyLoggerQt_pending_events_h_HEADER_GUARD_

#ifndef IN_TELEPATHY_LOGGER_QT_HEADER
#error IN_TELEPATHY_LOGGER_QT_HEADER
#endif

#include <TelepathyLoggerQt/PendingOperation>
#include <TelepathyLoggerQt/Types>

namespace Tpl
{

/**
 * \headerfile pending-events.h <TelepathyLoggerQt/PendingEvents>
 * \brief An operation for retrieving events from the logger storage
 */
class TELEPATHY_LOGGER_QT_EXPORT PendingEvents : public Tpl::PendingOperation
{
    Q_OBJECT
    Q_DISABLE_COPY(PendingEvents);

public:
    /**
     * \brief Destructor.
     */
    ~PendingEvents();

    /**
     * \brief Returns all events retrieved by the operation.
     *
     * It's an error calling this method before the finished() signal is emitted.
     */
    EventPtrList events() const;

private Q_SLOTS:
    virtual void start();

private:
    friend class LogManager;
    friend class LogWalker;

    PendingEvents(const LogManagerPtr & manager, const Tp::AccountPtr & account,
            const EntityPtr & entity, EventTypeMask typeMask, const QDate &date);
    PendingEvents(const LogManagerPtr & manager, const Tp::AccountPtr & account,
            const EntityPtr & entity, EventTypeMask typeMask, uint numEvents,
            LogEventFilter filterFunction, void *filterFunctionUserData);
    PendingEvents(const LogWalkerPtr & logWalker, uint numEvents);

    struct Private;
    friend struct Private;
    Private *mPriv;
};

} // Tpl

#endif
