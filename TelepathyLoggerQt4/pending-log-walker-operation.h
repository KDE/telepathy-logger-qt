/*
 * This file is part of TelepathyLoggerQt4
 *
 * Copyright (C) 2013 Dan Vrátil <dvratil@redhat.com>
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

#ifndef _TelepathyLoggerQt4_pending_log_walker_operation_h_HEADER_GUARD_
#define _TelepathyLoggerQt4_pending_log_walker_operation_h_HEADER_GUARD_

#include <TelepathyLoggerQt4/PendingOperation>
#include <TelepathyLoggerQt4/Types>

namespace Tpl {

/**
 * \internal
 * \brief Generic class for LogWalker operations
 */
class TELEPATHY_LOGGER_QT4_EXPORT PendingLogWalkerOperation : public Tpl::PendingOperation
{
    Q_OBJECT
    Q_DISABLE_COPY(PendingLogWalkerOperation)

  public:
    enum OperationType {
        Rewind
    };

    ~PendingLogWalkerOperation();

  private Q_SLOTS:
    virtual void start();

  private:
    friend class LogWalker;

    PendingLogWalkerOperation(const LogWalkerPtr &logWalker, OperationType operation, uint numEvents);

    struct Private;
    friend class Private;
    Private *mPriv;
};

} // Tpl

#endif
