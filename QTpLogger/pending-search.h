/*
 * This file is part of QTpLogger
 *
 * Copyright (C) 2011 Collabora Ltd. <http://www.collabora.co.uk/>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef __PENDING_SEARCH__
#define __PENDING_SEARCH__

#include <QTpLogger/LogManager>
#include <QTpLogger/PendingOperation>
#include <QTpLogger/Entity>

namespace QTpLogger
{

struct SearchHit
{
  Tp::AccountPtr account;
  EntityPtr target;
  QDate date;
};

class PendingSearch : public QTpLogger::PendingOperation
{
    Q_OBJECT
    Q_DISABLE_COPY(PendingSearch);

public:
    ~PendingSearch();

    virtual void start();
    SearchHitList hits() const;
\
private:
    friend class LogManager;

    PendingSearch(LogManagerPtr manager, const QString &text, EventTypeMask typeMask);

    struct Private;
    friend struct Private;
    Private *mPriv;
};

} // QTpLogger

#endif // __PENDING_SEARCH__