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

#include <Logger/tpl-correspondant.h>

#include <tpl-correspondant-private.h>

using namespace Logger;

Correspondant::Correspondant() : d(0)
{
}

Correspondant::~Correspondant()
{
    if (this->d) {
        delete this->d;
    }
}

QString Correspondant::alias() const
{
    return this->d->alias();
}

QString Correspondant::id() const
{
    return this->d->id();
}

QString Correspondant::avatar() const
{
    return this->d->avatar();
}

Correspondant::Whois Correspondant::type() const
{
    return this->d->type();
}