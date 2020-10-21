#ifndef EXCEPTION_H
#define EXCEPTION_H

#include "vec3.h"
#include <exception>
#include <sstream>

namespace exception
{   
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