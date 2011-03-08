#ifndef __LOGMANAGER__
#define __LOGMANAGER__

/*
 * Copyright (C) 2011 Stefano Sanfilippo <stefano.k.sanfilippo@gmail.com>
 * Copyright (C) 2011 Collabora Ltd. <http://www.collabora.co.uk/>
 *
 * @author Stefano Sanfilippo <stefano.k.sanfilippo@gmail.com>
 * @author Mateu Batle <mateu.batle@collabora.co.uk>
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

#include <QGlib/Object>
#include "global.h"
#include <TelepathyQt4/Types>
#include <QTpLogger/Types>

namespace QTpLogger
{

//TODO
/*
struct LogSearchHit
{
    TpAccount *account;
    TplEntity *target;
    GDate *date;
};
*/

//TODO
//typedef gboolean (*TplLogEventFilter) (TplEvent *event, gpointer user_data);

/*! \headerfile log-manager.h <QTpLogger/LogManager>
 * \brief Wrapper class for TplLogManager
 */
class LogManager : public QGlib::Object
{
    QTPLOGGER_WRAPPER(LogManager)
public:
    static LogManagerPtr instance();

    //GType tpl_log_manager_get_type (void);
    bool exists(Tp::AccountPtr account, EntityPtr target, EventTypeMask type) const;

    PendingDates *queryDates(Tp::AccountPtr account, EntityPtr entity, EventTypeMask typeMask);
    PendingEvents *queryEvents(Tp::AccountPtr account, EntityPtr entity, EventTypeMask typeMask,
        QDate date);
    PendingEvents *queryFilteredEvents(Tp::AccountPtr account, EntityPtr entity,
        EventTypeMask typeMask, uint numEvents, LogEventFilter filterFunction,
        void *filterFunctionUserData);
    PendingEntities *queryEntities(Tp::AccountPtr account);

    //PendingSearch *search() const;
};

} //namespace

QTPLOGGER_REGISTER_TYPE(QTpLogger::LogManagerError)
QTPLOGGER_REGISTER_TYPE(QTpLogger::EventTypeMask)
QTPLOGGER_REGISTER_TYPE(QTpLogger::LogManager)

#endif // __LOGMANAGER__
