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
#include <TelepathyQt4Logger/PendingOperation>

class TplToolApplication : public QCoreApplication {
    Q_OBJECT
public:
    TplToolApplication(int &argc, char **argv);

    Tp::AccountPtr accountPtr(const QString &id);
    Tpl::EntityPtr entityPtr(const QString &id);

    bool parseArgs1();
    bool parseArgs2();

private Q_SLOTS:
    void onAccountManagerReady(Tp::PendingOperation*);
    void onAccountReady(Tp::PendingOperation*);
    void onConnectionReady(Tp::PendingOperation*);
    void onPendingSearch(Tpl::PendingOperation*);
    void onPendingEntities(Tpl::PendingOperation*);

private:
    Tp::AccountManagerPtr mAccountManager;
    Tp::AccountPtr mAccountPtr;
};

