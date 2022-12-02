#ifndef LOGGER_H
#define LOGGER_H

#include "logger_exception.h"

#include <iostream>
#include <time.h>
#include <stdexcept>
#include <string>

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
    
    void info(const char* info)
    {
        std::cout << "[" << getTime() << "]" << " " << this->_name << ": " << info << '\n';
    }

    void warn(const char* warning)
    {
        std::cout << "[" << getTime() << "]" << " " << this->_name << ": " << warning << '\n';
    }

    void error(const char* error)
    {
        std::cout << "[" << getTime() << "]" << " " << this->_name << ": " << error << '\n';
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
};

#endif
