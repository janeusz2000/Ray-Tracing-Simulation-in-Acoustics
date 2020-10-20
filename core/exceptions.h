#ifndef EXCEPTION_H
#define EXCEPTION_H

#include "vec3.h"
#include <exception>
#include <sstream>

namespace exception
{   
    // TODO: Change all doubles to float
    // TODO: change underscore to the end

    // TODO: Make sure that exception::objectsUnableToImport this error provide enough information about the error occurred
    class objectUnableToImport : public std::exception
    {
    public:
        objectUnableToImport(std::string_view msg)
        {
            msg_ = msg;
        }
        const char *what() const noexcept override
        {
            return msg_.c_str();
        }
        private:
        std::string msg_;
    };

    // TODO: Make sure that exception::objectsFileCorruption this error provide enough information about the error occurred
    class objectFileCorruption : public std::exception
    {
    public:

        objectFileCorruption(std::string_view msg)
        {
            msg_ = msg;
        }
        const char *what() const noexcept override
        {
            return msg_.c_str();
        }
        private:
        std::string msg_;
    };

    class differentPositions : public std::exception
    {
    public:
        differentPositions(std::string_view msg)
        {
            msg_ = msg;
        }
        const char *what() const noexcept override
        {
            return msg_.c_str();
        }

    private:
        std::string msg_;
    };

} // namespace exception

#endif