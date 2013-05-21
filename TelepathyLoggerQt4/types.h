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
#ifndef _TelepathyLoggerQt4_types_h_HEADER_GUARD_
#define _TelepathyLoggerQt4_types_h_HEADER_GUARD_

#ifndef IN_TELEPATHY_LOGGER_QT4_HEADER
#error IN_TELEPATHY_LOGGER_QT4_HEADER
#endif

#include <TelepathyLoggerQt4/Global>
#include <TelepathyQt/Types>
#include <QtCore/QList>
#include <QtCore/QDate>

namespace Tpl
{

enum EntityType
{
    //codegen: EntityTypeUnknown=ENTITY_UNKNOWN,EntityTypeContact=ENTITY_CONTACT,EntityTypeRoom=ENTITY_ROOM,EntityTypeSelf=ENTITY_SELF
    EntityTypeUnknown, /**< the current contact's type is unknown **/
    EntityTypeContact, /**< the the contact's type represents a user (buddy), but not
                         the account's owner for which EntityTypeSelf is used **/
    EntityTypeRoom,    /**< a named room **/
    EntityTypeSelf     /**< the contact's type represents the owner of the account
                         whose channel has been logged, as opposed to 
                         EntityTypeContact which represents any other user */
};

enum LogManagerError
{
    LogManagerErrorAddEvent /**  Error returned when adding logs fails **/
};

/**
 * \brief Mask used to filter type of Tpl::Event returned.
 */
enum EventTypeMask
{
    //codegen: EventTypeMaskText=EVENT_MASK_TEXT,EventTypeMaskCall=EVENT_MASK_CALL,EventTypeMaskAny=EVENT_MASK_ANY
    EventTypeMaskText = 1 << 0, /**< Mask to Tpl::TextEvent **/
    EventTypeMaskCall = 1 << 1, /**< Mask to Tpl::CallEvent **/
    EventTypeMaskAny = 0xffff   /**< Special value to select all type of Tpl::Event **/
};

class Logger;
class LogWalker;
class PendingDates;
class PendingEvents;
class PendingEntities;
class PendingOperation;
class PendingSearch;
struct SearchHit;

typedef Tp::SharedPtr<Logger> LoggerPtr;
typedef QList<Tp::ContactPtr> ContactPtrList;
typedef QList<EntityPtr> EntityPtrList;
typedef QList<EventPtr> EventPtrList;
typedef QList<QDate> QDateList;
typedef QList<SearchHit> SearchHitList;

typedef bool (*LogEventFilter)(const EventPtr &event, void *user_data);

} //namespace

#endif
