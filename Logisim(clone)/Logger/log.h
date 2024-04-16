#ifndef LOG_MACROS_H
#define LOG_MACROS_H

#include <iostream>
#include <string>
#include "logger.h"

#define LOG_FUNCTION(...) \
    do { \
        Logger::GetInstance()->function(__PRETTY_FUNCTION__, "(", __VA_ARGS__, ")"); \
    }while (false)

#define LOG_FUNCTION_NO_ARGS() \
    do { \
        Logger::GetInstance()->function(__PRETTY_FUNCTION__, "( )"); \
    }while (false)

#endif