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

#include <QTpLogger/Log>

#include <QApplication>
#include <QtCore/QDebug>

using namespace QTpLogger;

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    QTpLogger::Debug e;

    if (argc < 3) {
        qWarning("Must specify a dbus object path as first parameter, and a search term as second!\n");
        exit(0);
    }

    try {
        // Query objects must be declared in same block as QueryMainLoop
        // (as they will return const refs)
        ChatExistsQuery q1(argv[1]);
        ConversationDatesQuery q2(argv[1]);
        MessagesForDateQuery q3(argv[1]);
        KeywordQuery q4(argv[1]);
        ChatsForAccountQuery q5(argv[1]);

        // Debugging/Example connections
        QObject::connect(&q1, SIGNAL(completed(bool)), &e, SLOT(echo(bool)));
        QObject::connect(&q2, SIGNAL(completed(QList<QDate>)), &e, SLOT(echo(QList<QDate>)));
        QObject::connect(&q3, SIGNAL(completed(QList<Entry>)), &e, SLOT(echo(QList<Entry>)));
        QObject::connect(&q4, SIGNAL(completed(QList<SearchHit>)), &e, SLOT(echo(QList<SearchHit>)));
        QObject::connect(&q5, SIGNAL(completed(QList<Entity>)), &e, SLOT(echo(QList<Entity>)));

        // Perform all queries...
        q1.perform(argv[2], true);
        q2.perform(argv[2], true);
        q3.perform(argv[2], true);
        q4.perform(argv[2]);
        q5.perform();

        // Run main loop...
        return app.exec();

    } catch (const Error &e) {
        qDebug() << e.message();
        exit(1);
    }

    return -1;
}
