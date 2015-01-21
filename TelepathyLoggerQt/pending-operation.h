/*
 * This file is part of TelepathyLoggerQt
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
#ifndef _TelepathyLoggerQt_pending_operation_h_HEADER_GUARD_
#define _TelepathyLoggerQt_pending_operation_h_HEADER_GUARD_

#include <QtCore/QObject>

#include <TelepathyLoggerQt_export.h>

namespace Tpl
{

/**
 * \headerfile pending-operation.h <TelepathyLoggerQt/PendingOperation>
 * \brief Abstract base class for pending asynchronous operations.
 *
 * This class represents an incomplete asynchronous operation, such as a
 * D-Bus method call. When the operation has finished, it emits
 * #finished(). The slot or slots connected to the #finished() signal may obtain
 * additional information from the PendingOperation.
 *
 * For pending operations that produce a result, another subclass of
 * PendingOperation can be used, with additional methods that provide that
 * result to the library user.
 *
 * After #finished() is emitted, the PendingOperation is automatically
 * deleted using deleteLater(), so library users must not explicitly
 * delete this object.
 *
 * The design is loosely based on KDE's KJob.
 */
class TELEPATHY_LOGGER_QT_EXPORT PendingOperation : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(PendingOperation)

public:

    /**
     * \brief Class destructor.
     */
    virtual ~PendingOperation();

    /**
     * \brief Returns whether or not the request has finished processing.
     *
     * finished() is emitted when this changes from <code>false</code> to
     * <code>true</code>.
     *
     * Equivalent to <code>(isValid() || isError())</code>.
     *
     * \sa finished()
     *
     * \return <code>true</code> if the request has finished
     */
    bool isFinished() const;

    /**
     * \brief Returns whether or not the request completed successfully.
     *
     * If the request has not yet finished processing (isFinished() returns
     * <code>false</code>), this cannot yet be known, and <code>false</code>
     * will be returned.
     *
     * Equivalent to <code>(isFinished() && !#isError())</code>.
     *
     * \return <code>true</code> iff the request has finished processing AND
     *         has completed successfully.
     */
    bool isValid() const;

    /**
     * \brief Returns whether or not the request resulted in an error.
     *
     * If the request has not yet finished processing (isFinished() returns
     * <code>false</code>), this cannot yet be known, and <code>false</code>
     * will be returned.
     *
     * Equivalent to <code>(isFinished() && !#isValid())</code>.
     *
     * \return <code>true</code> iff the request has finished processing AND
     *         has resulted in an error.
     */
    bool isError() const;


    /**
     * \brief If isError() would return true, returns the D-Bus error with which
     * the operation failed.
     *
     * If the operation succeeded or has not yet
     * finished, returns an empty string.
     *
     * \return a D-Bus error name or an empty string
     */
    QString errorName() const;

    /**
     * \brief If isError() would return true, returns a debugging message associated
     * with the error.
     *
     * The message may be an empty string. Otherwise, return an
     * empty string.
     *
     * \return a debugging message or an empty string
     */
    QString errorMessage() const;

Q_SIGNALS:
    /**
     * \brief Emitted when the pending operation finishes.
     *
     * Emitted when #isFinished() changes from <code>false</code> to <code>true</code>.
     *
     * \param operation This operation object, from which further information
     *                  may be obtained
     */
    void finished(Tpl::PendingOperation *operation);

protected:
    /**
     * \brief Protected constructor.
     *
     * Only subclasses of this class may be constructed.
     *
     * \param object The object on which this pending operation takes place
     */
    PendingOperation();

protected Q_SLOTS:
    /**
     * \brief Record that this pending operation has finished successfully.
     *
     * Will emit the #finished() signal next time the event loop runs.
     */
    void setFinished();

    /**
     * \brief Record that this pending operation has finished with an error.
     *
     * Will emit the #finished() signal next time the event loop runs.
     *
     * \param name A D-Bus error name, which must be non-empty
     * \param message A debugging message
     */
    void setFinishedWithError(const QString &name, const QString &message);

private Q_SLOTS:
    virtual void start();
    void emitFinished();

private:
    struct Private;
    friend struct Private;
    Private *mPriv;
};

} // Tpl

#endif
