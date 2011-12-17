/*
 * This file is part of TelepathyLoggerQt4
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

#ifndef _TelepathyLoggerQt4_pending_logger_h_HEADER_GUARD_
#define _TelepathyLoggerQt4_pending_logger_h_HEADER_GUARD_

#include <TelepathyLoggerQt4/Logger>
#include <TelepathyLoggerQt4/Types>
#include <TelepathyQt/PendingOperation>

namespace Tpl
{

class Logger;

class TELEPATHY_LOGGER_QT4_NO_EXPORT PendingLogger : public Tp::PendingOperation
{
    Q_OBJECT

public:
    PendingLogger(const Tp::SharedPtr<Logger> &logger, Tpl::LoggerInterface *interface);
    ~PendingLogger() {};

    void clearLog();
    void clearAccount(const Tp::AccountPtr &account);
    void clearContact(const Tp::AccountPtr &account, const QString &objectId);
    void clearRoom(const Tp::AccountPtr &account, const QString &objectId);

protected:
    void setError(const QString &errorName, const QString &errorMessage);
    void finish();

private Q_SLOTS:
    void onLogCleared(QDBusPendingCallWatcher *watcher);

private:
    QString errorName, errorMessage;
    Tpl::LoggerInterface *mInterface;
};

}

#endif
