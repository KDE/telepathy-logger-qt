/*
    Copyright (C) 2011 Collabora Ltd

    This library is free software; you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef QTPLOGGER_GLOBAL_H
#define QTPLOGGER_GLOBAL_H

#include <QGlib/type.h>
#include <QtCore/QtGlobal>
#include <QtCore/QDate>
#include <QtCore/QSharedPointer>

/* defined by cmake when building this library */
#if defined(QTpLogger_EXPORTS)
# define QTPLOGGER_EXPORT Q_DECL_EXPORT
#else
# define QTPLOGGER_EXPORT Q_DECL_IMPORT
#endif

#if !defined(Q_OS_WIN) && !defined(Q_CC_NOKIAX86) && \
    !defined(Q_CC_RVCT) && defined(QT_VISIBILITY_AVAILABLE)
# define QTPLOGGER_NO_EXPORT __attribute__((visibility("hidden")))
#else
# define QTPLOGGER_NO_EXPORT
#endif

#define QTPLOGGER_REGISTER_TYPE(T) \
    QGLIB_REGISTER_TYPE_WITH_EXPORT_MACRO(T, QTPLOGGER_EXPORT)

//cyclic dependency, must include after defining the above
//#include "enums.h"

//registered in value.cpp
QTPLOGGER_REGISTER_TYPE(QDate) //codegen: skip=true
QTPLOGGER_REGISTER_TYPE(QDateTime) //codegen: skip=true


#define QTPLOGGER_WRAPPER_QTPLOGGERCLASS_DECLARATION(Class) \
    typedef struct _Tpl##Class QTpLogger##Class;

#define QTPLOGGER_WRAPPER_DIFFERENT_QTPLOGGERCLASS_DECLARATION(Class, CClass) \
    typedef struct _Tpl##CClass QTpLogger##Class;

#define QTPLOGGER_WRAPPER_REFPOINTER_DECLARATION(Class) \
    namespace QTpLogger { \
        class Class; \
        typedef QGlib::RefPointer<Class> Class##Ptr; \
    }

#define QTPLOGGER_WRAPPER_DECLARATION(Class) \
    QTPLOGGER_WRAPPER_QTPLOGGERCLASS_DECLARATION(Class) \
    QTPLOGGER_WRAPPER_REFPOINTER_DECLARATION(Class)

QTPLOGGER_WRAPPER_DECLARATION(Entity)
QTPLOGGER_WRAPPER_DECLARATION(Event)
QTPLOGGER_WRAPPER_DECLARATION(TextEvent)
//QTPLOGGER_WRAPPER_DECLARATION(LogManager)

#undef QTPLOGGER_WRAPPER_DECLARATION
#undef QTPLOGGER_WRAPPER_REFPOINTER_DECLARATION
#undef QTPLOGGER_WRAPPER_QTPLOGGERCLASS_DECLARATION

#define QTPLOGGER_WRAPPER(Class) \
    QGLIB_WRAPPER_DECLARATION_MACRO(Class, Class, QTpLogger, Class)

#define QTPLOGGER_WRAPPER_DIFFERENT_C_CLASS(Class, CClass) \
    QGLIB_WRAPPER_DECLARATION_MACRO(Class, CClass, QTpLogger, Class)

#define QTPLOGGER_WRAPPER_FAKE_SUBCLASS(Sub, Class) \
    QGLIB_WRAPPER_DECLARATION_MACRO(Sub##Class, Class, QTpLogger, Class)

#define QTPLOGGER_REGISTER_SUBCLASS(BASECLASS, TYPE) \
    namespace QGlib { \
        template <> \
        struct GetTypeImpl<QTpLogger::TYPE##BASECLASS> { \
            inline operator Type() { return GetType<QTpLogger::BASECLASS>(); }; \
        }; \
    }

#endif // QTPLOGGER_GLOBAL_H
