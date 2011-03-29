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

#include <TelepathyQt4Logger/Models/LoggerConversationModel>

#include "TelepathyQt4Logger/Models/_gen/logger-conversation-model.moc.hpp"

#include <QDebug>
#include <TelepathyQt4/Account>
#include <TelepathyQt4/Contact>
#include <TelepathyQt4/ContactManager>
#include <TelepathyQt4/Connection>
#include <TelepathyQt4Logger/Entity>
#include <TelepathyQt4Logger/Event>
#include <TelepathyQt4Logger/TextEvent>
#include <TelepathyQt4Logger/LogManager>
#include <TelepathyQt4Logger/PendingDates>
#include <TelepathyQt4Logger/PendingEvents>
#include <TelepathyQt4Logger/Models/Global>
#include <TelepathyQt4Yell/Models/ConversationItem>
#include <TelepathyQt4Yell/Models/AbstractConversationModel>

#define MAX_ITEMS 1000

using namespace Tpl;

struct TELEPATHY_QT4_LOGGER_NO_EXPORT LoggerConversationModel::Private
{
    Private(const Tp::AccountPtr &account, const Tp::ContactPtr &contact);
    void setup() const;
    void fetchDate(const QDate &date) const;

    Tp::AccountPtr mAccount;
    Tp::ContactPtr mContact;
    Tpl::EntityPtr mEntity;
    Tpl::QDateList mDates;
};

LoggerConversationModel::Private::Private(const Tp::AccountPtr &account, const Tp::ContactPtr &contact)
    : mAccount(account),
      mContact(contact)
{
    mEntity = Tpl::Entity::create(mContact, Tpl::EntityTypeContact);
    if (mEntity.isNull()) {
        return;
    }
}

LoggerConversationModel::LoggerConversationModel(const Tp::AccountPtr &account, const Tp::ContactPtr &contact, QObject *parent)
    : Tpy::AbstractConversationModel(parent),
      mPriv(new Private(account, contact))
{
    qDebug() << "LoggerConversationModel::LoggerConversationModel";
    setup();
}

LoggerConversationModel::~LoggerConversationModel()
{
    delete mPriv;
}

void LoggerConversationModel::setup() const
{
    if (mPriv->mAccount.isNull()) {
        return;
    }

    if (mPriv->mContact.isNull()) {
        return;
    }

    if (mPriv->mEntity.isNull()) {
        return;
    }

    Tpl::LogManagerPtr manager = Tpl::LogManager::instance();
    if (manager.isNull()) {
        return;
    }

    Tpl::PendingDates *pendingDates = manager->queryDates(mPriv->mAccount, mPriv->mEntity, Tpl::EventTypeMaskAny);
    if (!pendingDates) {
        return;
    }
    connect(pendingDates,
            SIGNAL(finished(Tpl::PendingOperation*)),
            SLOT(onPendingDatesFinished(Tpl::PendingOperation*)));
    pendingDates->start();
}

void LoggerConversationModel::onPendingDatesFinished(Tpl::PendingOperation *op)
{
    Tpl::PendingDates *pendingDates = static_cast<Tpl::PendingDates*> (op);
    if (!pendingDates || pendingDates->isError()) {
        return;
    }

    mPriv->mDates = pendingDates->dates();

    fetchMoreBack();
}

bool LoggerConversationModel::canFetchMoreBack() const
{
    qDebug() << "LoggerConversationModel::canFetchMoreBack";
    return (mPriv->mDates.size() > 0);
}

void LoggerConversationModel::fetchMoreBack()
{
    qDebug() << "LoggerConversationModel::fetchMoreBack";

    if (mPriv->mDates.size() <= 0) {
        return;
    }

    QDate date = mPriv->mDates.last();
    mPriv->mDates.pop_back();
    fetchDate(date);
}

bool LoggerConversationModel::canFetchMore(const QModelIndex & index) const
{
    return false;
}

void LoggerConversationModel::fetchMore(const QModelIndex & index)
{
}

void LoggerConversationModel::fetchDate(const QDate &date) const
{
    if (mPriv->mAccount.isNull()) {
        return;
    }

    if (mPriv->mContact.isNull()) {
        return;
    }

    if (mPriv->mEntity.isNull()) {
        return;
    }

    Tpl::LogManagerPtr manager = Tpl::LogManager::instance();
    if (manager.isNull()) {
        return;
    }

    Tpl::PendingEvents *pendingEvents = manager->queryEvents(mPriv->mAccount, mPriv->mEntity, Tpl::EventTypeMaskAny, date);
    if (!pendingEvents) {
        return;
    }
    connect(pendingEvents, SIGNAL(finished(Tpl::PendingOperation*)), SLOT(onPendingEventsFinished(Tpl::PendingOperation*)));
    pendingEvents->start();
}

void LoggerConversationModel::onPendingEventsFinished(Tpl::PendingOperation *op)
{
    qDebug() << "LoggerConversationModel::onPendingEventsFinished";

    Tpl::PendingEvents *pendingEvents = static_cast<Tpl::PendingEvents*> (op);
    if (!pendingEvents || pendingEvents->isError()) {
        return;
    }

    Tpl::EventPtrList eventList = pendingEvents->events();
    Tpl::EventPtr event;

    QList<const Tpy::ConversationItem *> items;

    beginInsertRows(QModelIndex(), 0, eventList.size() - 1);

    Q_FOREACH(event, eventList) {
        qDebug() << "LoggerConversationModel::onPendingEventsFinished: event added";
        Tpl::TextEventPtr textEvent = event.dynamicCast<Tpl::TextEvent>();
        if (!textEvent.isNull()) {
            Tpy::ConversationItem::Type messageType;
            Tp::ContactPtr contact;
            if (textEvent->sender()->identifier() == mPriv->mContact->id()) {
                messageType = Tpy::ConversationItem::INCOMING_MESSAGE;
                contact = mPriv->mContact;
            } else {
                messageType = Tpy::ConversationItem::OUTGOING_MESSAGE;
                contact = mPriv->mContact->manager()->connection()->selfContact();
            }
            Tpy::ConversationItem *item = new Tpy::ConversationItem(contact,
                   textEvent->timestamp(), textEvent->message(), messageType, this);
            items.append(item);
        }
    }

    if (items.size() > 0) {
        this->insertItems(items);
    }

    endInsertRows();
}
