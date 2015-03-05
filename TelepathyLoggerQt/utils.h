/*
 * This file is part of TelepathyLoggerQt
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
#ifndef _TelepathyLoggerQt_utils_h_HEADER_GUARD_
#define _TelepathyLoggerQt_utils_h_HEADER_GUARD_

#include "global.h"

#include <TelepathyLoggerQt_export.h>

#include <TelepathyQt/Types>
#include <telepathy-glib/account-manager.h>
#include <telepathy-glib/account.h>

#include <QtCore/QDebug>

namespace Tpl {

class TELEPATHY_LOGGER_QT_NO_EXPORT Utils {
public:
    static Utils *instance();

    void setAccountManagerPtr(const Tp::AccountManagerPtr & accountManager);
    Tp::AccountManagerPtr accountManagerPtr();
    TpAccountManager *tpAccountManager();

    Tp::AccountPtr accountPtr(TpAccount *account);
    Tp::AccountPtr accountPtr(const QString &objectPath);
    TpAccount *tpAccount(const Tp::AccountPtr & accountPtr);

private:
    Utils();
    ~Utils();

    Tp::AccountManagerPtr mAccountManagerPtr;
};

};

#endif
