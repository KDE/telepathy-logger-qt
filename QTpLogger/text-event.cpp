/*
 * Copyright (C) 2011 Stefano Sanfilippo <stefano.k.sanfilippo@gmail.com>
 * Copyright (C) 2011 Collabora Ltd. <http://www.collabora.co.uk/>
 *
 * @author Stefano Sanfilippo <stefano.k.sanfilippo@gmail.com>
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

#include "text-event.h"
#include <telepathy-logger/text-event.h>
#include <TelepathyQt4/Account>

using namespace QTpLogger;

TpChannelTextMessageType TextEvent::messageType()
{
    TpChannelTextMessageType mt = tpl_text_event_get_message_type(object<TplTextEvent>());
    return mt;
}

QString TextEvent::message()
{
    const gchar *s = tpl_text_event_get_message(object<TplTextEvent>());
    QString str = QString::fromUtf8(s);
    g_free((gpointer) s);
    return str;
}
