/*
 * This file is part of TelepathyLoggerQt
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

#ifndef _TelepathyLoggerQt_pending_log_manager_h_HEADER_GUARD_
#define _TelepathyLoggerQt_pending_log_manager_h_HEADER_GUARD_

#include <TelepathyLoggerQt/LogManager>
#include <TelepathyLoggerQt/PendingOperation>

namespace Tpl
{

class Logger;

/**
 * \internal
 */
class TELEPATHY_LOGGER_QT_NO_EXPORT PendingClear : public Tpl::PendingOperation
{
    Q_OBJECT

public:
    PendingClear();
    ~PendingClear() {};

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
