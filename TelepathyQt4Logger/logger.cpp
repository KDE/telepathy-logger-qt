/*
 * This file is part of TelepathyLoggerQt4
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

#include <TelepathyLoggerQt4/Logger>

#include <TelepathyLoggerQt4/pending-logger.h>
#include <TelepathyLoggerQt4/_gen/logger.moc.hpp>
#include <TelepathyLoggerQt4/_gen/cli-logger-body.hpp>
#include <TelepathyLoggerQt4/_gen/cli-logger.moc.hpp>

#include <QDBusPendingReply>
#include <QDebug>

#define TPL_DBUS_SRV_WELL_KNOWN_BUS_NAME "org.freedesktop.Telepathy.Logger"
#define TPL_DBUS_SRV_OBJECT_PATH         "/org/freedesktop/Telepathy/Logger"

namespace Tpl {

struct TELEPATHY_LOGGER_QT4_NO_EXPORT Logger::Private
{
    Private(Logger *self);

    Tpl::LoggerInterface *mInterface;
    LoggerPtr mPtr;
};

Logger::Private::Private(Logger *self)
{
    mInterface = new Tpl::LoggerInterface(QDBusConnection::sessionBus(),
        TPL_DBUS_SRV_WELL_KNOWN_BUS_NAME,
        TPL_DBUS_SRV_OBJECT_PATH);
    mPtr = LoggerPtr(self);
}

LoggerPtr Logger::create()
{
    return LoggerPtr(new Logger());
}

Logger::Logger() :
    Tp::StatelessDBusProxy(QDBusConnection::sessionBus(), 
        QLatin1String(TPL_DBUS_SRV_WELL_KNOWN_BUS_NAME), 
	QLatin1String(TPL_DBUS_SRV_OBJECT_PATH), 
	Tp::Feature()),
    mPriv(new Private(this))
{
}

Logger::~Logger()
{
    delete mPriv;
}

Tp::PendingOperation *Logger::clearLog() const
{
    PendingLogger *operation = new PendingLogger(mPriv->mPtr, mPriv->mInterface);
    operation->clearLog();
    return operation;
}

Tp::PendingOperation *Logger::clearAccount(const Tp::AccountPtr &account) const
{
    PendingLogger *operation = new PendingLogger(mPriv->mPtr, mPriv->mInterface);
    operation->clearAccount(account);
    return operation;
}

Tp::PendingOperation *Logger::clearContact(const Tp::AccountPtr &account, const QString &objectId) const
{
    PendingLogger *operation = new PendingLogger(mPriv->mPtr, mPriv->mInterface);
    operation->clearContact(account, objectId);
    return operation;
}

Tp::PendingOperation *Logger::clearRoom(const Tp::AccountPtr &account, const QString &objectId) const
{
    PendingLogger *operation = new PendingLogger(mPriv->mPtr, mPriv->mInterface);
    operation->clearRoom(account, objectId);
    return operation;
}

}
