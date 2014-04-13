/*
 * This file is part of TelepathyLoggerQt4
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
#ifndef _TelepathyLoggerQt4_event_h_HEADER_GUARD_
#define _TelepathyLoggerQt4_event_h_HEADER_GUARD_

#ifndef IN_TELEPATHY_LOGGER_QT4_HEADER
#error IN_TELEPATHY_LOGGER_QT4_HEADER
#endif

#include <TelepathyLoggerQt4/Object>

namespace Tpl
{

/*!
 * \headerfile event.h <TelepathyLoggerQt4/Event>
 * \brief The TPLogger log event represents a generic log event, which will be
 * specialized by subclasses.
 */
class TELEPATHY_LOGGER_QT4_EXPORT Event: public Object
{
public:

    /*!
     * \brief Returns a timestamp when the event has been logged.
     */
    QDateTime timestamp() const;

    /*!
     *\brief Returns account path of the account the event is associated to.
     */
    QString accountPath() const;

    /*!
     * \brief Returns the account the event is associated to.
     */
    Tp::AccountPtr account() const;

    /*!
     * \brief Returns the entity that originated the log event.
     */
    EntityPtr sender() const;

    /*!
     * \brief Returns the entity thas was destination for the log event.
     */
    EntityPtr receiver() const;

    /*!
     * \brief Equivalent to operator==().
     */
    bool equalTo(const EventPtr & rhs) const;

    /*!
     * \brief Comparison operator.
     */
    bool operator==(const EventPtr & rhs ) const;

private:
    QTELEPATHYLOGGERQT4_WRAPPER(Event)
};

} //namespace

#endif
