#ifndef __TYPES__
#define __TYPES__

/*
 * Copyright (C) 2011 Collabora Ltd. <http://www.collabora.co.uk/>
 *
 * @author Mateu Batle <mateu.batle@collabora.co.uk>
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

namespace QTpLogger
{

enum EntityType
{
    EntityTypeUnknown,
    EntityTypeContat,
    EntityTypeRoom,
    EntityTypeSelf
};

enum LogManagerError
{
    LogManagerErrorAddEvent
};

enum EventTypeMask
{
    EventTypeMaskText = 1 << 0,
    EventTypeMaskAny = 0xffff
};

class PendingDates;
class PendingEvents;

typedef QList<Tp::ContactPtr> ContactPtrList;
typedef QList<EventPtr> EventPtrList;
typedef QList<QDate> QDateList;

typedef bool (*LogEventFilter)(EventPtr event, void *user_data);

} //namespace

#endif // __ENTITY__
