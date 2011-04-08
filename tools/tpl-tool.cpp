/*
 * This file is part of TelepathyLoggerQt4
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
#include <TelepathyLoggerQt4/utils.h>
#include <QApplication>
#include <TelepathyQt4/Account>
#include <TelepathyQt4/AccountSet>
#include <TelepathyQt4/AccountManager>
#include <TelepathyQt4/ContactManager>
#include <TelepathyQt4/Connection>
#include <TelepathyQt4/PendingReady>
#include <TelepathyLoggerQt4/Entity>
#include <TelepathyLoggerQt4/Event>
#include <TelepathyLoggerQt4/TextEvent>
#include <TelepathyLoggerQt4/LogManager>
#include <TelepathyLoggerQt4/PendingDates>
#include <TelepathyLoggerQt4/PendingEntities>
#include <TelepathyLoggerQt4/PendingEvents>
#include <TelepathyLoggerQt4/PendingSearch>
#include <TelepathyLoggerQt4/Init>
#include <glib-object.h>
#include <QGst/Init>
#include <QDebug>

#define TPL_TOOL_DATE_FORMAT "yyyyMMdd"

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

    mAccountPtr = Tpl::Utils::instance()->accountPtr(id);
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

    return Tpl::Entity::create(id.toUtf8(), Tpl::EntityTypeContact, NULL, NULL);
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
               (args.size() == 5 && args.at(1) == "events") ||
               (args.size() == 5 && args.at(1) == "filteredEvents")) {
        Tp::AccountPtr account = accountPtr(args.at(2));
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
    qDebug() << "tpl-tool events <account> <entity> <date>";
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
        qDebug() << "number of contacts = " << contacts.size();

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
    } else if (args.at(1) == "entities") {
        Tpl::PendingEntities *pe = logManager->queryEntities(mAccountPtr);
        debugfn() << "PendingEntities=" << pe;
        if (!pe) {
            qWarning() << "Error in PendingEntities";
            this->exit(-1);
            return false;
        }

        connect(pe,
                SIGNAL(finished(Tpl::PendingOperation*)),
                this,
                SLOT(onPendingEntities(Tpl::PendingOperation*)));

        return true;
    } else if (args.at(1) == "dates") {
        Tpl::EntityPtr entity = entityPtr(args.at(3));
        if (entity.isNull()) {
            qWarning() << "Entity not found " << args.at(3);
        }

        Tpl::PendingDates *pd = logManager->queryDates(mAccountPtr, entity, Tpl::EventTypeMaskAny);
        debugfn() << "PendingDates=" << pd;
        if (!pd) {
            qWarning() << "Error in PendingDates";
            this->exit(-1);
            return false;
        }

        connect(pd,
                SIGNAL(finished(Tpl::PendingOperation*)),
                this,
                SLOT(onPendingDates(Tpl::PendingOperation*)));

        return true;
    } else if (args.at(1) == "events") {
        Tpl::EntityPtr entity = entityPtr(args.at(3));
        if (entity.isNull()) {
            qWarning() << "Entity not found " << args.at(3);
        }

        QDate date = QDate::fromString(args.at(4), TPL_TOOL_DATE_FORMAT);

        Tpl::PendingEvents *pe = logManager->queryEvents(mAccountPtr, entity, Tpl::EventTypeMaskAny, date);
        debugfn() << "PendingEvents=" << pe << "date=" << date;
        if (!pe) {
            qWarning() << "Error in PendingDates";
            this->exit(-1);
            return false;
        }

        connect(pe,
                SIGNAL(finished(Tpl::PendingOperation*)),
                this,
                SLOT(onPendingEvents(Tpl::PendingOperation*)));

        return true;
    } else if (args.at(1) == "filteredEvents") {
        Tpl::EntityPtr entity = entityPtr(args.at(3));
        if (entity.isNull()) {
            qWarning() << "Entity not found " << args.at(3);
        }

        Tpl::PendingEvents *pe = logManager->queryFilteredEvents(mAccountPtr, entity, Tpl::EventTypeMaskAny, args.at(4).toInt(), &TplToolApplication::eventFilterMethod, this);
        debugfn() << "PendingEvents (filtered) =" << pe << "numEvents=" << args.at(4);
        if (!pe) {
            qWarning() << "Error in PendingDates";
            this->exit(-1);
            return false;
        }

        connect(pe,
                SIGNAL(finished(Tpl::PendingOperation*)),
                this,
                SLOT(onPendingEvents(Tpl::PendingOperation*)));

        return true;
    }

    this->exit(-1);
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

    Tpl::SearchHitList hits = ps->hits();
    qDebug() << " search hits " << hits.size();

    int count = 0;

    Q_FOREACH(const Tpl::SearchHit &hit, hits) {
        qDebug() << count++ << "account=" << hit.account.data() << (hit.account.isNull() ? "null" : hit.account->objectPath())
                 << "date=" << hit.date.toString(TPL_TOOL_DATE_FORMAT)
                 << "target=" << (hit.target ? hit.target->identifier() + "/" + hit.target->alias() + "/" + QString::number(hit.target->entityType()) + "/" + hit.target->avatarToken() : "null");
    }

    this->exit();
}

void TplToolApplication::onPendingEntities(Tpl::PendingOperation *po)
{
    Tpl::PendingEntities *pe = (Tpl::PendingEntities*) po;

    if (pe->isError()) {
        qWarning() << "error in search";
        exit(-1);
        return;
    }

    Tpl::EntityPtrList entities= pe->entities();
    qDebug() << " Pending entities " << entities.size();

    int count = 0;
    Tpl::EntityPtr entity;
    Q_FOREACH(entity, entities) {
        qDebug() << count++ << "entity id=" << entity->identifier() << "alias=" << entity->alias() << "type=" << entity->entityType() << "avatarToken=" << entity->avatarToken();
    }

    this->exit();
}

void TplToolApplication::onPendingDates(Tpl::PendingOperation *po)
{
    Tpl::PendingDates *pd = (Tpl::PendingDates*) po;

    if (pd->isError()) {
        qWarning() << "error in search";
        exit(-1);
        return;
    }

    Tpl::QDateList dates = pd->dates();
    qDebug() << " Pending dates " << dates.size();

    int count = 0;
    QDate date;
    Q_FOREACH(date, dates) {
        qDebug() << count++ << "date " << date.toString(TPL_TOOL_DATE_FORMAT);
    }

    this->exit();
}

void TplToolApplication::onPendingEvents(Tpl::PendingOperation *po)
{
    Tpl::PendingEvents *pe = (Tpl::PendingEvents*) po;

    if (pe->isError()) {
        qWarning() << "error in PendingEvents";
        exit(-1);
        return;
    }

    Tpl::EventPtrList events = pe->events();
    qDebug() << " Pending events " << events.size();

    int count = 0;
    QObject a;
    Tpl::EventPtr event;
    Q_FOREACH(event, events) {
        Tpl::TextEventPtr textEvent = event.dynamicCast<Tpl::TextEvent>();
        if (!textEvent.isNull()) {
            qDebug() << count++ << "textEvent"
                      << "timestamp=" << textEvent->timestamp().toString()
                      << "sender=" << textEvent->sender()->identifier()
                      << "receiver=" << textEvent->receiver()->identifier()
                      << "message=" << textEvent->message()
                      << "messageType=" << textEvent->messageType()
                      << "account=" << (textEvent->account() ? textEvent->account()->objectPath() : "null")
                      << "accountPath=" << textEvent->accountPath();
        } else {
            qDebug() << count++ << "event"
                      << "timestamp=" << event->timestamp().toString()
                      << "sender=" << event->sender()->identifier()
                      << "receiver=" << event->receiver()->identifier()
                      << "account=" << (event->account() ? event->account()->objectPath() : "null")
                      << "accountPath=" << event->accountPath();
        }
    }

    this->exit();
}

bool TplToolApplication::eventFilterMethod(const Tpl::EventPtr &event, void *user_data)
{
    return true;
}

int main(int argc, char **argv)
{
    g_type_init();
    Tp::registerTypes();

    TplToolApplication app(argc, argv);
    Tpl::init();

    return app.exec();
}
