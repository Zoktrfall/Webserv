#ifndef RESPONSE_HPP
#define RESPONSE_HPP
#include <string>
#include <iostream>
#include "HttpTypes.hpp"

class Response
{
    public :

    private :
        short _statusCode;
        std::string _reasonPhrase;
        size_t _contentLength;

};



#endif