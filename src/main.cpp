#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>

// This includes the target selected by the PIO build environment
// The include error can be dismissed until we find a fix for the dev env.
#include "target.h"

#ifdef TARGET_CLN17_V1_5
#warning "Target: CLN17 v1.5 not yet implemented"
#endif

#ifdef TARGET_CLN17_V2_0
#warning "Target: CLN17 v2 not yet implemented"
#endif

////////// TERMINAL SETUP //////////
#include <CliTerminal.h>
// Set Method limit to 10 pcs
// default is 50
#define METHOD_PCS 10

// Define. 50 is Console Charactor maximum Length
Cli_Terminal terminal(50);

////////// LOGGER SETUP //////////
#include <jblogger.h>
// Create a JBLogger instance with the module name "LOG" and set the log level to LOG_LEVEL_TRACE.
JBLogger logger("TEST", LogLevel::LOG_LEVEL_TRACE);

void change_log_level(String str)
{
    if (str == "debug")
    {
        logger.setLogLevel(LogLevel::LOG_LEVEL_DEBUG);
        logger.info("Log level debug");
    }
    if (str == "error")
    {
        logger.setLogLevel(LogLevel::LOG_LEVEL_ERROR);
        logger.info("Log level error");
    }
    if (str == "info")
    {
        logger.setLogLevel(LogLevel::LOG_LEVEL_INFO);
        logger.info("Log level info");
    }
    if (str == "none")
    {
        logger.setLogLevel(LogLevel::LOG_LEVEL_NONE);
        logger.info("Log level none");
    }
    if (str == "trace")
    {
        logger.setLogLevel(LogLevel::LOG_LEVEL_TRACE);
        logger.info("Log level trace");
    }
    if (str == "warning")
    {
        logger.setLogLevel(LogLevel::LOG_LEVEL_WARNING);
        logger.info("Log level warning");
    }
}

void method(String str)
{
    logger.error("This is an error message");
    logger.warning("This is a warning message");
    logger.info("This is an info message");
    logger.debug("This is a debug message");
    logger.trace("This is a trace message");
}

void setup()
{
    pinMode(PINOUT::LED_GRN, OUTPUT);

    // Setting Command Array by Lamda expression
    // First attribute is Command
    // Second Attribute is Method pointer
    //`str` has Command Attribute forexample, Enter command "lamda abc" str is "abc" or Enter Command "lamda -r abc" str
    // is "-r abc".
    terminal.commands[0] = Command("lamda", [](String str) -> void { SerialUSB.println("Lamda " + str); });
    // or this
    terminal.commands[1] = Command("method", method);
    terminal.commands[2] = Command("loglvl", change_log_level);

    SerialUSB.begin();
    // while (!SerialUSB)
    //     ;
    delay(100);
    SerialUSB.print(">");
}

void loop()
{
    // SerialUSB.printf("Test print, hello world!\n");

    terminal.cli_cal();

    // digitalToggle(PINOUT::LED_GRN);
    // delay(1000);
}
