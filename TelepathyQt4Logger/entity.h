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
#ifndef _TelepathyLoggerQt4_entity_h_HEADER_GUARD_
#define _TelepathyLoggerQt4_entity_h_HEADER_GUARD_

#ifndef IN_TELEPATHY_LOGGER_QT4_HEADER
#error IN_TELEPATHY_LOGGER_QT4_HEADER
#endif

#include <QGlib/Object>
#include <TelepathyLoggerQt4/Types>

namespace Tpl
{

/*! \headerfile entity.h <TelepathyLoggerQt4/Entity>
 * \brief Wrapper class for TplEntity
 */
class TELEPATHY_LOGGER_QT4_EXPORT Entity : public QGlib::Object
{
    QTELEPATHYLOGGERQT4_WRAPPER(Entity)
public:
    static EntityPtr create(const char *id, EntityType type, const char *alias, const char *avatarToken);
    static EntityPtr create(const Tp::ContactPtr & contact, EntityType type);
    static EntityPtr create(const char *room_id);

    QString alias() const;
    QString identifier() const;
    EntityType entityType() const;
    QString avatarToken() const;
};

} //namespace

QTELEPATHYLOGGERQT4_REGISTER_TYPE(Tpl::Entity)

#endif
