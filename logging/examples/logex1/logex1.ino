#include "Arduino.h"

#include "Logger.hpp"
#include "StandardFormatter.hpp"
#include "SerialAppender.hpp"

static logging::Logger logger(__FILE__, logging::Level::info);
logging::StandardFormatter formatter;
logging::SerialAppender appender(Serial, logging::Level::info, formatter);

void setup ()
{
    Serial.begin(115200);
    Serial.println(F("Logging example one"));
    logging::Logger::root->add_appender(&appender);
}

void loop ()
{
    LOG_INFO(logger, "Hello %s world", "logging");
    logger.info() << F("Hello, World ") << std::endl;
    logger.info(__LINE__) << F("Hello, World ") << std::endl;
    Serial.println("here");
    delay(5000);
}
