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

#include <TelepathyLoggerQt4/CallEvent>

#include <TelepathyLoggerQt4/Entity>
#include <telepathy-logger/call-event.h>

using namespace Tpl;

QTime CallEvent::duration() const
{
    GTimeSpan timeSpan = tpl_call_event_get_duration(object<TplCallEvent>());
    int hh = timeSpan / 3600;
    timeSpan = timeSpan % 3600;
    int mm = timeSpan / 60;
    int ss = timeSpan % 60;
    int ms = 0;
    // FIXME there is no timespan type in Qt yet, use QTime in the meanwhile
    QTime ret(hh,mm,ss,ms);
    return ret;
}

EntityPtr CallEvent::endActor() const
{
    TplEntity * entity = tpl_call_event_get_end_actor(object<TplCallEvent>());
    return EntityPtr::wrap(entity, true);
}

CallEndReason CallEvent::endReason() const
{
    CallEndReason er = (CallEndReason) tpl_call_event_get_end_reason(object<TplCallEvent>());
    return er;
}

QString CallEvent::detailedEndReason() const
{
    const gchar *s = tpl_call_event_get_detailed_end_reason(object<TplCallEvent>());
    QString str = QString::fromUtf8(s);
    return str;
}
