/*
 * This file is part of TelepathyLoggerQt4
 *
 * Copyright (C) 2014 Daniel Vrátil <dvratil@redhat.com>
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

#include "object.h"

#include <glib-object.h>

namespace Tpl
{

Object::Object(void *tplPtr, bool ref):
    m_tplPtr(tplPtr)
{
    if (ref) {
        g_object_ref(G_OBJECT(m_tplPtr));
    }
}

Object::~Object()
{
    g_object_unref(G_OBJECT(m_tplPtr));
}

}
