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

#include "entity.h"
#include <telepathy-logger/entity.h>
#include <TelepathyQt4/Account>

using namespace QTpLogger;

EntityPtr Entity::create(const char *id, EntityType type, const char *alias, const char *avatarToken)
{
    TplEntity *entity = tpl_entity_new(id, (TplEntityType) type, alias, avatarToken);
    return EntityPtr::wrap(entity, false);
}

EntityPtr Entity::create(Tp::ContactPtr contact, EntityType type)
{
    // TODO how to go from Tp::ContactPtr to TpContact
    TplEntity *entity = tpl_entity_new_from_tp_contact(0, (TplEntityType) type);
    return EntityPtr::wrap(entity, false);
}

EntityPtr Entity::create(const char *room_id)
{
    TplEntity *entity = tpl_entity_new_from_room_id(room_id);
    return EntityPtr::wrap(entity, false);
}

QString Entity::alias() const
{
    const gchar *s = tpl_entity_get_alias(object<TplEntity>());
    QString str = QString::fromUtf8(s);
    g_free((gpointer) s);
    return str;
}

QString Entity::identifier() const
{
    const gchar *s = tpl_entity_get_identifier(object<TplEntity>());
    QString str = QString::fromUtf8(s);
    g_free((gpointer) s);
    return str;
}

EntityType Entity::entityType() const
{
    TplEntityType entityType = tpl_entity_get_entity_type(object<TplEntity>());
    return EntityType(entityType);
}

QString Entity::avatarToken() const
{
    const gchar *s = tpl_entity_get_avatar_token(object<TplEntity>());
    QString str = QString::fromUtf8(s);
    g_free((gpointer) s);
    return str;
}
