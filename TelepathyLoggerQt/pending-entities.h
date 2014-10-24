/*
 * This file is part of TelepathyLoggerQt
 *
 * Copyright (C) 2011 Collabora Ltd. <http://www.collabora.co.uk/>
 * Copyright (C) 2012 David Edmundson <kde@davidedmundson.co.uk>
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
#ifndef _TelepathyLoggerQt_pending_entities_h_HEADER_GUARD_
#define _TelepathyLoggerQt_pending_entities_h_HEADER_GUARD_

#ifndef IN_TELEPATHY_LOGGER_QT_HEADER
#error IN_TELEPATHY_LOGGER_QT_HEADER
#endif

#include <TelepathyLoggerQt/PendingOperation>
#include <TelepathyLoggerQt/Types>

namespace Tpl
{

/**
 * \headerfile pending-entities.h <TelepathyLoggerQt/PendingEntities>
 * \brief An operation for retrieving entities for which logs exist.
 */
class TELEPATHY_LOGGER_QT_EXPORT PendingEntities: public Tpl::PendingOperation
{
    Q_OBJECT
    Q_DISABLE_COPY(PendingEntities);

public:
    /**
     * \brief Destructor.
     */
    ~PendingEntities();

    /**
     * \brief Returns list of entities retrieved from logger store.
     *
     * It's an error to call this method before the finished() signal is emitted.
     */
    EntityPtrList entities() const;

    /**
     * \brief Returns account that has been queried.
     */
    Tp::AccountPtr account() const;

private Q_SLOTS:
    virtual void start();

private:
    friend class LogManager;

    PendingEntities(const LogManagerPtr & manager, const Tp::AccountPtr & account);

    struct Private;
    friend struct Private;
    Private *mPriv;
};

} // Tpl

#endif
