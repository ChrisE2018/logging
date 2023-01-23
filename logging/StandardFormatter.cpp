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

#include <stdio.h>
#include "StandardFormatter.hpp"

namespace logging
{

StandardFormatter::StandardFormatter () :
        Formatter()
{
}

void StandardFormatter::format (char *buffer, const size_t buffer_size, const Logger *const logger, const Level level,
        const int line, const char *const message)
{
    const unsigned long now = millis();
    const unsigned long seconds = now / 1000;
    const int ms = now % 1000;
    snprintf(buffer, buffer_size, "%lu.%03d [%s %s:%d] %s", seconds, ms, stringify(level),
            logger->get_short_name().c_str(), line, message);
    buffer[buffer_size - 1] = '\0';
}

}
