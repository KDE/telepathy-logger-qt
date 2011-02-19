#include <query-log.h>

#include <QDebug>

using namespace Logger;

int main(int argc, char **argv)
{
  Echo e;

  if (argc < 3)
  {
    qWarning("Must specify a dbus object path as first parameter, and a search term as second!");
    exit(0);
  }

  try
  {
    ChatExistsQuery q(argv[1]);
	QObject::connect(&q, SIGNAL(completed(bool)), &e, SLOT(echo(bool)));
    q.perform(argv[2]);
  }
  catch (Error *e)
  {
    qDebug() << e->message();
    delete e;
    exit(1);
  }

  return 0;
}
