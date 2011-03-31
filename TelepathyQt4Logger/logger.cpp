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
#include <QDBusPendingReply>
#include <QDebug>

namespace Tpl {

PendingClearOp::PendingClearOp(const LoggerPtr &logger)
    : Tp::PendingOperation(logger)
{
}

void PendingClearOp::setError(const QString &errorName, const QString &errorMessage)
{
    Q_ASSERT(this->errorName.isEmpty());
    Q_ASSERT(this->errorMessage.isEmpty());

    Q_ASSERT(!errorName.isEmpty());

    this->errorName = errorName;
    this->errorMessage = errorMessage;
}

void PendingClearOp::finish()
{
    if (errorName.isEmpty()) {
        setFinished();
    } else {
        setFinishedWithError(errorName, errorMessage);
    }
}

Logger::Logger() :
    Tp::StatelessDBusProxy(QDBusConnection::sessionBus(), QLatin1String(TPL_DBUS_SRV_WELL_KNOWN_BUS_NAME), QLatin1String(TPL_DBUS_SRV_WELL_KNOWN_BUS_NAME), Tp::Feature()),
    mInterface(0)
{
    mInterface = new Tpl::LoggerInterface(QDBusConnection::sessionBus(),
                                                       TPL_DBUS_SRV_WELL_KNOWN_BUS_NAME, TPL_DBUS_SRV_WELL_KNOWN_BUS_NAME);
    mPtr = LoggerPtr(this);
}

Logger::~Logger()
{

}

Tp::PendingOperation *Logger::clearLog()
{
    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(
                mInterface->Clear());
    connect(watcher, SIGNAL(finished(QDBusPendingCallWatcher*)),
            this,
            SLOT(onLogCleared(QDBusPendingCallWatcher*)));


    PendingClearOp *operation = new PendingClearOp(mPtr);
    mOperationMap.insert(watcher, operation);
    return operation;
}

void Logger::onLogCleared(QDBusPendingCallWatcher *watcher)
{
    QDBusPendingReply<> reply = *watcher;

    if (!reply.isError()) {
        qDebug() << "Logger:onLogCleared: Log has been cleared";
        Q_EMIT logCleared();
    } else {
        qWarning().nospace() << "Logger:onLogCleared:  Clear log failed with " <<
            reply.error().name() << ":" << reply.error().message();
    }

    if(mOperationMap.contains(watcher)) {
        PendingClearOp *pendingOp = static_cast<PendingClearOp *>(mOperationMap.value(watcher));
        if(pendingOp) {
            if(!reply.isError()) {
                pendingOp->finish();
            } else {
                pendingOp->setError(reply.error().name(), reply.error().message());
                pendingOp->finish();
            }
            mOperationMap.remove(watcher);
        }
    }
}

}
