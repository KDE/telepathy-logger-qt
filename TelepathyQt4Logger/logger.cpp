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

#include <TelepathyQt4Logger/Logger>
#include <TelepathyQt4Logger/PendingLogger>
#include <TelepathyQt4Logger/_gen/logger.moc.hpp>
#include <TelepathyQt4Logger/_gen/cli-logger-body.hpp>
#include <TelepathyQt4Logger/_gen/cli-logger.moc.hpp>

#include <QDBusPendingReply>
#include <QDebug>

namespace Tpl {

typedef Tp::SharedPtr<Logger> LoggerPtr;

Logger::Logger() :
    Tp::StatelessDBusProxy(QDBusConnection::sessionBus(), 
        QLatin1String(TPL_DBUS_SRV_WELL_KNOWN_BUS_NAME), 
	QLatin1String(TPL_DBUS_SRV_OBJECT_PATH), 
	Tp::Feature()),
    mInterface(0)
{
    mInterface = new Tpl::LoggerInterface(QDBusConnection::sessionBus(),
        TPL_DBUS_SRV_WELL_KNOWN_BUS_NAME, TPL_DBUS_SRV_OBJECT_PATH);
    mPtr = LoggerPtr(this);
}

Logger::~Logger()
{
}

Tp::PendingOperation *Logger::clearLog()
{
    PendingLogger *operation = new PendingLogger(mPtr, mInterface);
    operation->clearLog();
    return operation;
}

Tp::PendingOperation *Logger::clearAccount(const Tp::AccountPtr &account)
{
    PendingLogger *operation = new PendingLogger(mPtr, mInterface);
    operation->clearAccount(account);
    return operation;
}

Tp::PendingOperation *Logger::clearContact(const Tp::AccountPtr &account, const QString &objectId)
{
    PendingLogger *operation = new PendingLogger(mPtr, mInterface);
    operation->clearContact(account, objectId);
    return operation;
}

Tp::PendingOperation *Logger::clearRoom(const Tp::AccountPtr &account, const QString &objectId)
{
    PendingLogger *operation = new PendingLogger(mPtr, mInterface);
    operation->clearRoom(account, objectId);
    return operation;
}

}
