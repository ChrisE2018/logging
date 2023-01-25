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

#include "Appender.hpp"
#include "Logger.hpp"
#include "LogBuffer.hpp"

#include <WString.h>

namespace logging
{

Logger *Logger::root = new Logger(nullptr, "root", Level::debug);

char Logger::buffer[Logger::buffer_size];

int Logger::pos = 0;

LogBuffer Logger::stream;

Logger::Logger (const String name) :
        Logger(Logger::root, name, Level::info)
{
}

Logger::Logger (const String name, const Level level) :
        Logger(Logger::root, name, level)
{
}

Logger::Logger (Logger *parent, const String name, const Level level) :
        parent(parent), name(name), short_name(shorten(name)), level(level)
{
}

String Logger::get_name () const
{
    return name;
}

String Logger::get_short_name () const
{
    return short_name;
}

String Logger::shorten (const String name)
{
    String filename = name;
    const int slash_pos = filename.lastIndexOf('/');
    if (slash_pos >= 0)
    {
        filename = filename.substring(slash_pos + 1);
    }
    const int dot_pos = filename.lastIndexOf('.');
    if (dot_pos >= 0)
    {
        filename = filename.substring(0, dot_pos);
    }
    return filename;
}

Level Logger::get_level () const
{
    return level;
}

void Logger::add_appender (Appender *const appender)
{
    if (appender != nullptr)
    {
        if (std::find(appenders.begin(), appenders.end(), appender) == appenders.end())
        {
            appenders.push_back(appender);
        }
    }
}

LogBuffer& Logger::error ()
{
    stream.set_logger(this, Level::error, 0);
    return stream;
}

LogBuffer& Logger::warning ()
{
    stream.set_logger(this, Level::warning, 0);
    return stream;
}

LogBuffer& Logger::info ()
{
    stream.set_logger(this, Level::info, 0);
    return stream;
}

LogBuffer& Logger::debug ()
{
    stream.set_logger(this, Level::debug, 0);
    return stream;
}

LogBuffer& Logger::data ()
{
    stream.set_logger(this, Level::data, 0);
    return stream;
}

LogBuffer& Logger::error (const int line)
{
    stream.set_logger(this, Level::error, line);
    return stream;
}

LogBuffer& Logger::warning (const int line)
{
    stream.set_logger(this, Level::warning, line);
    return stream;
}

LogBuffer& Logger::info (const int line)
{
    stream.set_logger(this, Level::info, line);
    return stream;
}

LogBuffer& Logger::debug (const int line)
{
    stream.set_logger(this, Level::debug, line);
    return stream;
}

void Logger::logging (const Level _level, const int line, const char *format, ...)
{
    if (_level <= level)
    {
        va_list args;
        va_start(args, format);
        vsnprintf(buffer, buffer_size, format, args);
        va_end(args);
        append(this, _level, line, buffer);
    }
}

void Logger::logging (const Level _level, const int line, const __FlashStringHelper *format, ...)
{
    if (_level <= level)
    {
        va_list args;
        va_start(args, format);
        vsnprintf_P(buffer, buffer_size, (const char*) format, args);
        va_end(args);
        append(this, _level, line, buffer);
    }
}

void Logger::append (const Logger *logger, const Level _level, const int line, const char *message)
{
    for (Appender *appender : appenders)
    {
        appender->append(logger, _level, line, message);
    }
    if (parent != nullptr)
    {
        parent->append(logger, _level, line, message);
    }
}

}

