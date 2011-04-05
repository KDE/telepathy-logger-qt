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
#ifndef _TelepathyQt4Logger_Models_logger_conversation_model_h_HEADER_GUARD_
#define _TelepathyQt4Logger_Models_logger_conversation_model_h_HEADER_GUARD_

#ifndef IN_TELEPATHY_QT4_LOGGER_MODELS_HEADER
#error IN_TELEPATHY_QT4_LOGGER_MODELS_HEADER
#endif

#include <QAbstractListModel>
#include <QStringList>
#include <TelepathyQt4/Types>
#include <TelepathyQt4Logger/Entity>
#include <TelepathyQt4Logger/PendingOperation>
#include <TelepathyQt4Logger/Types>
#include <TelepathyQt4Yell/Models/ConversationItem>
#include <TelepathyQt4Yell/Models/AbstractConversationModel>

namespace Tpl {

class LoggerConversationModel : public Tpy::AbstractConversationModel
{
    Q_OBJECT
    Q_PROPERTY(bool backFetching READ backFetching NOTIFY backFetchingChanged)

public:
    LoggerConversationModel(const Tp::AccountPtr &account, const Tp::ContactPtr &contact, QObject *parent = 0);
    virtual ~LoggerConversationModel();

    Q_INVOKABLE virtual bool canFetchMoreBack() const;
    Q_INVOKABLE virtual void fetchMoreBack();
    virtual bool backFetching() const;

    virtual bool canFetchMore(const QModelIndex & index) const;
    virtual void fetchMore(const QModelIndex & index);

Q_SIGNALS:
    void backFetchable();
    void backFetched(int numItems);
    void backFetchingChanged() const;

private Q_SLOTS:
    void onPendingDatesFinished(Tpl::PendingOperation *op);
    void onPendingEventsFinished(Tpl::PendingOperation *op);

 private:
    void setup() const;
    void fetchDate(const QDate &date) const;

    struct Private;
    friend struct Private;
    Private *mPriv;
};

}

#endif
