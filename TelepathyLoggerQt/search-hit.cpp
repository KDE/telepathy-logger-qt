/*
 * This file is part of TelepathyLoggerQt
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

#include <TelepathyLoggerQt/SearchHit>

#include <TelepathyQt/Account>
#include <TelepathyLoggerQt/Entity>
#include <QDate>

using namespace Tpl;

struct TELEPATHY_LOGGER_QT_NO_EXPORT SearchHit::Private : public QSharedData
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

SearchHit::SearchHit(const SearchHit &other)
    : mPriv(other.mPriv)
{
}

SearchHit & SearchHit::operator=(const SearchHit &other)
{
    mPriv = other.mPriv;
    return *this;
}

SearchHit::~SearchHit()
{
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



