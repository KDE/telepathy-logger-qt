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
#ifndef _TelepathyLoggerQt_pending_dates_h_HEADER_GUARD_
#define _TelepathyLoggerQt_pending_dates_h_HEADER_GUARD_

#include "pending-operation.h"
#include "types.h"

#include <TelepathyLoggerQt_export.h>

namespace Tpl
{

/**
 * \headerfile pending-dates.h <TelepathyLoggerQt/PendingDates>
 * \brief An operation for retrieving logged dates.
 */
class TELEPATHY_LOGGER_QT_EXPORT PendingDates : public Tpl::PendingOperation
{
    Q_OBJECT
    Q_DISABLE_COPY(PendingDates);

public:
    /**
     * \brief Destructor.
     */
    ~PendingDates();

    /**
     * \brief Returns list of all dates retrieved from the log store.
     *
     * It's an error to call this method before the finished() signal is emitted.
     */
    QDateList dates() const;

    /**
     * \brief Returns account that has been queried
     */
    const Tp::AccountPtr account() const;

    /**
     * \brief Returns entity that has been queried.
     */
    const Tpl::EntityPtr entity() const;

private Q_SLOTS:
    virtual void start();

private:
    friend class LogManager;

    PendingDates(const LogManagerPtr & manager, const Tp::AccountPtr & account,
                 const EntityPtr & entity, EventTypeMask typeMask);

    struct Private;
    friend struct Private;
    Private *mPriv;
};

} // Tpl

#endif
