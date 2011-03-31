/*
 * This file is part of TelepathyQt4Yell
 *
 * Copyright (C) 2010 Collabora Ltd. <http://www.collabora.co.uk/>
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

#ifndef _TelepathyQt4Logger_logger_h_HEADER_GUARD_
#define _TelepathyQt4Logger_logger_h_HEADER_GUARD_

#ifndef IN_TELEPATHY_QT4_LOGGER_HEADER
#error IN_TELEPATHY_QT4_LOGGER_HEADER
#endif

#include <TelepathyQt4Logger/_gen/cli-logger.h>

#include <TelepathyQt4Logger/Types>
#include <TelepathyQt4/OptionalInterfaceFactory>
#include <TelepathyQt4/PendingOperation>

#include <QDBusPendingCallWatcher>
#include <QObject>

namespace Tpl
{

#define TPL_DBUS_SRV_WELL_KNOWN_BUS_NAME \
  "org.freedesktop.Telepathy.Logger"
#define TPL_DBUS_SRV_OBJECT_PATH \
  "/org/freedesktop/Telepathy/Logger"

class Logger;

typedef Tp::SharedPtr<Logger> LoggerPtr;

class Logger : public Tp::StatelessDBusProxy
{
    Q_OBJECT

public:
    Logger();
    ~Logger();

    Tp::PendingOperation *clearLog();

Q_SIGNALS:
    void logCleared();

private Q_SLOTS:
    void onLogCleared(QDBusPendingCallWatcher *watcher);

private:
    Tpl::LoggerInterface *mInterface;
    LoggerPtr mPtr;

    QMap<QDBusPendingCallWatcher *, Tp::PendingOperation *> mOperationMap;
};

class PendingClearOp : public Tp::PendingOperation
{
    Q_OBJECT

public:
    PendingClearOp(const LoggerPtr &logger);
    ~PendingClearOp() {};

    void setError(const QString &errorName, const QString &errorMessage);
    void finish();

private:
    QString errorName, errorMessage;
};

}

#endif
