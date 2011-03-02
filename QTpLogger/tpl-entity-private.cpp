/*
 * Copyright (C) 2011 Stefano Sanfilippo <stefano.k.sanfilippo@gmail.com>
 *
 * @author Stefano Sanfilippo <stefano.k.sanfilippo@gmail.com>
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

#include <QTpLogger/tpl-entity.h>

#include <tpl-entity-private.h>

using namespace QTpLogger;

bool EntityPrivate::first_object = true;

QHash<TplEntityType, Entity::Whois>
  EntityPrivate::tplToCorrespondantWhoisHash;

EntityPrivate::EntityPrivate(TplEntity *chat)
{
    if (this->first_object)
    {
		tplToCorrespondantWhoisHash[TPL_ENTITY_UNKNOWN] = Entity::undefined;
		tplToCorrespondantWhoisHash[TPL_ENTITY_CONTACT] = Entity::contact;
		tplToCorrespondantWhoisHash[TPL_ENTITY_GROUP] = Entity::group;
		tplToCorrespondantWhoisHash[TPL_ENTITY_SELF] = Entity::self;

        this->first_object = false;
    }

    gchar *galias;
    gchar *gid;
    gchar *gavatar;
    TplEntityType gentitytype;

    g_object_get(chat,
                 "alias", &galias, "identifier", &gid,
                 "avatar-token", &gavatar, "entity-type", &gentitytype, NULL);

    this->_alias = QString(galias);
    this->_id = QString(gid);
    this->_avatar = QString(gavatar);
    this->_type = tplToCorrespondantWhoisHash[gentitytype];

    g_free(galias);
    g_free(gid);
    g_free(gavatar);
}

EntityPrivate::~EntityPrivate()
{
}

QString EntityPrivate::alias() const
{
    return this->_alias;
}

QString EntityPrivate::id() const
{
    return this->_id;
}

QString EntityPrivate::avatar() const
{
    return this->_avatar;
}

Entity::Whois EntityPrivate::type() const
{
    return this->_type;
}
