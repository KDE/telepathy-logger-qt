#ifndef __HIT__
#define __HIT__

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

#include <QtCore/QObject>

namespace QTpLogger
{

class TplLogSearchHit;

class SearchHitPrivate
{
public:
    SearchHitPrivate(TplLogSearchHit *hit) { (void)hit; } //TODO unknown type
};

class SearchHit : public QObject
{
public:
    SearchHit() {}

private:
    SearchHit(SearchHitPrivate *_d) : d(_d) {}

    friend class KeywordQuery;

    SearchHitPrivate *d;
};

} //namespace

#endif // __HIT__
