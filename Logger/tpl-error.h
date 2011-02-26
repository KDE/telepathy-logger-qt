#ifndef __TP_LOGGER_ERROR__
#define __TP_LOGGER_ERROR__

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

namespace Logger
{

class Error
{
public:
    Error(const QString &message, int code = 0);
    Error(GError *gerror, bool dontfree = false);

    QString message()
    {
        return this->_message;
    }

    int code()
    {
        return this->_code;
    }

private:
    QString _message;
    int _code;
};

} // namespace

#endif // __TP_LOGGER_ERROR__