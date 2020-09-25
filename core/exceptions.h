#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <exception>

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
        const char *what() const noexcept override
        {
            return "ERROR #002 : Division by Zero or float close to zero!";
        }
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
} // namespace exception

#endif