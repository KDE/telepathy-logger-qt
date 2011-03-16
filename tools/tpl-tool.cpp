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

#include "tpl-tool.h"
#include <tools/_gen/tpl-tool.moc.hpp>
#include <TelepathyQt4Logger/utils.h>
#include <QApplication>
#include <TelepathyQt4/Account>
#include <TelepathyQt4/AccountSet>
#include <TelepathyQt4/AccountManager>
#include <TelepathyQt4/ContactManager>
#include <TelepathyQt4/Connection>
#include <TelepathyQt4/PendingReady>
#include <TelepathyQt4Logger/Entity>
#include <TelepathyQt4Logger/LogManager>
#include <TelepathyQt4Logger/PendingDates>
#include <TelepathyQt4Logger/PendingEntities>
#include <TelepathyQt4Logger/PendingEvents>
#include <TelepathyQt4Logger/PendingSearch>
#include <TelepathyQt4Logger/Init>
#include <glib-object.h>
#include <QGst/Init>
#include <QDebug>

TplToolApplication::TplToolApplication(int &argc, char **argv)
    : QCoreApplication(argc, argv)
{
    debugfn();

    mAccountManager = Tp::AccountManager::create();
    connect(mAccountManager->becomeReady(Tp::AccountManager::FeatureCore),
            SIGNAL(finished(Tp::PendingOperation*)),
            this,
            SLOT(onAccountManagerReady(Tp::PendingOperation*)));
}

Tp::AccountPtr TplToolApplication::accountPtr(const QString &id)
{
    debugfn();

    mAccountPtr = Tpl::Utils::instance()->accountPtr(id.toAscii());
    if (!mAccountPtr->isValid()) {
        return mAccountPtr;
    }

    connect(mAccountPtr->becomeReady(Tp::Features() << Tp::Account::FeatureCore << Tp::Account::FeatureAvatar),
            SIGNAL(finished(Tp::PendingOperation*)),
            this,
            SLOT(onAccountReady(Tp::PendingOperation*)));

    return mAccountPtr;
}

Tpl::EntityPtr TplToolApplication::entityPtr(const QString &id)
{
    debugfn();

    return Tpl::Entity::create(id.toAscii(), Tpl::EntityTypeContact, NULL, NULL);
}

bool TplToolApplication::parseArgs1()
{
    debugfn();

    QStringList args = arguments();

    Tpl::LogManagerPtr logManager = Tpl::LogManager::instance();
    if (logManager.isNull()) {
        qWarning() << "LogManager not found";
        return false;
    }

    if (args.size() == 2 && args.at(1) == "accounts") {
        Tp::AccountSetPtr accountSet = Tpl::Utils::instance()->accountManagerPtr()->validAccounts();
        QList<Tp::AccountPtr> accounts = accountSet->accounts();
        Tp::AccountPtr account;
        int i = 0;
        Q_FOREACH(account, accounts) {
            qDebug() << "account " << i++ << account->objectPath();
        }
        this->exit();
        return true;
    } else if ((args.size() == 3 && args.at(1) == "contacts") ||
               (args.size() == 4 && args.at(1) == "exists") ||
               (args.size() == 3 && args.at(1) == "entities") ||
               (args.size() == 4 && args.at(1) == "dates") ||
               (args.size() == 4 && args.at(1) == "events") ||
               (args.size() == 5 && args.at(1) == "filteredEvents")) {
        Tp::AccountPtr account = accountPtr(args.at(2).toAscii());
        if (account.isNull()) {
            qWarning() << "Account not found " << args.at(2);
        }
        return true;
    } else if (args.size() == 3 && args.at(1) == "search") {
        Tpl::PendingSearch *ps = logManager->search(args.at(2), Tpl::EventTypeMaskAny);
        debugfn() << "PendingSearch=" << ps;
        if (!ps) {
            qWarning() << "Error in search";
            this->exit(-1);
            return false;
        }

        connect(ps,
                SIGNAL(finished(Tpl::PendingOperation*)),
                this,
                SLOT(onPendingSearch(Tpl::PendingOperation*)));

        ps->start();

        return true;
    }

    qDebug() << "Telepathy logger command line tool (qt4)";
    qDebug() << "";
    qDebug() << "General usage: tpl-tool <command> <parameters>";
    qDebug() << "";
    qDebug() << "tpl-tool accounts";
    qDebug() << "tpl-tool contacts <account>";
    qDebug() << "tpl-tool exists <account> <entity>";
    qDebug() << "tpl-tool entities <account>";
    qDebug() << "tpl-tool dates <account> <entity>";
    qDebug() << "tpl-tool events <account> <entity>";
    qDebug() << "tpl-tool filteredEvents <account> <entity> <numEvents>";
    qDebug() << "tpl-tool search <text>";
    this->exit(-1);

    return false;
}

