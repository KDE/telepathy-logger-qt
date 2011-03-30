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

#include <TelepathyQt4Logger/LoggerManager>

#include <TelepathyQt4Logger/Logger>
#include <TelepathyQt4Logger/_gen/cli-logger.h>
#include <QDBusPendingReply>
#include <QDebug>

namespace Tpl {

LoggerManager::LoggerManager()
{

}

LoggerManager::~LoggerManager()
{

}

void LoggerManager::clearLog()
{
    Tpl::LoggerInterface *logger = new Tpl::LoggerInterface(QDBusConnection::sessionBus(),
                                                       QLatin1String("org.freedesktop.Telepathy.Logger.DRAFT"), QString(""));

    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(
                logger->Clear());
    connect(watcher, SIGNAL(finished(QDBusPendingCallWatcher*)),
            SLOT(onLogCleared(QDBusPendingCallWatcher*)));
}

void LoggerManager::onLogCleared(QDBusPendingCallWatcher *watcher)
{
    QDBusPendingReply<> reply = *watcher;

    if (!reply.isError()) {
        qDebug() << "Log has been cleared";
        Q_EMIT logCleared();
    } else {
        qWarning().nospace() << "Clear log failed with " <<
            reply.error().name() << ":" << reply.error().message();
    }
}

}
