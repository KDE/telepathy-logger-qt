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

#if 0
#include <Logger/tpl-query-main-loop.h>

#include <glib.h>

namespace Logger {

class QueryMainLoopPrivateData
{
public:
    GMainLoop *handle;
};

} //namespace

using namespace Logger;

// TODO maybe use a singleton?

QueryMainLoop::QueryMainLoop(QObject *parent) : QObject(parent)
{
    this->d = new QueryMainLoopPrivateData;
    this->d->handle = g_main_loop_new(NULL, false);
}

QueryMainLoop::~QueryMainLoop()
{
    this->stop();
    delete this->d;
}

void QueryMainLoop::exec()
{
    // Run only if it wasn't stopped before...
    if (!this->d->handle) return;

    g_main_loop_run(this->d->handle);
}

void QueryMainLoop::stop()
{
    // Do it only once...
    if (!this->d->handle) return;

    // Since once is stopped, it cannot be resumed, get rid of it.
    g_main_loop_quit(this->d->handle);
    g_main_loop_unref(this->d->handle);

    // Set the specific pointer to NULL
    this->d->handle = NULL;
}

void QueryMainLoop::fatal(const QString &error)
{
    this->stop();

    (void)error; //FIXME
}

// void QueryMainLoop::stopIfRunning()
// {
//     if (g_main_loop_is_running(this->d->handle)) {
//         this->d->stop();
//     }
// }

#endif