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

#include <TelepathyLoggerQt4/Models/LoggerConversationModel>

#include "TelepathyLoggerQt4/Models/_gen/logger-conversation-model.moc.hpp"

#include <QtCore/QDebug>
#include <TelepathyQt/Account>
#include <TelepathyQt/Contact>
#include <TelepathyQt/ContactManager>
#include <TelepathyQt/Connection>
#include <TelepathyLoggerQt4/Entity>
#include <TelepathyLoggerQt4/TextEvent>
#include <TelepathyLoggerQt4/CallEvent>
#include <TelepathyLoggerQt4/LogManager>
#include <TelepathyLoggerQt4/PendingDates>
#include <TelepathyLoggerQt4/PendingEvents>
#include <TelepathyQt4Yell/Models/TextEventItem>
#include <TelepathyQt4Yell/Models/CallEventItem>

#define MAX_ITEMS 1000

using namespace Tpl;

struct TELEPATHY_LOGGER_QT4_MODELS_NO_EXPORT LoggerConversationModel::Private
{
    Private(const Tp::AccountPtr &account, const Tp::ContactPtr &contact);
    void setup() const;
    void fetchDate(const QDate &date) const;

    Tp::AccountPtr mAccount;
    Tp::ContactPtr mContact;
    Tpl::EntityPtr mEntity;
    Tpl::QDateList mDates;
    bool mBackFetching;
};

LoggerConversationModel::Private::Private(const Tp::AccountPtr &account, const Tp::ContactPtr &contact)
    : mAccount(account),
      mContact(contact),
      mBackFetching(false)
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
}

void LoggerConversationModel::onPendingDatesFinished(Tpl::PendingOperation *op)
{
    Tpl::PendingDates *pendingDates = static_cast<Tpl::PendingDates*> (op);
    if (!pendingDates || pendingDates->isError()) {
        return;
    }

    mPriv->mDates = pendingDates->dates();

    Q_EMIT backFetchable();
}

bool LoggerConversationModel::canFetchMoreBack() const
{
    qDebug() << "LoggerConversationModel::canFetchMoreBack";
    return (mPriv->mDates.size() > 0);
}

void LoggerConversationModel::fetchMoreBack()
{
    qDebug() << "LoggerConversationModel::fetchMoreBack";

    if (mPriv->mBackFetching) {
        qWarning() << "LoggerConversationModel::fetchMoreBack: already fetching";
        return;
    }

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
    if (mPriv->mBackFetching) {
        qWarning() << "LoggerConversationModel::fetchDate: already fetching";
        return;
    }

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
    mPriv->mBackFetching = true;
    Q_EMIT backFetchingChanged();
    connect(pendingEvents, SIGNAL(finished(Tpl::PendingOperation*)), SLOT(onPendingEventsFinished(Tpl::PendingOperation*)));
}

void LoggerConversationModel::onPendingEventsFinished(Tpl::PendingOperation *op)
{
    qDebug() << "LoggerConversationModel::onPendingEventsFinished";

    Tpl::PendingEvents *pendingEvents = static_cast<Tpl::PendingEvents*> (op);
    if (!pendingEvents || pendingEvents->isError()) {
        mPriv->mBackFetching = false;
        Q_EMIT backFetchingChanged();
        return;
    }

    Tpl::EventPtrList eventList = pendingEvents->events();
    Tpl::EventPtr event;

    QList<const Tpy::EventItem *> items;

    beginInsertRows(QModelIndex(), 0, eventList.size() - 1);

    Q_FOREACH(event, eventList) {
        qDebug() << "LoggerConversationModel::onPendingEventsFinished: event added";
        Tpl::TextEventPtr textEvent = event.dynamicCast<Tpl::TextEvent>();
        Tpl::CallEventPtr callEvent = event.dynamicCast<Tpl::CallEvent>();

        Tp::ContactPtr sender;
        if (!event->sender().isNull()) {
            if (event->sender()->identifier() == mPriv->mContact->id()) {
                sender = mPriv->mContact;
            } else if (event->sender()->identifier() == mPriv->mContact->manager()->connection()->selfContact()->id()) {
                sender = mPriv->mContact->manager()->connection()->selfContact();
            }
            if (sender.isNull()) {
                qDebug() << "LoggerConversationModel::onPendingEventsFinished: unknown sender " << event->sender()->identifier();
            }
        }

        Tp::ContactPtr receiver;
        if (!event->receiver().isNull()) {
            if (event->receiver()->identifier() == mPriv->mContact->id()) {
                receiver = mPriv->mContact;
            } else if (event->receiver()->identifier() == mPriv->mContact->manager()->connection()->selfContact()->id()) {
                receiver = mPriv->mContact->manager()->connection()->selfContact();
            }
            if (receiver.isNull()) {
                qDebug() << "LoggerConversationModel::onPendingEventsFinished: unknown receiver " << event->receiver()->identifier();
            }
        }

        if (!textEvent.isNull()) {
            Tpy::TextEventItem *item = new Tpy::TextEventItem(
                sender, receiver, textEvent->timestamp(), textEvent->message(),
                textEvent->messageType(), this);
            items.append(item);
        } else if (!callEvent.isNull()) {
            // convert from Tpl::CallEndReason to Tpy::CallStateChangeReason (very similar)
            Tpy::CallStateChangeReason stateChangeReason = Tpy::CallStateChangeReasonUnknown;
            Tpl::CallEndReason endReason = callEvent->endReason();
            if (endReason == Tpl::CallEndReasonNoAnswer) {
                stateChangeReason = Tpy::CallStateChangeReasonNoAnswer;
            } else if (endReason == Tpl::CallEndReasonUserRequested) {
                stateChangeReason = Tpy::CallStateChangeReasonUserRequested;
            } if (endReason == Tpl::CallEndReasonUnknown) {
                stateChangeReason = Tpy::CallStateChangeReasonUnknown;
            } else {
                // Unknown Tpl::CallEndReason enum
                Q_ASSERT(true);
            }

            Tp::ContactPtr endActor;
            if (!callEvent->endActor().isNull()) {
                if (callEvent->endActor()->identifier() == mPriv->mContact->id()) {
                    endActor = mPriv->mContact;
                } else if (callEvent->endActor()->identifier() == mPriv->mContact->manager()->connection()->selfContact()->id()) {
                    endActor = mPriv->mContact->manager()->connection()->selfContact();
                }
                if (receiver.isNull()) {
                    qDebug() << "LoggerConversationModel::onPendingEventsFinished: unknown endActor " << callEvent->endActor()->identifier();
                }
            }

            Tpy::CallEventItem *item = new Tpy::CallEventItem(
                sender, receiver, callEvent->timestamp(), callEvent->duration(),
                endActor, stateChangeReason, callEvent->detailedEndReason(), this);
            items.append(item);
        }
    }

    if (items.size() > 0) {
        this->insertItems(items);
    }

    endInsertRows();

    mPriv->mBackFetching = false;
    Q_EMIT backFetchingChanged();
    Q_EMIT backFetched(items.size());
}

bool LoggerConversationModel::backFetching() const
{
    return mPriv->mBackFetching;
}
