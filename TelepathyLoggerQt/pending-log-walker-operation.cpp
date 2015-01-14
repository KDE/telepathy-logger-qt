/*
 * This file is part of TelepathyLoggerQt
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

#include "pending-log-walker-operation.h"

#include <TelepathyLoggerQt/LogWalker>
#include <TelepathyLoggerQt/_gen/pending-log-walker-operation.moc.hpp>
#include <TelepathyQt/Constants>

#include <telepathy-logger/log-walker.h>

#include "utils.h"

using namespace Tpl;

struct TELEPATHY_LOGGER_QT_NO_EXPORT PendingLogWalkerOperation::Private
{
    LogWalkerPtr logWalker;
    OperationType operation;
    int numEvents;

    static void rewindFinished(TplLogWalker *tpLogWalker, void *result, PendingLogWalkerOperation *operation);
};

PendingLogWalkerOperation::PendingLogWalkerOperation(const LogWalkerPtr& logWalker,
                                                     PendingLogWalkerOperation::OperationType operation,
                                                     uint numEvents)
    : PendingOperation(),
      mPriv(new Private())
{
    mPriv->logWalker = logWalker;
    mPriv->operation = operation;
    mPriv->numEvents = numEvents;
}

PendingLogWalkerOperation::~PendingLogWalkerOperation()
{
    delete mPriv;
}

void PendingLogWalkerOperation::start()
{
    if (mPriv->operation == Rewind) {
        tpl_log_walker_rewind_async(
            TPLoggerQtWrapper::unwrap<TplLogWalker, LogWalker>(mPriv->logWalker),
            mPriv->numEvents,
            (GAsyncReadyCallback) Private::rewindFinished,
            this);
    }
}

void PendingLogWalkerOperation::Private::rewindFinished(TplLogWalker *tpLogWalker,
                                                        void* result,
                                                        PendingLogWalkerOperation* operation)
{
    if (!TPL_IS_LOG_WALKER(tpLogWalker)) {
        operation->setFinishedWithError(TP_QT_ERROR_INVALID_ARGUMENT, "Invalid log walker in callback");
        return;
    }

    if (!G_IS_ASYNC_RESULT(result)) {
        operation->setFinishedWithError(TP_QT_ERROR_INVALID_ARGUMENT, "Invalid async result in callback");
        return;
    }

    operation->setFinished();
}
