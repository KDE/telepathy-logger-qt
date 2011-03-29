/*
 * This file is part of TelepathyQt4Logger
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
#ifndef _TelepathyQt4Logger_log_manager_h_HEADER_GUARD_
#define _TelepathyQt4Logger_log_manager_h_HEADER_GUARD_

#ifndef IN_TELEPATHY_QT4_LOGGER_HEADER
#error IN_TELEPATHY_QT4_LOGGER_HEADER
#endif

#include <QGlib/Object>
#include <TelepathyQt4Logger/Types>

namespace Tpl
{

/*! \headerfile log-manager.h <TelepathyQt4Logger/LogManager>
 * \brief Wrapper class for TplLogManager
 */
class LogManager : public QGlib::Object
{
    QTELEPATHYQT4LOGGER_WRAPPER(LogManager)
public:
    static LogManagerPtr instance();

    Tp::AccountManagerPtr accountManagerPtr() const;
    void setAccountManagerPtr(const Tp::AccountManagerPtr & accountManager);

    bool exists(const Tp::AccountPtr & account, const EntityPtr & target, EventTypeMask type) const;

    PendingDates *queryDates(const Tp::AccountPtr & account, const EntityPtr & entity,
        EventTypeMask typeMask);
    PendingEvents *queryEvents(const Tp::AccountPtr & account, const EntityPtr & entity,
        EventTypeMask typeMask, const QDate & date);
    PendingEvents *queryFilteredEvents(const Tp::AccountPtr & account, const EntityPtr & entity,
        EventTypeMask typeMask, uint numEvents, LogEventFilter filterFunction,
        void *filterFunctionUserData);
    PendingEntities *queryEntities(const Tp::AccountPtr & account);
    PendingSearch *search(const QString &text, EventTypeMask typeMask);
};

} //namespace

QTELEPATHYQT4LOGGER_REGISTER_TYPE(Tpl::LogManager)

#endif
