/*
 * This file is part of TelepathyQt4Logger
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

#include <QApplication>
#include <TelepathyQt4/Account>
#include <TelepathyQt4Logger/Entity>
#include <TelepathyQt4Logger/LogManager>
#include <TelepathyQt4Logger/Init>
#include <glib-object.h>
#include <QGst/Init>

class TplToolApplication : public QCoreApplication {
public:
    TplToolApplication(int &argc, char **argv);

    Tp::AccountPtr accountPtr(const QString &id);
    Tpl::EntityPtr entityPtr(const QString &id);

    bool parseArgs();
private:

};

TplToolApplication::TplToolApplication(int &argc, char **argv)
    : QCoreApplication(argc, argv)
{
}

Tp::AccountPtr TplToolApplication::accountPtr(const QString &id)
{
    return Tp::AccountPtr();
}

Tpl::EntityPtr TplToolApplication::entityPtr(const QString &id)
{
    return Tpl::EntityPtr();
}

bool TplToolApplication::parseArgs()
{
    QStringList args = arguments();

    if (args.size() < 3) {
        return false;
    }

    Tpl::LogManagerPtr logManager = Tpl::LogManager::instance();
    if (logManager.isNull()) {
        qWarning() << "LogManager not found";
        return false;
    }

    if (args.at(0) == "exists") {
        Tp::AccountPtr account = accountPtr(args.at(1));
        if (account.isNull()) {
            qWarning() << "Account not found " << args.at(1);
        }

        Tpl::EntityPtr entity = entityPtr(args.at(2));
        if (entity.isNull()) {
            qWarning() << "Entity not found " << args.at(2);
        }

        bool ret = logManager->exists(account, entity, Tpl::EventTypeMaskAny);
        qDebug() << "tpl-tool " << args.at(1) << args.at(2) << " -> " << ret;
    } else if (args.at(0) == "entities") {
    } else if (args.at(0) == "dates") {
    } else if (args.at(0) == "events") {
    } else if (args.at(0) == "filteredEvents") {
    } else if (args.at(0) == "search") {
    }

    return true;
}


int main(int argc, char **argv)
{
    g_type_init();

    TplToolApplication app(argc, argv);
    Tpl::init();

    if (!app.parseArgs()) {
        qDebug() << "Telepathy logger command line tool (qt4)";
        qDebug() << "";
        qDebug() << "General usage: tpl-tool <command> <parameters>";
        qDebug() << "";
        qDebug() << "tpl-tool exists <account> <entity>";
        qDebug() << "tpl-tool entities <account>";
        qDebug() << "tpl-tool dates <account> <entity>";
        qDebug() << "tpl-tool events <account> <entity>";
        qDebug() << "tpl-tool filteredEvents <account> <entity> <numEvents>";
        qDebug() << "tpl-tool search <text>";
        exit(0);
    }

    return app.exec();
}
