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
#ifndef _TelepathyLoggerQt4_Models_logger_conversation_model_h_HEADER_GUARD_
#define _TelepathyLoggerQt4_Models_logger_conversation_model_h_HEADER_GUARD_

#ifndef IN_TELEPATHY_LOGGER_QT4_MODELS_HEADER
#error IN_TELEPATHY_LOGGER_QT4_MODELS_HEADER
#endif

#include <TelepathyLoggerQt4/Models/Global>
#include <TelepathyQt4Yell/Models/AbstractConversationModel>
#include <TelepathyQt4/Types>
#include <TelepathyLoggerQt4/PendingOperation>

namespace Tpl {

class TELEPATHY_LOGGER_QT4_MODELS_EXPORT LoggerConversationModel
    : public Tpy::AbstractConversationModel
{
    Q_OBJECT
    Q_PROPERTY(bool backFetching READ backFetching NOTIFY backFetchingChanged)

public:
    LoggerConversationModel(const Tp::AccountPtr &account, const Tp::ContactPtr &contact, QObject *parent = 0);
    virtual ~LoggerConversationModel();

    /*
        Next methods are very similar to canFetchMore / fetchMore already present in Qt models.
        The difference is that already existing methods work just for scrolling forwards.
        For scrolling backwards, neither the views nor the models have a way to support
        on-demand feeding of information, since it is not a normal use case.
        However for chat history, normally you see the last portion (newest information) and
        you might want to scroll backwards to feed more data in in reverse chronological order.

        backFetching is a new method that checks if there is an already exiting request being
        executed. We prevent to execute multiple requests concurrently.
     */
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
