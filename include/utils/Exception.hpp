#pragma once

#include <exception>
#include <sstream>
#include <string>
#include <string.h>

#include "UtilityCommon.hpp"


#ifdef _WIN32
    #define __FILENAME_ONLY__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#else
    #define __FILENAME_ONLY__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#endif

#define EXCEPTION_TAG "Exception thrown in "+std::string(__FILENAME_ONLY__)+"::"+std::string(__func__)+", Line="+std::to_string(__LINE__)+": "
#define EXCEPTION_TAG_LINE "Exception thrown in "+std::string(__FILENAME_ONLY__)+"::"+std::string(__func__)+", Line="+std::to_string(__LINE__)+": \n\n"


#define CREATE_EXCEPTION(NAME, MESSAGE) \
    class NAME : public std::exception\
    {\
    public:\
        NAME()\
        {\
            m_message = MESSAGE;\
        }\
        \
        NAME(const std::string & message)\
            : m_message(message)\
        {\
        }\
        \
        ~NAME() throw(){}\
        virtual const char* what() const throw()\
        {\
            return m_message.c_str();\
        }\
    private:\
            std::string m_message;\
    };\

CREATE_EXCEPTION(GenericException, "")
CREATE_EXCEPTION(FeatureNotSupportedException, "Requested feature is not supported")
CREATE_EXCEPTION(FileNotFoundException, "FileNotFoundException")
CREATE_EXCEPTION(FileParseException, "FileParseException")
CREATE_EXCEPTION(FileAccessException, "FileAccessException")
CREATE_EXCEPTION(IndexOutOfRangeException, "Index is out of range")
CREATE_EXCEPTION(InvalidOperationException, "Invalid operation occured")
CREATE_EXCEPTION(InvalidParameterException, "Invalid parameter exception")
CREATE_EXCEPTION(InvalidMapKeyException, "Invalid Map Key")
CREATE_EXCEPTION(InvalidCastException, "Invalid Cast Exception")

#define THROW_GENERIC_EXCEPTION(MESSAGE) throw GenericException(EXCEPTION_TAG_LINE + MESSAGE);
