#ifndef __EVENT__
#define __EVENT__

/*
 * Copyright (C) 2011 Stefano Sanfilippo <stefano.k.sanfilippo@gmail.com>
 * Copyright (C) 2011 Collabora Ltd. <http://www.collabora.co.uk/>
 *
 * @author Stefano Sanfilippo <stefano.k.sanfilippo@gmail.com>
 * @author Mateu Batle <mateu.batle@collabora.co.uk>
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

#include "global.h"
#include <QGlib/Object>
#include <QTpLogger/Entity>
#include <QtCore/QString>
#include <TelepathyQt4/Types>

namespace QTpLogger
{

/*! \headerfile event.h <QTpLogger/Event>
 * \brief Wrapper class for TplEvent
 */
class Event : public QGlib::Object
{
    QTPLOGGER_WRAPPER(Event)
public:
    // GType tpl_event_get_type (void);
    QDateTime timestamp() const;
    QString accountPath() const;
    Tp::AccountPtr account() const;
    EntityPtr sender() const;
    EntityPtr receiver() const;
    bool operator==(Event& rhs) const;
};

} //namespace

QTPLOGGER_REGISTER_TYPE(QTpLogger::Event)

#endif // __EVENT__
