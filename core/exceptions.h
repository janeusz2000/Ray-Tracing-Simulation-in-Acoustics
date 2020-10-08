#ifndef EXCEPTION_H
#define EXCEPTION_H

#include "vec3.h"
#include <exception>
#include <sstream>

namespace exception
{
    class invalidConstructor : public std::exception
    {
    public:
        const char *what() const noexcept override
        {
            // TODO: this message does not provide any useful debugging information. Add some,
            // so that when reading the error massage later in the logs, you know what is
            // going on.
            //
            // Please fix all the exceptions in this file.
            return "ERROR #001 : Invalid input for constructor!";
        }
    };

    // TODO: this exception has a very generic name, yet it seems it is about vectors
    // and normalization in the case of zero length, so please change the name to be
    // more specific.
    //
    // Also: why this exception is not thrown from the Vec3 itself when the division
    // happens?
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

    // TODO: same here, generic name, yet specific usage. Please make names more specific.
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
    };
} // namespace exception

#endif