#ifndef ENTITY_EXCEPTION_H
#define ENTITY_EXCEPTION_H

class EntityException : public std::exception
{
public:
    EntityException(const std::string& msg) : m_msg(msg) {}
   ~EntityException() {}

   virtual const char* what() const throw ()
   {
        return m_msg.c_str();
   }

   const std::string m_msg;
};

#endif
