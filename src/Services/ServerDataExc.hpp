#ifndef SERVER_DATA_EXC_HPP
#define SERVER_DATA_EXC_HPP
#include <stdexcept>
#include <string>

/* Errors related to the configuration file */
#define ENotFile "Something is wrong with the configuration file"
#define EAccess "Permission denied while accessing the configuration file"
#define EOpenFile "Unable to open file"

/* Errors related to the syntax */
#define ESyntaxSem "Syntax error related to ';'"
#define ESyntaxTo(x) "Syntax error related to " #x
#define ESyntaxV "Invalid directive value"
#define EUDirective "Undefined directive"
#define EFServer "No 'server' block found"
#define EScope "Problem with scope"
#define EOutScope "Wrong character out of server scope{}"
#define ERequiredParam "Missing required paraneters"
#define EServerSetup "Inconsistent Server Setup"
#define ENEPage "No Error Page"

/* Errors related to directives */
#define EPortSyntax "Wrong syntax: listen"
#define EPortDuplicate "Duplicated: port"
#define EHost "Wrong syntax: host"
#define EHostIp "IP address in in_addr_t format: host"

#define ERootSyntax "Wrong syntax: root"
#define ERootDublicate "Duplicated: root"

#define EServerName "Wrong syntax: server_name"
#define EServerNameDuplicate "Duplicated: server_name"

#define EClientSize "Wrong syntax: client_max_body_size"
#define EClientSizeDublicate "Duplicated: client_max_body_size"

#define EAutoIndex "Wrong syntax: autoindex"
#define EAutoIndexDublicate "Duplicated: autoindex"

#define EIndex "Wrong syntax: index"
#define EIndexDublicate "Duplicated: index"

#define EEPage "Wrong syntax: error_page"
#define EEPageDublicate "Duplicated: error_page"

#define EReturn "Wrong syntax: return"
#define EReturnDublicate "Duplicated: return"

#define EAlias "Wrong syntax: alias"
#define EAliasDublicate "Duplicated: alias"

#define ELimit "Wrong syntax: limit_except"
#define ELimitDublicate "Duplicated: limit_except"

#define ELocation "Wrong syntax: location"

#define ECGIPath "Wrong syntax: cgi_path"
#define ECGIPathDublicate "Duplicated: cgi_path"

#define ECGIExt "Wrong syntax: cgi_ext"
#define ECGIExcDublicate "Duplicated: cgi_ext"

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