#ifndef CONFIG_PARSER_HPP
#define CONFIG_PARSER_HPP
#include <string>
#include <vector>
#include "ServerDataExc.hpp"

class ConfigParser  //* Needs some work *
{
    public :
		virtual ~ConfigParser() {};

        void removeComments(std::string& content);
        void parseServerBlocks(std::string &content);

        size_t findStartServer(size_t start, std::string &content);
		size_t findEndServer(size_t start, std::string &content);

    private :
        std::vector<std::string> _serverBlocks;
};

#endif