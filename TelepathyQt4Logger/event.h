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
#ifndef _TelepathyQt4Logger_event_h_HEADER_GUARD_
#define _TelepathyQt4Logger_event_h_HEADER_GUARD_

#ifndef IN_TELEPATHY_QT4_LOGGER_HEADER
#error IN_TELEPATHY_QT4_LOGGER_HEADER
#endif

#include <QGlib/Object>
#include <TelepathyQt4Logger/Entity>
#include <TelepathyQt4Logger/Types>
#include <QtCore/QString>

namespace Tpl
{

/*! \headerfile event.h <TelepathyQt4Logger/Event>
 * \brief Wrapper class for TplEvent
 */
class Event : public QGlib::Object
{
    QTELEPATHYQT4LOGGER_WRAPPER(Event)
public:
    QDateTime timestamp() const;
    QString accountPath() const;
    Tp::AccountPtr account() const;
    EntityPtr sender() const;
    EntityPtr receiver() const;
    bool operator==(Event& rhs) const;
};

} //namespace

QTELEPATHYQT4LOGGER_REGISTER_TYPE(Tpl::Event)

#endif
