#ifndef TOOLS_HPP
#define TOOLS_HPP
#include <string>

#define WhiteSpaces " \n\t\v\f\r"

class Tools
{
    public :
        static std::string ToLower(const std::string& str);
        static std::string& Trim(std::string& str, const std::string& trimmerStr);
    private:
};

#endif