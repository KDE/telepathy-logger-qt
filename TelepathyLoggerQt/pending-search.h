/*
 * This file is part of TelepathyLoggerQt
 *
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
#ifndef _TelepathyLoggerQt_pending_search_h_HEADER_GUARD_
#define _TelepathyLoggerQt_pending_search_h_HEADER_GUARD_

#ifndef IN_TELEPATHY_LOGGER_QT_HEADER
#error IN_TELEPATHY_LOGGER_QT_HEADER
#endif

#include <TelepathyLoggerQt/PendingOperation>
#include <TelepathyLoggerQt/Entity>

namespace Tpl
{

/**
 * \headerfile pending-search.h <TelepathyLoggerQt/PendingSearch>
 * \brief An operation for retrieving search results.
 */
class TELEPATHY_LOGGER_QT_EXPORT PendingSearch : public Tpl::PendingOperation
{
    Q_OBJECT
    Q_DISABLE_COPY(PendingSearch);

public:
    /**
     * \brief Destructor.
     */
    ~PendingSearch();

    /**
     * \brief Returns list of search hits.
     *
     * It's an error to call this method before the finished() signal is amitted.
     */
    SearchHitList hits() const;

private Q_SLOTS:
    virtual void start();

private:
    friend class LogManager;

    PendingSearch(const LogManagerPtr & manager, const QString &text, EventTypeMask typeMask);

    struct Private;
    friend struct Private;
    Private *mPriv;
};

} // Tpl

#endif
