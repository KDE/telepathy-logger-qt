#ifndef __FILTER_QUERY__
#define __FILTER_QUERY__

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

#include <QTpLogger/Query>
#include <QTpLogger/Entry>

namespace QTpLogger
{

class FilterQuery : public Query
{

Q_OBJECT

public:
    explicit FilterQuery(const QString &dbusid);

public Q_SLOTS:
    void perform(const QString &chatid, bool ischatroom);

Q_SIGNALS:
	 void completed(const QList<Entry> &messages);

private:
    static void callback(void *obj, void *result, FilterQuery *self);

	QList<Entry> messages;
};

} //namespace

#endif // __FILTER_QUERY__
