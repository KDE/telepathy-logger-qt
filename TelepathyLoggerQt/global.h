/*
 * This file is part of TelepathyLoggerQt
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
#ifndef _TelepathyLoggerQt_global_h_HEADER_GUARD_
#define _TelepathyLoggerQt_global_h_HEADER_GUARD_

#ifndef IN_TELEPATHY_LOGGER_QT_HEADER
#error IN_TELEPATHY_LOGGER_QT_HEADER
#endif

#include <QGlib/Global>

/* defined by cmake when building this library */
#if defined(BUILDING_TELEPATHY_LOGGER_QT4)
# define TELEPATHY_LOGGER_QT_EXPORT Q_DECL_EXPORT
#else
# define TELEPATHY_LOGGER_QT_EXPORT Q_DECL_IMPORT
#endif

#if !defined(Q_OS_WIN) && !defined(Q_CC_NOKIAX86) && \
    !defined(Q_CC_RVCT) && defined(QT_VISIBILITY_AVAILABLE)
# define TELEPATHY_LOGGER_QT_NO_EXPORT __attribute__((visibility("hidden")))
#else
# define TELEPATHY_LOGGER_QT_NO_EXPORT
#endif

#define QTELEPATHYLOGGERQT4_REGISTER_TYPE(T) \
    QGLIB_REGISTER_TYPE_WITH_EXPORT_MACRO(T, TELEPATHY_LOGGER_QT_EXPORT)


#define QTELEPATHYLOGGERQT4_WRAPPER_TPLCLASS_DECLARATION(Class) \
    typedef struct _Tpl##Class Tpl##Class;

#define QTELEPATHYLOGGERQT4_WRAPPER_REFPOINTER_DECLARATION(Class) \
    namespace Tpl { \
        class Class; \
        typedef QGlib::RefPointer<Class> Class##Ptr; \
    }

#define QTELEPATHYLOGGERQT4_WRAPPER_DECLARATION(Class) \
    QTELEPATHYLOGGERQT4_WRAPPER_TPLCLASS_DECLARATION(Class) \
    QTELEPATHYLOGGERQT4_WRAPPER_REFPOINTER_DECLARATION(Class)

QTELEPATHYLOGGERQT4_WRAPPER_DECLARATION(CallEvent)
QTELEPATHYLOGGERQT4_WRAPPER_DECLARATION(Entity)
QTELEPATHYLOGGERQT4_WRAPPER_DECLARATION(Event)
QTELEPATHYLOGGERQT4_WRAPPER_DECLARATION(TextEvent)
QTELEPATHYLOGGERQT4_WRAPPER_DECLARATION(LogManager)
QTELEPATHYLOGGERQT4_WRAPPER_DECLARATION(LogWalker)

#undef QTELEPATHYLOGGERQT4_WRAPPER_DECLARATION
#undef QTELEPATHYLOGGERQT4_WRAPPER_REFPOINTER_DECLARATION
#undef QTELEPATHYLOGGERQT4_WRAPPER_TPLCLASS_DECLARATION


#define QTELEPATHYLOGGERQT4_WRAPPER(Class) \
    QGLIB_WRAPPER_DECLARATION_MACRO(Class, Class, Tpl, Class)


#endif
