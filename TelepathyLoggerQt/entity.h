/*
 * This file is part of TelepathyLoggerQt
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
#ifndef _TelepathyLoggerQt_entity_h_HEADER_GUARD_
#define _TelepathyLoggerQt_entity_h_HEADER_GUARD_

#include "object.h"
#include "types.h"
#include <TelepathyLoggerQt_export.h>

namespace Tpl
{

/**
 * \headerfile entity.h <TelepathyLoggerQt/Entity>
 * \brief An object representing a contact or room.
 */
class TELEPATHY_LOGGER_QT_EXPORT Entity : public Tpl::Object
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
    QTELEPATHYLOGGERQT_WRAPPER(Entity)
};

} //namespace

#endif
