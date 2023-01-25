#include <Arduino.h>
#include <ArduinoSTL.h>

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

static logging::Logger logger(__FILE__, logging::Level::info);

void setup ()
{
    Serial.begin(115200);
    Serial.println(F("Logging example two"));
    Clock *drwho = new Clock();
    drwho->setup();
    logging::TimestampFormatter *formatter = new logging::TimestampFormatter(drwho);
    logging::SerialAppender *appender = new logging::SerialAppender(Serial, logging::Level::info, formatter);
    logging::Logger::root->add_appender(appender);
}

void loop ()
{
    LOG_INFO(logger, "Hello %s world", "logging");
    logger.info() << F("Hello, World ") << std::endl;
    logger.info(__LINE__) << F("Hello, World ") << std::endl;
    Serial.println("here");
    delay(5000);
}
