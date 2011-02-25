#include <query-log.h>

#include <QtCore/QDebug>

using namespace Logger;

int main(int argc, char **argv)
{
    Logger::Debug e;

    if(argc < 3) {
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
    } catch(Error *e) {
        qDebug() << e->message();
        delete e;
        exit(1);
    }

    // To ensure all active queries are over...
    sleep(5);

    return 0;
}
