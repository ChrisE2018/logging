/*
 * Copyright 2023 Christopher Eliot
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <locale>
#include <cstdio>
#include "LogBuffer.hpp"
#include "Logger.hpp"

std::ostream& operator<< (std::ostream &lhs, const __FlashStringHelper *pstr)
{
    PGM_P p = reinterpret_cast<PGM_P>(pstr);
    while (true)
    {
        unsigned char c = pgm_read_byte(p++);
        if (c == 0)
        {
            return lhs;
        }
        lhs.put(c);
    }
}

namespace logging
{

LogBuffer::LogBuffer () :
        logger(nullptr), level(Level::info), std::ios(0), std::ostream(this)
{
}

std::streambuf::int_type LogBuffer::overflow (const std::streambuf::int_type c)
{
    if (c == '\n' || c == EOF || logging::Logger::pos + 1 >= Logger::buffer_size)
    {
        flush();
    }
    else if (c != '\r')
    {
        Logger::buffer[Logger::pos++] = c;
    }
    Logger::buffer[Logger::pos] = '\0';
    return c;
}

void LogBuffer::flush ()
{
    logger->append(logger, level, line, Logger::buffer);
    reset();
}

void LogBuffer::reset ()
{
    Logger::pos = 0;
    Logger::buffer[0] = '\0';
}

void LogBuffer::set_logger (Logger *const _logger, const Level _level, const int _line)
{
    logger = _logger;
    level = _level;
    line = _line;
}

}
