/*
 * This file is part of TelepathyLoggerQt4
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
#ifndef _TelepathyLoggerQt4_search_hit_h_HEADER_GUARD_
#define _TelepathyLoggerQt4_search_hit_h_HEADER_GUARD_

#ifndef IN_TELEPATHY_LOGGER_QT4_HEADER
#error IN_TELEPATHY_LOGGER_QT4_HEADER
#endif

#include <TelepathyQt/Types>
#include <TelepathyLoggerQt4/PendingOperation>
#include <TelepathyLoggerQt4/Entity>

#include <QtCore/QSharedDataPointer>

namespace Tpl
{


/**
 * \headerfile search-hit.h <TelepathyLoggerQt4/SearchHit>
 * \brief A single search match.
 */
class TELEPATHY_LOGGER_QT4_EXPORT SearchHit
{
public:
    /**
     * \brief Constructs a new search hit
     * \param account
     * \param target
     * \param date
     */
    SearchHit(const Tp::AccountPtr &account, const EntityPtr &target, const QDate &date);

    /**
     * \brief Copy constructor.
     * \param other
     */
    SearchHit(const SearchHit &other);

    /**
     * \brief Assignment operator.
     */
    SearchHit & operator=(const SearchHit &other);

    /**
     * \brief Destructor.
     */
    ~SearchHit();

    /**
     * \brief Returns an account that the matching event is associated with.
     */
    Tp::AccountPtr account() const;

    /**
     * \brief Returns an entity that the matching event is associated with.
     */
    EntityPtr target() const;

    /**
     * \brief Returns an entity that the matching event is associated with.
     */
    QDate date() const;

private:
    struct Private;
    QSharedDataPointer<Private> mPriv;
};

} // Tpl

#endif
