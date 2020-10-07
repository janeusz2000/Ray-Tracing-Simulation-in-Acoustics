#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <exception>
#include <sstream>
#include "vec3.h"

namespace exception
{
    class invalidConstructor : public std::exception
    {
    public:
        const char *what() const noexcept override
        {
            return "ERROR #001 : Invalid input for constructor!";
        }
    };

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

    class invalidArgumentSize : public std::exception
    {
    public:
        const char *what() const noexcept override
        {
            return "ERROR #003 : Invalid argument size!";
        }
    };

    class objectUnableToImport : public std::exception
    {
    public:
        const char *what() const noexcept override
        {
            return "ERROR #004 : Unable to import object!";
        }
    };

    class objFileCorruption : public std::exception
    {
    public:
        const char *what() const noexcept override
        {
            return "ERROR #005 : .obj file corrupted!";
        }
    };

    class differentPositions : public std::exception
    {
    public:
        const char *what() const noexcept override
        {
            return "ERROR #006 : energy Collectors doesn't have the same origin positions";
        }
    };

    class outOfSize : public std::exception
    {
    public:
        outOfSize(const size_t &xIter, const size_t &yIter, const size_t &rayNumLimit)
        {
            std::stringstream ss;
            ss << "ERROR: Out of index: xIter: " << xIter << " of:  " << rayNumLimit - 1 << ", yIter: " << yIter << " of: " << rayNumLimit - 1;
            _msg = ss.str();
        }
        const char *what() const noexcept override
        {
            return _msg.c_str();
        }

    private:
        std::string _msg;
    }
} // namespace exception

#endif