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

#include <TelepathyLoggerQt/pending-clear.h>
#include <TelepathyLoggerQt/_gen/pending-clear.moc.hpp>

#include <TelepathyQt/Account>

#define TPL_DBUS_SRV_WELL_KNOWN_BUS_NAME "org.freedesktop.Telepathy.Logger"
#define TPL_DBUS_SRV_OBJECT_PATH         "/org/freedesktop/Telepathy/Logger"

namespace Tpl {

PendingClear::PendingClear()
    : Tpl::PendingOperation()
{
    mInterface = new Tpl::LoggerInterface(QDBusConnection::sessionBus(),
        TPL_DBUS_SRV_WELL_KNOWN_BUS_NAME,
        TPL_DBUS_SRV_OBJECT_PATH);
}

void PendingClear::setError(const QString &errorName, const QString &errorMessage)
{
    Q_ASSERT(this->errorName.isEmpty());
    Q_ASSERT(this->errorMessage.isEmpty());

    Q_ASSERT(!errorName.isEmpty());

    this->errorName = errorName;
    this->errorMessage = errorMessage;
}

void PendingClear::finish()
{
    if (errorName.isEmpty()) {
        setFinished();
    } else {
        setFinishedWithError(errorName, errorMessage);
    }
}

void PendingClear::clearLog()
{
    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(
                mInterface->Clear());
    connect(watcher, SIGNAL(finished(QDBusPendingCallWatcher*)),
            this,
            SLOT(onLogCleared(QDBusPendingCallWatcher*)));
}

void PendingClear::clearAccount(const Tp::AccountPtr &account)
{
    QDBusObjectPath path = QDBusObjectPath(account->objectPath());
    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(
                mInterface->ClearAccount(path));

    connect(watcher, SIGNAL(finished(QDBusPendingCallWatcher*)),
            this,
            SLOT(onLogCleared(QDBusPendingCallWatcher*)));
}

void PendingClear::clearContact(const Tp::AccountPtr &account, const QString &objectId)
{
    QDBusObjectPath path = QDBusObjectPath(account->objectPath());
    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(
                mInterface->ClearEntity(path, objectId, EntityTypeContact));

    connect(watcher, SIGNAL(finished(QDBusPendingCallWatcher*)),
            this,
            SLOT(onLogCleared(QDBusPendingCallWatcher*)));
}

void PendingClear::clearRoom(const Tp::AccountPtr &account, const QString &objectId)
{
    QDBusObjectPath path = QDBusObjectPath(account->objectPath());
    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(
                mInterface->ClearEntity(path, objectId, EntityTypeRoom));

    connect(watcher, SIGNAL(finished(QDBusPendingCallWatcher*)),
            this,
            SLOT(onLogCleared(QDBusPendingCallWatcher*)));
}

void PendingClear::onLogCleared(QDBusPendingCallWatcher *watcher)
{
    QDBusPendingReply<> reply = *watcher;

    if (!reply.isError()) {
        qDebug() << "PendingLogger:onLogCleared: Log has been cleared";
    } else {
        qWarning().nospace() << "PendingLogger:onLogCleared:  Clear log failed with " <<
            reply.error().name() << ":" << reply.error().message();
    }

    if(!reply.isError()) {
        finish();
    } else {
        setError(reply.error().name(), reply.error().message());
        finish();
    }

}

}

