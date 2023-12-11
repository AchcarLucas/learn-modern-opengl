#ifndef EXCEPTION_HPP
#define EXCEPTION_HPP

class Exception : public std::runtime_error
{
    public:
        Exception(std::string const& msg) : std::runtime_error(msg) {}
};

#endif // EXCEPTION_HPP
