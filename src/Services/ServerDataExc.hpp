#ifndef SERVER_DATA_EXC_HPP
#define SERVER_DATA_EXC_HPP
#include <stdexcept>
#include <string>

#define NotFileMessage "The argument is not a file"
#define EStats "Error getting file stats"
#define EAcces "Permission denied"
#define EOpenFile "Unable to open file"

class ServerDataExc : public std::exception
{
    public :
        ~ServerDataExc() throw () {}
        ServerDataExc(const std::string& errorMessage);
        virtual const char* what(void) const throw();
    private :
        std::string _errorMessage;
};

#endif