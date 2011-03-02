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

#include "tpl-entity.h"
#include "tpl-entity-private.h"

using namespace QTpLogger;

Entity::Entity() : d(0)
{
}

Entity::~Entity()
{
    if (this->d) {
        delete this->d;
    }
}

QString Entity::alias() const
{
    return this->d->alias();
}

QString Entity::id() const
{
    return this->d->id();
}

QString Entity::avatar() const
{
    return this->d->avatar();
}

Entity::Whois Entity::type() const
{
    return this->d->type();
}
