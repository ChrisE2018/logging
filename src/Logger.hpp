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

#pragma once

#include <Arduino.h>
#include <vector>
#include "Appender.hpp"
#include "Level.hpp"
#include "LogBuffer.hpp"

namespace logging
{

class Appender;

#define LOG_ERROR(logger, fmt, args...) logger.logging(logging::Level::error, __LINE__, F(fmt), args);
#define LOG_WARNING(logger, fmt, args...) logger.logging(logging::Level::warning, __LINE__, F(fmt), args);
#define LOG_INFO(logger, fmt, args...) logger.logging(logging::Level::info, __LINE__, F(fmt), args);
#define LOG_DEBUG(logger, fmt, args...) logger.logging(logging::Level::debug, __LINE__, F(fmt), args);
#define LOG_DATA(logger, fmt, args...) logger.logging(logging::Level::data, __LINE__, F(fmt), args);

class Logger
{
        // Share the private buffer to save sram
        friend LogBuffer;

    public:
        static Logger *root;
        Logger (const String name);
        Logger (const String name, const Level level);
        Logger (Logger *parent, const String name, const Level level);
        String get_name () const;
        String get_short_name () const;
        Level get_level () const;
        void add_appender (Appender *const appender);
        LogBuffer& error ();
        LogBuffer& warning ();
        LogBuffer& info ();
        LogBuffer& debug ();
        LogBuffer& data ();
        LogBuffer& error (const int line);
        LogBuffer& warning (const int line);
        LogBuffer& info (const int line);
        LogBuffer& debug (const int line);
        void logging (const Level level, const int line, const char *format, ...);
        void logging (const Level level, const int line, const __FlashStringHelper *format, ...);
        void append (const Logger *logger, const Level level, const int line, const char *message);

    private:
        static const int buffer_size = 128;
        static char buffer[buffer_size];
        static int pos;
        // LogBuffer uses a lot of sram so we only want one instance
        static LogBuffer stream;
        Logger *const parent;
        const String name;
        const String short_name;
        const Level level;

        std::vector<Appender*> appenders;
        String shorten (const String name);
};

//template<typename ... Types>
//void log_error (Logger logger, const int line, const __FlashStringHelper *format, Types ... args)
//{
//    logger.logging(logging::Level::error, line, format, args...);
//}
//
//template<typename ... Types>
//void log_error (Logger logger, const int line, const char *format, Types ... args)
//{
//    logger.logging(logging::Level::error, line, format, args...);
//}
//
//template<typename ... Types>
//void log_warning (Logger logger, const int line, const __FlashStringHelper *format, Types ... args)
//{
//    logger.logging(logging::Level::warning, line, format, args...);
//}
//
//template<typename ... Types>
//void log_warning (Logger logger, const int line, const char *format, Types ... args)
//{
//    logger.logging(logging::Level::warning, line, format, args...);
//}
//
//template<typename ... Types>
//void log_info (Logger logger, const int line, const __FlashStringHelper *format, Types ... args)
//{
//    logger.logging(logging::Level::info, line, format, args...);
//}
//
//template<typename ... Types>
//void log_info (Logger logger, const int line, const char *format, Types ... args)
//{
//    logger.logging(logging::Level::info, line, format, args...);
//}
//
//template<typename ... Types>
//void log_debug (Logger logger, const int line, const __FlashStringHelper *format, Types ... args)
//{
//    logger.logging(logging::Level::debug, line, format, args...);
//}
//
//template<typename ... Types>
//void log_debug (Logger logger, const int line, const char *format, Types ... args)
//{
//    logger.logging(logging::Level::debug, line, format, args...);
//}
//
//template<typename ... Types>
//void log_data (Logger logger, const int line, const __FlashStringHelper *format, Types ... args)
//{
//    logger.logging(logging::Level::data, line, format, args...);
//}
//
//template<typename ... Types>
//void log_data (Logger logger, const int line, const char *format, Types ... args)
//{
//    logger.logging(logging::Level::data, line, format, args...);
//}

}
