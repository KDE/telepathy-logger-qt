/*
 * This file is part of TelepathyLoggerQt
 *
 * Copyright (C) 2011 Stefano Sanfilippo <stefano.k.sanfilippo@gmail.com>
 * Copyright (C) 2011 Collabora Ltd. <http://www.collabora.co.uk/>
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
#ifndef _TelepathyLoggerQt_log_manager_h_HEADER_GUARD_
#define _TelepathyLoggerQt_log_manager_h_HEADER_GUARD_

#ifndef IN_TELEPATHY_LOGGER_QT_HEADER
#error IN_TELEPATHY_LOGGER_QT_HEADER
#endif

#ifndef TPLQT_TPL_IS_VERSION
#define TPLQT_TPL_IS_VERSION(maj, min, patch) \
   (TPLQT_TPL_VERSION_MAJOR > maj) \
|| (TPLQT_TPL_VERSION_MAJOR == maj && TPLQT_TPL_VERSION_MINOR > min) \
|| (TPLQT_TPL_VERSION_MAJOR == maj && TPLQT_TPL_VERSION_MINOR == min && TPLQT_TPL_VERSION_PATCH >= patch)
#endif

#include <QGlib/Object>
#include <TelepathyLoggerQt/_gen/cli-logger.h>
#include <TelepathyLoggerQt/Types>

namespace Tpl
{

/*!
 * \headerfile log-manager.h <TelepathyLoggerQt/LogManager>
 * \brief The LogManager object allows user to fetch logs and make searches.
 */
class TELEPATHY_LOGGER_QT_EXPORT LogManager : public QGlib::Object
{
public:
    /*!
     * \brief Returns reference to a LogManager singleton
     */
    static LogManagerPtr instance();

    /*!
     * \brief Returns Tp::AccountManager assigned to the LogManager.
     */
    Tp::AccountManagerPtr accountManagerPtr() const;

    /*!
     * \brief Sets a new Tp::AccountManager to be used by the LogManager.
     *
     * The \p accountManager is expected to be in ready state.
     *
     * \param accountManager An Tp::AccountManager in the ready state.
     */
    void setAccountManagerPtr(const Tp::AccountManagerPtr & accountManager);

    /*!
     * \brief Checks if logs exist for \p target.
     *
     * It applies for any registered readable log store.
     *
     * \param account An account to which the events should be associated
     * \param target Target to query
     * \param type Type of the events to query
     * \returns true when logs exist for \p target, otherwise false.
     */
    bool exists(const Tp::AccountPtr & account, const EntityPtr & target, EventTypeMask type) const;

    /*!
     * \brief Retrieves a list of dates corresponding to each day where
     * at least one event exist for \p target.
     *
     * \param account An account associated with the events
     * \param entity An entity to query
     * \param typeMask Type of events to query
     * \return Returns a PendingOperation that will emit finished() when
     * events were retrieved or an error occurs.
     */
    PendingDates *queryDates(const Tp::AccountPtr & account, const EntityPtr & entity,
        EventTypeMask typeMask);

    /*!
     * \brief Retrieves a list of events on given \p date for \p target.
     *
     * \param account An account associated with the events
     * \param entity An entity to query
     * \param typeMask Type of events to query
     * \param date A date for which to retrieve events
     * \return Returns a PendingOperation that will emit finished() when
     * events were retrieved or an error occurs.
     */
    PendingEvents *queryEvents(const Tp::AccountPtr & account, const EntityPtr & entity,
        EventTypeMask typeMask, const QDate & date);

    /*!
     * \brief Retrieves a list of events matching given filter.
     *
     * \param account An account associated with the events
     * \param entity An entity to query
     * \param typeMask Type of events to query
     * \param numEvents Maximum amount of events to fetch
     * \param filterFunction A filter function that will be called for every event to
     * check whether it matches the filter or not.
     * \param filterFunctionUserData An arbitrary user data to be passed to the \p filterFunction
     * with every call.
     * \return Returns a PendingOperation that will emit finished() when
     * events were retrieved has been rewound or an error occurs.
     */
    PendingEvents *queryFilteredEvents(const Tp::AccountPtr & account, const EntityPtr & entity,
        EventTypeMask typeMask, uint numEvents, LogEventFilter filterFunction,
        void *filterFunctionUserData = 0);

    /**
     * \brief Retrieves a list of entities associated with given \p account.
     *
     * \param account
     * \return Returns a PendingOperation that will emit finished() when
     * entities were retrieved or an error occurs.
     */
    PendingEntities *queryEntities(const Tp::AccountPtr & account);

    /**
     * \brief Retrieves a list of events matching given full-text filter
     *
     * \param text A pattern to search
     * \param typeMask Type of events to match
     * \return Returns a PendingOperation that will emit finished() when
     * entities were retrieved or an error occurs.
     */
    PendingSearch *search(const QString &text, EventTypeMask typeMask);

    /**
     * \brief Irreversibly deletes all logs.
     *
     * \return Returns a PendingOperation that will emit finished() when
     * all logs were deleted or an error occurs.
     */
    PendingOperation *clearHistory();

    /**
     * \brief Irreversibly deletes all logs associated with given \p account
     *
     * \param account
     * \return Returns a PendingOperation that will emit finished() when
     * logs were deleted or an error occurs.
     */
    PendingOperation *clearAccountHistory(const Tp::AccountPtr &account);

    /**
     * \brief Irreversibly delets all logs associated with given \p account and
     * \p entity.
     *
     * \param account
     * \param entity
     * \return Returns a PendingOperation that will emit finished() when
     * logs were deleted or an error occurs.
     */
    PendingOperation *clearEntityHistory(const Tp::AccountPtr &account, const Tpl::EntityPtr &entity);

    /**
     * \brief Creates a LogWalker to traverse all the events exchanged with \p target
     *
     * \param account An account associated with the events
     * \param entity An entity associated with the events
     * \param typeMask Type of events to query
     * \param filterFunction A filter function that will be called for every event to
     * check whether it matches the filter or not.
     * \param filterFunctionUserData An arbitrary user data to be passed to the
     * \p filterFunction with every call.
     * \return A new LogWalker.
     * \since 0.8.0
     */
    Tpl::LogWalkerPtr queryWalkFilteredEvents(const Tp::AccountPtr &account,
        const Tpl::EntityPtr &entity, EventTypeMask typeMask, LogEventFilter filterFunction,
        void *filterFunctionUserData = 0);

#if TPLQT_TPL_IS_VERSION(0, 9, 0)
    /**
     * \brief Disables logging for given \p account and \p entity
     *
     * \param account
     * \param entity
     *
     * \return true if operation succeeds
     */
    bool disableForEntity(const Tp::AccountPtr &account, const EntityPtr &entity);

    /**
     * \brief Enables logging for given \p account and \p entity
     *
     * \param account
     * \param entity
     *
     * \return true if operation succeeds
     */
    bool enableForEntity(const Tp::AccountPtr &account, const EntityPtr &entity);

    /**
     * \brief Checks if logging is disabled for given \p account and \p entity
     *
     * \param account
     * \param entity
     *
     * \return true if logging is disabled for given \p account and \p entity
     */
    bool isDisabledForEntity(const Tp::AccountPtr &account, const EntityPtr &entity);
#endif

private:
    QTELEPATHYLOGGERQT4_WRAPPER(LogManager)
};

} //namespace

QTELEPATHYLOGGERQT4_REGISTER_TYPE(Tpl::LogManager)

#endif
