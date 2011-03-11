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
#ifndef _TelepathyQt4Logger_types_h_HEADER_GUARD_
#define _TelepathyQt4Logger_types_h_HEADER_GUARD_

#include <TelepathyQt4Logger/Global>
#include <TelepathyQt4/Types>
#include <QtCore/QList>
#include <QtCore/QDate>

namespace Tpl
{

enum EntityType
{
    //codegen: EntityTypeUnknown=ENTITY_UNKNOWN,EntityTypeContact=ENTITY_CONTACT,EntityTypeRoom=ENTITY_ROOM,EntityTypeSelf=ENTITY_SELF
    EntityTypeUnknown,
    EntityTypeContact,
    EntityTypeRoom,
    EntityTypeSelf
};

enum LogManagerError
{
    //xodegen: LogManagerErrorAddEvent=TPL_LOG_MANAGER_ERROR_ADD_EVENT
    LogManagerErrorAddEvent
};

enum EventTypeMask
{
    //codegen: EventTypeMaskText=EVENT_MASK_TEXT,EventTypeMaskAny=EVENT_MASK_ANY
    EventTypeMaskText = 1 << 0,
    EventTypeMaskAny = 0xffff
};

class PendingDates;
class PendingEvents;
class PendingEntities;
class PendingSearch;
struct SearchHit;

typedef QList<Tp::ContactPtr> ContactPtrList;
typedef QList<EntityPtr> EntityPtrList;
typedef QList<EventPtr> EventPtrList;
typedef QList<QDate> QDateList;
typedef QList<SearchHit> SearchHitList;

typedef bool (*LogEventFilter)(EventPtr event, void *user_data);

} //namespace

QTELEPATHYQT4LOGGER_REGISTER_TYPE(Tpl::EntityType)
QTELEPATHYQT4LOGGER_REGISTER_TYPE(Tpl::LogManagerError)
QTELEPATHYQT4LOGGER_REGISTER_TYPE(Tpl::EventTypeMask)

#endif
