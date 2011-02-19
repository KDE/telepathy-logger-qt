#include <query-log.h>

#include <QDebug>

using namespace Logger;

int main(int argc, char **argv)
{
	if (argc < 2)
	{
		qWarning("Must specify a dbus object path as first parameter!");
		exit(0);
	}

	try
	{
		ChatExistsQuery q(argv[1]);
		q.perform("Search  text");
	}
	catch (Error *e)
	{
		qDebug() << e->message();
		delete e;
		exit(1);
	}

	return 0;
}
