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

#ifndef _TelepathyQt4Logger_logger_manager_h_HEADER_GUARD_
#define _TelepathyQt4Logger_logger_manager_h_HEADER_GUARD_

#include <TelepathyQt4Logger/Types>
#include <QDBusPendingCallWatcher>
#include <QObject>

namespace Tpl
{
class LoggerManager : public QObject
{
    Q_OBJECT

public:
    LoggerManager();
    ~LoggerManager();

    void clearLog();

Q_SIGNALS:
    void logCleared();

private Q_SLOTS:
    void onLogCleared(QDBusPendingCallWatcher *watcher);

};

}


#endif
