/*
 * This file is part of TelepathyLoggerQt4
 *
 * Copyright (C) 2011 Stefano Sanfilippo <stefano.k.sanfilippo@gmail.com>
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

#include <TelepathyLoggerQt4/TextEvent>

#include <telepathy-logger/text-event.h>

using namespace Tpl;

Tp::ChannelTextMessageType TextEvent::messageType() const
{
    return (Tp::ChannelTextMessageType) tpl_text_event_get_message_type(object<TplTextEvent>());
}

QString TextEvent::message() const
{
    const gchar *s = tpl_text_event_get_message(object<TplTextEvent>());
    QString str = QString::fromUtf8(s);
    return str;
}

QString TextEvent::messageToken() const
{
    const gchar *s = tpl_text_event_get_message_token(object<TplTextEvent>());
    QString str = QString::fromUtf8(s);
    return str;
}

QDateTime TextEvent::editTimestamp() const
{
    // FIXME See http://bugs.freedesktop.org/show_bug.cgi?id=21690
    uint seconds = (uint) tpl_text_event_get_edit_timestamp(object<TplTextEvent>());
    QDateTime dateTime;
    dateTime.setTime_t(seconds);
    return dateTime;
}

QString TextEvent::supersedesToken() const
{
    const gchar *s = tpl_text_event_get_supersedes_token(object<TplTextEvent>());
    QString str = QString::fromUtf8(s);
    return str;
}


QList< TextEventPtr > TextEvent::supersedes() const
{
    GList *tplEvents = tpl_text_event_get_supersedes(object<TplTextEvent>());
    GList *iter;
    QList<TextEventPtr> events;
    for (iter = tplEvents; iter; iter = g_list_next(iter)) {
        events << TextEventPtr::wrap((TplTextEvent*) iter->data, true);
    }

    return events;
}
