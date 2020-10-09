#ifndef EXCEPTION_H
#define EXCEPTION_H

#include "vec3.h"
#include <exception>
#include <sstream>

namespace exception
{   
    class divisionByZero : public std::exception
    {
    public:
        divisionByZero(const core::Vec3 &vec, const double &div)
        {
            std::stringstream ss;
            ss << "ERROR #002 : Division by Zero or float close to zero!"
               << "\n DETAILS: "
               << vec << " / " << div << " = ERROR";
            _msg = ss.str();
        }

        const char *what() const noexcept override
        {
            return _msg.c_str();
        }

    private:
        std::string _msg;
    };

    // TODO: Make sure that exception::objectsUnableToImport this error provide enough information about the error occurred
    class objectUnableToImport : public std::exception
    {
    public:
        objectUnableToImport(std::string_view msg)
        {
            _msg = msg;
        }
        const char *what() const noexcept override
        {
            return _msg.c_str();
        }
        private:
        std::string _msg;
    };

    // TODO: Make sure that exception::objectsFileCorruption this error provide enough information about the error occurred
    class objectFileCorruption : public std::exception
    {
    public:

        objectFileCorruption(std::string_view msg)
        {
            _msg = msg;
        }
        const char *what() const noexcept override
        {
            return _msg.c_str();
        }
        private:
        std::string _msg;
    };

    class differentPositions : public std::exception
    {
    public:
        differentPositions(std::string_view msg)
        {
            _msg = msg;
        }
        const char *what() const noexcept override
        {
            return _msg.c_str();
        }

    private:
        std::string _msg;
    };

} // namespace exception

#endif