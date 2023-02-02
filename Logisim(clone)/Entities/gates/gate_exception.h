#ifndef GATE_EXCEPTION_H
#define GATE_EXCEPTION_H

class GateException : public std::exception
{
public:
    GateException(const std::string& msg) : m_msg(msg) {}
   ~GateException() {}

   virtual const char* what() const throw ()
   {
        return m_msg.c_str();
   }

   const std::string m_msg;
};

#endif
