/*
 * This file is part of TelepathyQt4Logger
 *
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
#ifndef _TelepathyQt4Logger_global_h_HEADER_GUARD_
#define _TelepathyQt4Logger_global_h_HEADER_GUARD_

#ifndef IN_TELEPATHY_QT4_LOGGER_HEADER
#error IN_TELEPATHY_QT4_LOGGER_HEADER
#endif

#include <QGlib/Global>

/* defined by cmake when building this library */
#if defined(BUILDING_TELEPATHY_QT4_LOGGER)
# define TELEPATHY_QT4_LOGGER_EXPORT Q_DECL_EXPORT
#else
# define TELEPATHY_QT4_LOGGER_EXPORT Q_DECL_IMPORT
#endif

#if !defined(Q_OS_WIN) && !defined(Q_CC_NOKIAX86) && \
    !defined(Q_CC_RVCT) && defined(QT_VISIBILITY_AVAILABLE)
# define TELEPATHY_QT4_LOGGER_NO_EXPORT __attribute__((visibility("hidden")))
#else
# define TELEPATHY_QT4_LOGGER_NO_EXPORT
#endif

#define QTELEPATHYQT4LOGGER_REGISTER_TYPE(T) \
    QGLIB_REGISTER_TYPE_WITH_EXPORT_MACRO(T, TELEPATHY_QT4_LOGGER_EXPORT)


#define QTELEPATHYQT4LOGGER_WRAPPER_TPLCLASS_DECLARATION(Class) \
    typedef struct _Tpl##Class Tpl##Class;

#define QTELEPATHYQT4LOGGER_WRAPPER_REFPOINTER_DECLARATION(Class) \
    namespace Tpl { \
        class Class; \
        typedef QGlib::RefPointer<Class> Class##Ptr; \
    }

#define QTELEPATHYQT4LOGGER_WRAPPER_DECLARATION(Class) \
    QTELEPATHYQT4LOGGER_WRAPPER_TPLCLASS_DECLARATION(Class) \
    QTELEPATHYQT4LOGGER_WRAPPER_REFPOINTER_DECLARATION(Class)

QTELEPATHYQT4LOGGER_WRAPPER_DECLARATION(CallEvent)
QTELEPATHYQT4LOGGER_WRAPPER_DECLARATION(Entity)
QTELEPATHYQT4LOGGER_WRAPPER_DECLARATION(Event)
QTELEPATHYQT4LOGGER_WRAPPER_DECLARATION(TextEvent)
QTELEPATHYQT4LOGGER_WRAPPER_DECLARATION(LogManager)

#undef QTELEPATHYQT4LOGGER_WRAPPER_DECLARATION
#undef QTELEPATHYQT4LOGGER_WRAPPER_REFPOINTER_DECLARATION
#undef QTELEPATHYQT4LOGGER_WRAPPER_TPLCLASS_DECLARATION


#define QTELEPATHYQT4LOGGER_WRAPPER(Class) \
    QGLIB_WRAPPER_DECLARATION_MACRO(Class, Class, Tpl, Class)


#endif