bool TplToolApplication::parseArgs2()
{
    debugfn();

    QStringList args = arguments();

    Tpl::LogManagerPtr logManager = Tpl::LogManager::instance();
    if (logManager.isNull()) {
        qWarning() << "LogManager not found";
        return false;
    }

    if (args.size() == 3 && args.at(1) == "contacts") {
        Tp::Contacts contacts = mAccountPtr->connection()->contactManager()->allKnownContacts();
        debugfn() << "number of contacts = " << contacts.size();

        Tp::ContactPtr contact;
        int i = 0;
        Q_FOREACH(contact, contacts) {
            qDebug() << "contact " << i++ << contact->id();
        }
        this->exit();
        return true;
    } else if (args.size() == 4 && args.at(1) == "exists") {
        Tpl::EntityPtr entity = entityPtr(args.at(3));
        if (entity.isNull()) {
            qWarning() << "Entity not found " << args.at(3);
        }

        bool ret = logManager->exists(mAccountPtr, entity, Tpl::EventTypeMaskAny);
        qDebug() << "tpl-tool exists " << args.at(2) << args.at(3) << " -> " << ret;
        this->exit();
        return true;
    } else if (args.at(0) == "entities") {
    } else if (args.at(0) == "dates") {
    } else if (args.at(0) == "events") {
    } else if (args.at(0) == "filteredEvents") {
    }

    return false;
}

void TplToolApplication::onAccountManagerReady(Tp::PendingOperation *po)
{
    debugfn() << "po=" << po << "isError=" << po->isError();

    if (po->isError()) {
        qWarning() << "error getting account mananger ready";
        exit(-1);
        return;
    }

    Tpl::LogManagerPtr logManager = Tpl::LogManager::instance();
    if (logManager.isNull()) {
        qWarning() << "LogManager not found";
        exit(-1);
        return;
    }

    logManager->setAccountManagerPtr(mAccountManager);

    parseArgs1();
}

void TplToolApplication::onAccountReady(Tp::PendingOperation *po)
{
    debugfn() << "po=" << po << "isError=" << po->isError();

    if (po->isError()) {
        qWarning() << "error getting account ready";
        exit(-1);
        return;
    }

    Tp::ConnectionPtr connection = mAccountPtr->connection();
    if (connection.isNull()) {
        qWarning() << "error null connection";
        exit(-1);
        return;
    }

    connect(mAccountPtr->connection()->becomeReady(Tp::Features() << Tp::Connection::FeatureCore << Tp::Connection::FeatureSelfContact << Tp::Connection::FeatureRoster),
        SIGNAL(finished(Tp::PendingOperation*)),
        this,
        SLOT(onConnectionReady(Tp::PendingOperation*)));
}

void TplToolApplication::onConnectionReady(Tp::PendingOperation *po)
{
    debugfn() << "po=" << po << "isError=" << po->isError();

    if (po->isError()) {
        qWarning() << "error getting connection ready";
        exit(-1);
        return;
    }

    parseArgs2();
}

void TplToolApplication::onPendingSearch(Tpl::PendingOperation *po)
{
    Tpl::PendingSearch *ps = (Tpl::PendingSearch*) po;

    if (ps->isError()) {
        qWarning() << "error in search";
        exit(-1);
        return;
    }

    Tpl::SearchHitList *hits = ps->hits();
    debugfn() << " search hits " << hits->size();

    Tpl::SearchHit *hit;
    Q_FOREACH(hit, *hits) {
        //debugfn() << "account=" << hit->account << "date=" << hit->date << "target=" << hit->target ? hit->target->identifier() : "null";
        debugfn() << "account=" << hit->account;
        debugfn() << "date=" << hit->date;
        debugfn() << "entity=" << (hit->target.isNull() ? "null" : hit->target->identifier());
    }

    this->exit();
}

int main(int argc, char **argv)
{
    g_type_init();
    Tp::registerTypes();

    TplToolApplication app(argc, argv);
    Tpl::init();

    return app.exec();
}
