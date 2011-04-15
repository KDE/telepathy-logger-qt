/*
 * This file is part of TelepathyLoggerQt4
 *
 * Copyright (C) 2011 Collabora Ltd. <http://www.collabora.co.uk/>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef _TelepathyLoggerQt4_Models_global_h_HEADER_GUARD_
#define _TelepathyLoggerQt4_Models_global_h_HEADER_GUARD_

#ifndef IN_TELEPATHY_LOGGER_QT4_MODELS_HEADER
#error IN_TELEPATHY_LOGGER_QT4_MODELS_HEADER
#endif

#include <QtCore/QtGlobal>

#if defined(BUILDING_TELEPATHY_LOGGER_QT4_MODELS)
# define TELEPATHY_LOGGER_QT4_MODELS_EXPORT Q_DECL_EXPORT
#else
# define TELEPATHY_LOGGER_QT4_MODELS_EXPORT Q_DECL_IMPORT
#endif

#if !defined(Q_OS_WIN) && !defined(Q_CC_NOKIAX86) && \
    !defined(Q_CC_RVCT) && defined(QT_VISIBILITY_AVAILABLE)
# define TELEPATHY_LOGGER_QT4_MODELS_NO_EXPORT __attribute__((visibility("hidden")))
#else
# define TELEPATHY_LOGGER_QT4_MODELS_NO_EXPORT
#endif

#endif