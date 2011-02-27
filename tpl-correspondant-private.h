#ifndef __CORRESPONDANT_PRIVATE__
#define __CORRESPONDANT_PRIVATE__

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

#include <telepathy-logger/log-manager.h>

#include <Logger/tpl-correspondant.h>

#include <QtCore/QString>
#include <QtCore/QHash>

namespace Logger
{

class CorrespondantPrivateData
{
public:
    CorrespondantPrivateData(TplEntity *chat);
    ~CorrespondantPrivateData();

    QString alias();
    QString id();
    QString avatar();
    Correspondant::Whois type();

private:
    QString _alias;
    QString _id;
    QString _avatar;
    Correspondant::Whois _type;

    static QHash<TplEntityType, Correspondant::Whois> tplToCorrespondantWhoisHash;
    static bool first_object;
};

} //namespace

#endif // __CORRESPONDANT_PRIVATE__