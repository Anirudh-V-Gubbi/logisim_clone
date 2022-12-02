#ifndef LOGGER_EXCEPTION_H
#define LOGGER_EXCEPTION_H

class LoggerException : public std::exception
{
public:
    LoggerException(const std::string& msg) : m_msg(msg) {}
   ~LoggerException() {}

   virtual const char* what() const throw ()
   {
        return m_msg.c_str();
   }

   const std::string m_msg;
};

#endif
