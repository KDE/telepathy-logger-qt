#ifndef __ENTITY__
#define __ENTITY__

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

#include <QGlib/Object>
#include "global.h"
#include <TelepathyQt4/Types>
#include <QTpLogger/Types>

namespace QTpLogger
{

/*! \headerfile entity.h <QTpLogger/Entity>
 * \brief Wrapper class for TplEntity
 */
class Entity : public QGlib::Object
{
    QTPLOGGER_WRAPPER(Entity)
public:
    static EntityPtr create(const char *id, EntityType type, const char *alias, const char *avatarToken);
    static EntityPtr create(Tp::ContactPtr contact, EntityType type);
    static EntityPtr create(const char *room_id);

    QString alias() const;
    QString identifier() const;
    EntityType entityType() const;
    QString avatarToken() const;
};

} //namespace

QTPLOGGER_REGISTER_TYPE(QTpLogger::Entity)

#endif // __ENTITY__
