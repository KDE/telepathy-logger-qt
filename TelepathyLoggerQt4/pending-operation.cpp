/*
 * This file is part of TelepathyLoggerQt4
 *
 * Copyright (C) 2011 Collabora Ltd. <http://www.collabora.co.uk/>
 * Copyright (C) 2011 Nokia Corporation
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

#include <TelepathyLoggerQt4/PendingOperation>

#include <TelepathyLoggerQt4/_gen/pending-operation.moc.hpp>
#include <QtCore/QDebug>
#include <QtCore/QTimer>

using namespace Tpl;

struct TELEPATHY_LOGGER_QT4_NO_EXPORT PendingOperation::Private
{
    Private()
        : finished(false)
    {
    }

    QString errorName;
    QString errorMessage;
    bool finished;
};


PendingOperation::PendingOperation()
    : QObject(),
      mPriv(new Private())
{
    QTimer::singleShot(0, this, SLOT(start()));
}

PendingOperation::~PendingOperation()
{
    if (!mPriv->finished) {
        qWarning() << this <<
            "still pending when it was deleted - finished will "
            "never be emitted";
    }

    delete mPriv;
}

void PendingOperation::emitFinished()
{
    Q_ASSERT(mPriv->finished);
    Q_EMIT finished(this);
    deleteLater();
}

void PendingOperation::setFinished()
{
    if (mPriv->finished) {
        if (!mPriv->errorName.isEmpty()) {
            qWarning() << this << "trying to finish with success, but already"
                " failed with" << mPriv->errorName << ":" << mPriv->errorMessage;
        } else {
            qWarning() << this << "trying to finish with success, but already"
                " succeeded";
        }
        return;
    }

    mPriv->finished = true;
    Q_ASSERT(isValid());
    QTimer::singleShot(0, this, SLOT(emitFinished()));
}

void PendingOperation::setFinishedWithError(const QString &name,
        const QString &message)
{
    if (mPriv->finished) {
        if (mPriv->errorName.isEmpty()) {
            qWarning() << this << "trying to fail with" << name <<
                "but already failed with" << errorName() << ":" <<
                errorMessage();
        } else {
            qWarning() << this << "trying to fail with" << name <<
                "but already succeeded";
        }
        return;
    }

    if (name.isEmpty()) {
        qWarning() << this << "should be given a non-empty error name";
        mPriv->errorName = QLatin1String("org.freedesktop.Telepathy.Qt4.ErrorHandlingError");
    } else {
        mPriv->errorName = name;
    }

    mPriv->errorMessage = message;
    mPriv->finished = true;
    Q_ASSERT(isError());
    QTimer::singleShot(0, this, SLOT(emitFinished()));
}


bool PendingOperation::isValid() const
{
    return (mPriv->finished && mPriv->errorName.isEmpty());
}

bool PendingOperation::isFinished() const
{
    return mPriv->finished;
}

bool PendingOperation::isError() const
{
    return (mPriv->finished && !mPriv->errorName.isEmpty());
}

QString PendingOperation::errorName() const
{
    return mPriv->errorName;
}

QString PendingOperation::errorMessage() const
{
    return mPriv->errorMessage;
}

void PendingOperation::start()
{
}
