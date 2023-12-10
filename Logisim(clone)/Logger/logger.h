#ifndef LOGGER_H
#define LOGGER_H

#include "logger_exception.h"

#include <iostream>
#include <time.h>
#include <stdexcept>
#include <string>
#include <sstream>

class Logger {
public:
    Logger(std::string name) {
        if(m_logger != nullptr) {
            throw LoggerException("Cannot reinstantiate Logger");
        }
        m_logger = this;
        _name = name;
    }
    ~Logger() {
        m_logger = nullptr;
    }
    
    static Logger* GetInstance() {
        if(m_logger == nullptr) {
            throw LoggerException("No instance of Logger could be found");
        }
        
        return m_logger;
    }
    
    template<typename... Args>
    void info(const Args&... args)
    {
        #ifdef ENABLE_LOGGER
        std::ostringstream oss;
        oss << "[INFO]" << " [" << getTime() << "] " << this->_name << ": ";
        log(oss, args...);
        std::cout << oss.str() << std::endl;
        #endif
    }

    template<typename... Args>
    void warn(const Args&... args)
    {
        #ifdef ENABLE_LOGGER
        std::ostringstream oss;
        oss << "[WARN]" << " [" << getTime() << "] " << this->_name << ": ";
        log(oss, args...);
        std::cout << oss.str() << std::endl;
        #endif
    }

    template<typename... Args>
    void error(const Args&... args)
    {
        #ifdef ENABLE_LOGGER
        std::ostringstream oss;
        oss << "[ERR]" << " [" << getTime() << "] " << this->_name << ": ";
        log(oss, args...);
        std::cout << oss.str() << std::endl;
        #endif
    }
    
private:
    static inline Logger* m_logger = nullptr;
    std::string _name;
    
    std::string getTime()
    {
        std::string timeNow(15, '\0');
        time_t t;
        struct tm* tmp;
        time(&t);

        tmp = localtime(&t);
        strftime(&timeNow[0], 15, "%r", tmp);

        return timeNow;
    }
    
    void log(std::ostringstream& oss) { }

    template<typename First, typename... Rest>
    void log(std::ostringstream& oss, const First& first, const Rest&... rest) {
        oss << first << " ";
        log(oss, rest...);
    }
};

#endif
