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

#include <TelepathyLoggerQt/LogWalker>
#include <TelepathyLoggerQt/Types>
#include <TelepathyLoggerQt/PendingEvents>

#include "pending-log-walker-operation.h"

#include <telepathy-logger/log-walker.h>

using namespace Tpl;

bool LogWalker::isStart()
{
    return tpl_log_walker_is_start(object<TplLogWalker>());
}

bool LogWalker::isEnd()
{
    return tpl_log_walker_is_end(object<TplLogWalker>());
}

PendingEvents* LogWalker::queryEvents(uint numEvents)
{
    return new PendingEvents(LogWalkerPtr(this), numEvents);
}

PendingOperation* LogWalker::rewind(uint numEvents)
{
    return new PendingLogWalkerOperation(LogWalkerPtr(this), PendingLogWalkerOperation::Rewind, numEvents);
}
