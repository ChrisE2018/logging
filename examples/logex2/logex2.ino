#include "Arduino.h"

#include "Logger.hpp"
#include "StandardFormatter.hpp"
#include "TimestampFormatter.hpp"
#include "SerialAppender.hpp"
#include "TimeSource.hpp"
#include <DS3231.h>
#include <ctime>
#include <Wire.h>

class Clock: public logging::TimeSource
{
    public:
        void setup ()
        {
            clock.begin();
        }

        time_t get_unixtime ()
        {
            RTCDateTime dt = clock.getDateTime();
            // @see https://forum.arduino.cc/t/result-of-strftime-is-30-years-off/658302/9
            return dt.unixtime - UNIX_OFFSET + 3600;
        }

    private:
        DS3231 clock;
};

Clock drwho;

static logging::Logger logger(__FILE__, logging::Level::info);
logging::TimestampFormatter formatter(drwho);
logging::SerialAppender appender(Serial, logging::Level::info, formatter);

void setup ()
{
    Serial.begin(9600);
    Serial.println(F("Logging example two"));
    logging::Logger::root->add_appender(&appender);

    drwho.setup();
}

void loop ()
{
    LOG_INFO(logger, "Hello %s world", "logging");
    logger.info() << F("Hello, World ") << std::endl;
    logger.info(__LINE__) << F("Hello, World ") << std::endl;
    Serial.println("here");
    delay(5000);
}
