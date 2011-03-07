/*
 * This file is part of QTpLogger
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

#ifndef __PENDING_EVENTS_QUERY__
#define __PENDING_EVENTS_QUERY__

#include <QTpLogger/LogManager>
#include <QTpLogger/PendingOperation>
#include <QtCore/QList>
#include <QtCore/QDate>

namespace QTpLogger
{

typedef QList<EventPtr> EventList;

class PendingEvents : public QTpLogger::PendingOperation
{
    Q_OBJECT
    Q_DISABLE_COPY(PendingEvents);

public:
    ~PendingEvents();

    virtual void start();
    EventList events() const;

private:
    friend class LogManager;

    PendingEvents(LogManagerPtr manager, Tp::AccountPtr account, EntityPtr entity, EventTypeMask typeMask, const QDate &date);

    struct Private;
    friend struct Private;
    Private *mPriv;
};

} // QTpLogger

#endif // __PENDING_EVENTS_QUERY__
