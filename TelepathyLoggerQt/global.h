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

#include <QSharedPointer>
#include <glib-object.h>

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

class TPLoggerQtWrapper {
  public:
    template<typename TplClass, typename TplQtClass>
    static QSharedPointer<TplQtClass> wrap(TplClass *tplObj, bool ref = false) {
        return QSharedPointer<TplQtClass>(new TplQtClass(tplObj, ref));
    }
    template<typename TplClass, typename TplQtClass>
    static TplQtClass* wrap2(TplClass *tplObj, bool ref = false) {
        return new TplQtClass(tplObj, ref);
    }

    template<typename TplClass, typename TplQtClass>
    static TplClass* unwrap(const QSharedPointer<TplQtClass> &obj) {
        return reinterpret_cast<TplClass*>(obj->m_tplPtr);
    }
    template<typename TplClass, typename TplQtClass>
    static TplClass* unwrap(TplQtClass *obj) {
        return reinterpret_cast<TplClass*>(obj->m_tplPtr);
    }
};

#define QTELEPATHYLOGGERQT_WRAPPER_IMPL_BASECLASS(Class) \
  protected: \
    friend class ::TPLoggerQtWrapper; \
    Class(struct _Tpl##Class *tplPtr, bool ref): \
        Object(tplPtr, ref) \
    { }

#define QTELEPATHYLOGGERQT_WRAPPER_IMPL_SUBCLASSED(Class, Superclass) \
  friend class ::TPLoggerQtWrapper; \
  private: \
    Class(struct _Tpl##Class *tplPtr, bool ref): \
        Superclass(reinterpret_cast<struct _Tpl##Superclass*>(tplPtr), ref) \
    { }


#define QTELEPATHYLOGGERQT_WRAPPER_GET_IMPL(_1, _2, NAME, ...) NAME

#define QTELEPATHYLOGGERQT_WRAPPER(...) \
    QTELEPATHYLOGGERQT_WRAPPER_GET_IMPL( \
        __VA_ARGS__, \
        QTELEPATHYLOGGERQT_WRAPPER_IMPL_SUBCLASSED, \
        QTELEPATHYLOGGERQT_WRAPPER_IMPL_BASECLASS)(__VA_ARGS__)

#endif
