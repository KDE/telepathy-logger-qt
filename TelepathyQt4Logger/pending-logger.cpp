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

#include <TelepathyQt4Logger/PendingLogger>
#include <TelepathyQt4Logger/_gen/pending-logger.moc.hpp>

namespace Tpl {

PendingLogger::PendingLogger(const Tp::SharedPtr<Logger> &logger)
    : Tp::PendingOperation(logger)
{
}

void PendingLogger::setError(const QString &errorName, const QString &errorMessage)
{
    Q_ASSERT(this->errorName.isEmpty());
    Q_ASSERT(this->errorMessage.isEmpty());

    Q_ASSERT(!errorName.isEmpty());

    this->errorName = errorName;
    this->errorMessage = errorMessage;
}

void PendingLogger::finish()
{
    if (errorName.isEmpty()) {
        setFinished();
    } else {
        setFinishedWithError(errorName, errorMessage);
    }
}

}

