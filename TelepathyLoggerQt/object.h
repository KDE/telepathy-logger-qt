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
#ifndef _TelepathyLoggerQt_object_h_HEADER_GUARD_
#define _TelepathyLoggerQt_object_h_HEADER_GUARD_

#include "types.h"

#include <TelepathyLoggerQt_export.h>

namespace Tpl
{

class TELEPATHY_LOGGER_QT_EXPORT Object
{
public:
    explicit Object(void *tplPtr, bool ref = false);

    virtual ~Object();

protected:
    template<typename T>
    T* object() const {
        return reinterpret_cast<T*>(m_tplPtr);
    }

private:
    friend class ::TPLoggerQtWrapper;
    void *m_tplPtr;
};

} //namespace

#endif
