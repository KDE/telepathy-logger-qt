/*
 * This file is part of TelepathyLoggerQt4
 *
 * Copyright (C) 2011 Collabora Ltd. <http://www.collabora.co.uk/>
 * Copyright (C) 2012 David Edmundson <kde@davidedmundson.co.uk>
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

#include <TelepathyLoggerQt4/SearchHit>

#include <TelepathyQt/Account>
#include <TelepathyLoggerQt4/Entity>
#include <QDate>

using namespace Tpl;

struct TELEPATHY_LOGGER_QT4_NO_EXPORT SearchHit::Private
{
    Tp::AccountPtr account;
    EntityPtr target;
    QDate date;
};

SearchHit::SearchHit(const Tp::AccountPtr &account, const Tpl::EntityPtr &target, const QDate &date) :
    mPriv(new Private())
{
    mPriv->account = account;
    mPriv->target = target;
    mPriv->date = date;
}

Tp::AccountPtr SearchHit::account() const
{
    return mPriv->account;
}

EntityPtr SearchHit::target() const
{
    return mPriv->target;
}

QDate SearchHit::date() const
{
    return mPriv->date;
}



