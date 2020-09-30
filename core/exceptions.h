#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <exception>
#include <sstream>

#include "obj/objects.h"

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

    class differentPosition : public std::exception
    {
    public:
        differentPosition(const object::EnergyCollector &collector1, const object::EnergyCollector &collector2)
        {
            _collector1 = collector1;
            _collector2 = collector2;
        }

        const char *what() const noexcept override
        {
            _ss << "ERROR #006 at: " << _collector1 << " and " << _collector2 << " Collectors have different position \n"
                << "ID " << _collector1.getID() << ": " << _collector1.getOrigin() << "\n"
                << "ID " << _collector2.getID() << ": " << _collector2.getOrigin();
            return _ss.str().c_str();
        }

    private:
        object::EnergyCollector _collector1;
        object::EnergyCollector _collector2;
        std::stringStream _ss;
    }
} // namespace exception

#endif