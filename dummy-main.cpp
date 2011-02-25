/*
 * Copyright (C) 2011 Stefano Sanfilippo <stefano.k.sanfilippo@gmail.com>
 *
 * @author Stefano Sanfilippo <stefano.k.sanfilippo@gmail.com>
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

#include <query-log.h>

#include <QtCore/QDebug>

using namespace Logger;

int main(int argc, char **argv)
{
    Logger::Debug e;

    if (argc < 3) {
        qWarning("Must specify a dbus object path as first parameter, and a search term as second!\n");
        exit(0);
    }

    try {
        ChatExistsQuery q1(argv[1]);
        QObject::connect(&q1, SIGNAL(completed(bool)), &e, SLOT(echo(bool)));
        q1.perform(argv[2]);


        ConversationDatesQuery q2(argv[1]);
        QObject::connect(&q2, SIGNAL(completed(QList<QDate>)), &e, SLOT(echo(QList<QDate>)));
        q2.perform(argv[2]);

        MessagesForDateQuery q3(argv[1]);
        QObject::connect(&q3, SIGNAL(completed(QList<Message>)), &e, SLOT(echo(QList<Message>)));
        q3.perform(argv[2]);

    } catch (const Error &e) {
        // qDebug() << e.message();
        exit(1);
    }

    // To ensure all active queries are over...
    sleep(5);

    return 0;
}
