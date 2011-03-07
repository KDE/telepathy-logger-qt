/*
 * This file is part of QTpLogger
 *
 * Copyright (C) 2011 Collabora Ltd. <http://www.collabora.co.uk/>
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

#include "pending-tpcontacts.h"
#include "constants.h"
#include <TelepathyQt4/Account>
#include <TelepathyQt4/ReferencedHandles>
#include <TelepathyQt4/ContactManager>
#include <telepathy-logger/log-manager.h>
#include <QDebug>
#include <glib/gerror.h>
#include <glib/gdate.h>

using namespace QTpLogger;

struct QTPLOGGER_NO_EXPORT PendingTpContacts::Private
{
    /*
    Private(const ContactPtrList &contactPtrList)
        : mContactPtrList(contactPtrList),
          mTpContacts(0)
    {
    }
    */

    Private(Tp::ContactPtr &contactPtr)
        : mTpContacts(0),
          mNumTpContacts(0)
    {
        mContactPtrList << contactPtr;
    }

    ~Private()
    {
        if (mTpContacts) {
            free(mTpContacts);
        }
    }

    ContactPtrList mContactPtrList;
    TpContact **mTpContacts;
    uint mNumTpContacts;

    static void callback(TpConnection *connection, guint n_contacts, TpContact * const *contacts,
            guint n_failed, const TpHandle *failed, const GError *error,
            gpointer user_data, GObject *weak_object);
};

PendingTpContacts::PendingTpContacts(Tp::ContactPtr contact)
    : PendingOperation(),
      mPriv(new Private(contact))
{
}

/*
PendingTpContacts::PendingTpContacts(ContactPtrList contactPtrList)
    : PendingOperation(),
      mPriv(new Private(contactPtrList))
{
}
*/

PendingTpContacts::~PendingTpContacts()
{
    delete mPriv;
}

void PendingTpContacts::start()
{
    if (mPriv->mContactPtrList.size() <= 0) {
        setFinishedWithError(QTPLOGGER_ERROR_INVALID_ARGUMENT, "No contacts provided");
    }

    Tp::ContactPtr contact = mPriv->mContactPtrList.at(0);

    if (contact.isNull()) {
        setFinishedWithError(QTPLOGGER_ERROR_INVALID_ARGUMENT, "No valid contact provided");
        return;
    }

    Tp::ReferencedHandles handles = contact->handle();
    if (handles.isEmpty()) {
        setFinishedWithError(QTPLOGGER_ERROR_INVALID_ARGUMENT, "No valid contact provided");
        return;
    }

    GError *error = 0;
    TpDBusDaemon * tpDBusDaemon = tp_dbus_daemon_dup(&error);
    if (error) {
        g_error_free(error);
        setFinishedWithError(QTPLOGGER_ERROR_INVALID_ARGUMENT, "Invalid dbus daemon");
        return;
    }

    if (!tpDBusDaemon) {
        setFinishedWithError(QTPLOGGER_ERROR_INVALID_ARGUMENT, "Invalid dbus daemon");
        return;
    }

    if (!contact->manager()) {
        setFinishedWithError(QTPLOGGER_ERROR_INVALID_ARGUMENT, "No Contact Manager found");
        return;
    }

    Tp::ConnectionPtr connection = contact->manager()->connection();
    if (!connection) {
        setFinishedWithError(QTPLOGGER_ERROR_INVALID_ARGUMENT, "No Contact Manager found");
        return;
    }

    QString busName = connection->busName();
    QString objectPath = connection->objectPath();
    TpConnection *tpConnection = tp_connection_new(tpDBusDaemon, busName.toLatin1(), objectPath.toLatin1(), &error);
    if (error) {
        setFinishedWithError(QTPLOGGER_ERROR_INVALID_ARGUMENT, "No connection found");
        return;
    }

    if (!tpConnection) {
        setFinishedWithError(QTPLOGGER_ERROR_INVALID_ARGUMENT, "No connection found");
        return;
    }

    Tp::ReferencedHandles::const_iterator it;
    for(it = handles.constBegin(); it != handles.constEnd(); it++) {
        // todo use async version: tp_connection_get_contacts_by_handle
        TpHandle handle = *it;
        tp_connection_get_contacts_by_handle(tpConnection, 1, &handle, 0, NULL,
            Private::callback, this, NULL, NULL);
        //TpContact *tpContact = tp_connection_dup_contact_if_possible(connection, *it, contact->id());
    }
}

void PendingTpContacts::Private::callback(TpConnection *connection, guint n_contacts,
        TpContact * const *contacts, guint n_failed, const TpHandle *failed, const GError *error,
        gpointer user_data, GObject *weak_object)
{
    PendingTpContacts *self = (PendingTpContacts *) user_data;

    if (error != NULL) {
        self->setFinishedWithError(QTPLOGGER_ERROR_INVALID_ARGUMENT, error->message);
        return;
    }

    if (n_failed > 0 || n_contacts <= 0) {
        self->setFinishedWithError(QTPLOGGER_ERROR_INVALID_ARGUMENT, "Failed to prepare some TpContact (InvalidHandle)");
        return;
    }

    self->mPriv->mNumTpContacts = n_contacts;
    self->mPriv->mTpContacts = (TpContact**) malloc(sizeof(TpContact*) * self->mPriv->mNumTpContacts);
    memcpy(self->mPriv->mTpContacts, contacts, sizeof(TpContact*) * self->mPriv->mNumTpContacts);

    self->setFinished();
}

TpContact *PendingTpContacts::tpContact() const
{
    if (!isFinished()) {
        qWarning() << "PendingTpContacts::tpContact called before finished, returning 0";
        return 0;
    } else if (!isValid()) {
        qWarning() << "PendingTpContacts::tpContact called when not valid, returning 0";
        return 0;
    } else if (mPriv->mNumTpContacts <= 0) {
        return 0;
    }

    return mPriv->mTpContacts[0];
}
