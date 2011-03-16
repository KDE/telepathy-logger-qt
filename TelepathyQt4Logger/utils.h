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
#ifndef _TelepathyQt4Logger_utils_h_HEADER_GUARD_
#define _TelepathyQt4Logger_utils_h_HEADER_GUARD_

#include <TelepathyQt4/Types>
#include <telepathy-glib/account-manager.h>
#include <telepathy-glib/account.h>
#include <QDebug>

#if __GNUC__ >= 2
#  define __func__ __PRETTY_FUNCTION__
#elif __STDC_VERSION__ >= 199901L
#  define __func__ __FUNCTION__
#else
#  define __func__ "<unknown>"
#endif

#define debug()         qDebug() << "tp-qt4-tpl DEBUG: "
#define debugfn()       qDebug() << "tp-qt4-tpl DEBUG: " << __func__ << " : "
#define warning()       qWarning(); << "tp-qt4-tpl WARN: "
#define warningfn()     qWarning(); << "tp-qt4-tpl WARN: " << __func__ << " : "

namespace Tpl {

class Utils {
public:
    static Utils *instance();

    void setAccountManagerPtr(Tp::AccountManagerPtr accountManager);
    Tp::AccountManagerPtr accountManagerPtr();
    TpAccountManager *tpAccountManager();

    Tp::AccountPtr accountPtr(TpAccount *account);
    Tp::AccountPtr accountPtr(const QString &objectPath);
    TpAccount *tpAccount(Tp::AccountPtr accountPtr);

private:
    Utils();
    ~Utils();

    Tp::AccountManagerPtr mAccountManagerPtr;
};

};

#endif
