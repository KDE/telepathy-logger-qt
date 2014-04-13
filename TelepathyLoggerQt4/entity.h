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

#include <TelepathyLoggerQt4/Object>
#include <TelepathyLoggerQt4/Types>

namespace Tpl
{

/**
 * \headerfile entity.h <TelepathyLoggerQt4/Entity>
 * \brief An object representing a contact or room.
 */
class TELEPATHY_LOGGER_QT4_EXPORT Entity: public Object
{
public:
    /**
     * \brief Constructs a new entity
     *
     * \param id
     * \param type
     * \param alias
     * \param avatarToken
     */
    static EntityPtr create(const char *id, EntityType type, const char *alias,
                            const char *avatarToken);

    /**
     * \brief Constructs a new entity from Tp::Contact
     *
     * \param contact
     * \param type
     */
    static EntityPtr create(const Tp::ContactPtr & contact, EntityType type);

    /**
     * \brief Constructs a new for a room
     *
     * \param room_id
     */
    static EntityPtr create(const char *room_id);

    /**
     * \brief Returns entity's alias
     */
    QString alias() const;

    /**
     * \brief Returns entity's unique identifier
     */
    QString identifier() const;

    /**
     * \brief Returns whether the entity is a contact, a room or account owner
     */
    EntityType entityType() const;

    /**
     * \brief Returns entity's avatar token
     */
    QString avatarToken() const;

private:
    QTELEPATHYLOGGERQT4_WRAPPER(Entity)
};

} //namespace

#endif
