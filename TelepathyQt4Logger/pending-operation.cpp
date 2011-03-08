/*
 * This file is part of TelepathyQt4Logger
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

#include <QDebug>
#include <QTimer>
#include <TelepathyQt4Logger/PendingOperation>

using namespace Tpl;

struct PendingOperation::Private
{
    Private()
        : finished(false)
    {
    }

    QString errorName;
    QString errorMessage;
    bool finished;
};

/**
 * \class PendingOperation
 * \headerfile TelepathyQt4/pending-operation.h <TelepathyQt4/PendingOperation>
 *
 * \brief Abstract base class for pending asynchronous operations.
 *
 * This class represents an incomplete asynchronous operation, such as a
 * D-Bus method call. When the operation has finished, it emits
 * #finished. The slot or slots connected to the #finished() signal may obtain
 * additional information from the %PendingOperation.
 *
 * In simple cases, like a D-Bus method with no 'out' arguments or for which
 * all 'out' arguments are to be ignored (so the possible results are
 * success with no extra information, or failure with an error code), the
 * trivial subclass %PendingVoid can be used.
 *
 * For pending operations that produce a result, another subclass of
 * %PendingOperation can be used, with additional methods that provide that
 * result to the library user.
 *
 * After #finished() is emitted, the %PendingOperation is automatically
 * deleted using deleteLater(), so library users must not explicitly
 * delete this object.
 *
 * The design is loosely based on KDE's KJob.
 */

/**
 * Protected constructor. Only subclasses of this class may be constructed.
 *
 * \param object The object on which this pending operation takes place
 */
PendingOperation::PendingOperation()
    : QObject(),
      mPriv(new Private())
{
}

/**
 * Class destructor.
 */
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

/**
 * Record that this pending operation has finished successfully, and
 * emit the #finished() signal next time the event loop runs.
 */
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

/**
 * Record that this pending operation has finished with an error, and
 * emit the #finished() signal next time the event loop runs.
 *
 * \param name A D-Bus error name, which must be non-empty
 * \param message A debugging message
 */
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

/**
 * Returns whether or not the request completed successfully. If the
 * request has not yet finished processing (isFinished() returns
 * <code>false</code>), this cannot yet be known, and <code>false</code>
 * will be returned.
 *
 * Equivalent to <code>(isFinished() && !isError())</code>.
 *
 * \return <code>true</code> iff the request has finished processing AND
 *         has completed successfully.
 */
bool PendingOperation::isValid() const
{
    return (mPriv->finished && mPriv->errorName.isEmpty());
}

/**
 * Returns whether or not the request has finished processing. #finished()
 * is emitted when this changes from <code>false</code> to
 * <code>true</code>.
 *
 * Equivalent to <code>(isValid() || isError())</code>.
 *
 * \sa finished()
 *
 * \return <code>true</code> if the request has finished
 */
bool PendingOperation::isFinished() const
{
    return mPriv->finished;
}

/**
 * Returns whether or not the request resulted in an error. If the
 * request has not yet finished processing (isFinished() returns
 * <code>false</code>), this cannot yet be known, and <code>false</code>
 * will be returned.
 *
 * Equivalent to <code>(isFinished() && !isValid())</code>.
 *
 * \return <code>true</code> iff the request has finished processing AND
 *         has resulted in an error.
 */
bool PendingOperation::isError() const
{
    return (mPriv->finished && !mPriv->errorName.isEmpty());
}

/**
 * If isError() would return true, returns the D-Bus error with which
 * the operation failed. If the operation succeeded or has not yet
 * finished, returns an empty string.
 *
 * \return a D-Bus error name or an empty string
 */
QString PendingOperation::errorName() const
{
    return mPriv->errorName;
}

/**
 * If isError() would return true, returns a debugging message associated
 * with the error, which may be an empty string. Otherwise, return an
 * empty string.
 *
 * \return a debugging message or an empty string
 */
QString PendingOperation::errorMessage() const
{
    return mPriv->errorMessage;
}

/**
 * \fn void PendingOperation::finished(Tp::PendingOperation* operation)
 *
 * Emitted when the pending operation finishes, i.e. when #isFinished()
 * changes from <code>false</code> to <code>true</code>.
 *
 * \param operation This operation object, from which further information
 *                  may be obtained
 */
