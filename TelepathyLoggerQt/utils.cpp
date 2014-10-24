/*
 * This file is part of TelepathyLoggerQt
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

#include <TelepathyLoggerQt/utils.h>

#include <TelepathyQt/AccountManager>
#include <TelepathyQt/Connection>
#include <TelepathyQt/Contact>
#include <TelepathyQt/ContactManager>
#include <TelepathyQt/ReferencedHandles>
#include <telepathy-glib/connection.h>
#include <telepathy-glib/contact.h>
#include <telepathy-glib/dbus.h>
#include <telepathy-glib/proxy.h>
#include <telepathy-glib/simple-client-factory.h>

using namespace Tpl;

Utils *Utils::instance()
{
    static Utils *instance = 0;

    if (!instance) {
        qDebug() << "Created Utils instance";
        instance = new Utils();
    }

    return instance;
}

Utils::Utils()
{
}

Utils::~Utils()
{
}

void Utils::setAccountManagerPtr(const Tp::AccountManagerPtr & accountManager)
{
    mAccountManagerPtr = accountManager;
}

Tp::AccountManagerPtr Utils::accountManagerPtr()
{
    qDebug();
    if (mAccountManagerPtr.isNull()) {
        qDebug() << "Created Tp::AccountManager instance";
        mAccountManagerPtr = Tp::AccountManager::create();
    }

    return mAccountManagerPtr;
}

TpAccountManager *Utils::tpAccountManager()
{
    qDebug();
    return tp_account_manager_dup();
}

Tp::AccountPtr Utils::accountPtr(TpAccount *account)
{
    qDebug() << "account=" << account;

    if (!account) {
        return Tp::AccountPtr();
    }

    const gchar *objectPath = tp_proxy_get_object_path(account);
    qDebug() << "objectPath=" << objectPath;
    if (!objectPath) {
        return Tp::AccountPtr();
    }

    return accountPtr(objectPath);
}

Tp::AccountPtr Utils::accountPtr(const QString &objectPath)
{
    qDebug() << "objectPath=" << objectPath;

    Tp::AccountPtr accountPtr = accountManagerPtr()->accountForPath(objectPath);

    qDebug() << "accountPtr=" << accountPtr;
    return accountPtr;
}

TpAccount *Utils::tpAccount(const Tp::AccountPtr & accountPtr)
{
    qDebug() << "account=" << accountPtr;

    QString objectPath = accountPtr->objectPath();
    qDebug() << "objectPath=" << objectPath;
    if (objectPath.isEmpty()) {
        return 0;
    }

    GError *error = NULL;
    TpAccount * account = tp_simple_client_factory_ensure_account(tp_proxy_get_factory(tpAccountManager()),
                                                                  objectPath.toUtf8(),
                                                                  NULL,
                                                                  &error);

    if (account == NULL) {
        qDebug() << "failed to create account:" << error->message;
        g_clear_error(&error);
        return NULL;
    }

    qDebug() << "account=" << account;
    return account;
}

#if 0
TpContact *ToTpContact(Tp::ContactPtr contact)
{
    if (contact.isNull()) {
        return 0;
    }

    Tp::ReferencedHandles handles = contact->handle();
    if (handles.isEmpty()) {
        return 0;
    }

    GError *error = 0;
    TpDBusDaemon * tpDBusDaemon = tp_dbus_daemon_dup(&error);
    if (error) {
        g_error_free(error);
        return 0;
    }

    if (!tpDBusDaemon) {
        return 0;
    }

    if (!contact->manager()) {
        return 0;
    }

    Tp::ConnectionPtr connection = contact->manager()->connection();
    if (!connection) {
        return 0;
    }

    QString busName = connection->busName();
    QString objectPath = connection->objectPath();
    TpConnection *tpConnection = tp_connection_new(tpDBusDaemon, busName.toLatin1(), objectPath.toLatin1(), &error);
    if (error) {
        return 0;
    }

    if (!tpConnection) {
        return 0;
    }

    Tp::ReferencedHandles::const_iterator it;
    for(it = handles.constBegin(); it != handles.constEnd(); it++) {
        // todo use async version: tp_connection_get_contacts_by_handle
        TpContact *tpContact = tp_connection_dup_contact_if_possible(tpConnection, *it, contact->id().toLatin1());
        if (tpContact) {
            return tpContact;
        }
    }

    return 0;
}

Tp::ContactPtr ToTpContactPtr(TpContact * tpContact)
{
    if (!tpContact) {
        return Tp::ContactPtr();
    }

    TpConnection * tpConnection = tp_contact_get_connection(tpContact);
    if (!tpConnection) {
        return Tp::ContactPtr();
    }

    const gchar *busName = tp_proxy_get_bus_name(TP_PROXY(tpConnection));
    if (!busName) {
        return Tp::ContactPtr();
    }

    const gchar *objectPath = tp_proxy_get_object_path(TP_PROXY(tpConnection));
    if (!objectPath) {
        return Tp::ContactPtr();
    }

    Tp::ConnectionPtr connection = Tp::Connection::create(busName, objectPath);
    if (connection.isNull()) {
        return Tp::ContactPtr();
    }

    if (connection->contactManager().isNull()) {
        return Tp::ContactPtr();
    }

    TpHandle handle = tp_contact_get_handle(contact);
    // TODO use async version
    return connection->contactManager()->lookupContactByHandle(handle);
}
#endif
