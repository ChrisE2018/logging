# Logging Readme

The logging library provides a flexible way to trace execution of Arduino programs.
I created this library because I could not find another solution that met
my needs. It is inspired by `log4j` but is designed to work within the 
limitations of the Arduino platform. 

# Basic Configuration

```
logging::StandardFormatter;
logging::SerialAppender appender(Serial, logging::Level::info, &formatter);
logging::Logger::ROOT->add_appender(&appender);
```

# Basic Usage

```
static logging::Logger logger(__FILE__, logging::Level::info);
LOG_INFO(logger, "Hello %s world", "logging");
logger.info() << F("Hello, World ") << std::endl;
logger.info(__LINE__) << F("Hello, World ") << std::endl;
```

# Advanced Configuration

You can attach multiple appenders to any logger to send output to several
places. I have a bluetooth card attached to `Serial3` so I define a
`SerialAppender` for `Serial` (usb) and `Serial3` (bluetooth) so log output
goes to my phone or the host computer or both. An `SdAppender` sends
log output to files on the `microSD` card on pin 53. My clock card works
as a `TimeSource` to supply unix time to a `TimestampFormatter` so everything
has date and time. You will need to install a clock card and include
the `DS3231` library for `TimestampFormatter` to work.

The `SdAppender` sends output to files on a `microSD` card. You will need to include
extra libraries for this to work: `SD`, `SPI` and `Wire`. The `SD` library defines
a 500 byte buffer so this appender increases the sram usage significantly.

Normally all the appenders are attached to the `root` logger which is 
the parent of all other loggers. However, there is nothing special about the 
`root` logger, except that it is is the default parent of new loggers. You can 
construct loggers with explicit parent loggers or supply `nullptr` to make a new 
`root` logger. For example, I have one logger with no parent logger which
is connected to a single `SerialAppender` to avoid the delays associated
with file logging.

Configuration can be done in the `setup` function of your sketch. Be sure 
to allocate objects in the heap rather than the stack, so they 
won't be deleted when the `setup` function returns.

New formatters and appenders are easy to define. Use the source code
for examples.

# Future Versions

Suggestions, requests and contributions are welcome. I have a limited
experience with Arduino, so there are probably many ways this can be
improved. If you want to fix something, feel free to open a pull request
and send me email.

This version does not include any configuration mechanism so you need
to create and link all the objects manually.

The clock and microSD interfaces are based on the specific hardware I have
available. If I learn of more standardized interfaces I might incorporate
those into some future version to be compatible with more hardware varieties.

# Limitations

Internal buffers are shared to minimize the amount of sram required for
logging, so it is not re-entrant.

# Credits

Information from many websites has been used in preparation of this library,
too many to list. 

Implementation of the C++ style stream logging benefited
from http://www.angelikalanger.com/Articles/C++Report/IOStreamsDerivation/IOStreamsDerivation.html
and https://gcc.gnu.org/onlinedocs/libstdc++/manual/streambufs.html.

Jeroen F.J. Laros reviewed the code and provided useful feedback.

The overall concept is based on log4j and log4cxx. Ideas from the Python
logging library has also been incorporated.

