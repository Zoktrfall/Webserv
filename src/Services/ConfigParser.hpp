#ifndef CONFIG_PARSER_HPP
#define CONFIG_PARSER_HPP
#include <string>
#include <vector>
#include "ServerDataExc.hpp"

class ConfigParser
{
    public :
		virtual ~ConfigParser() {};

        void removeComments(std::string& content);
        void parseServerBlocks(std::string &content);

    private :
        std::vector<std::string> _serverBlocks;

        size_t findStartBlock(size_t start, std::string &content);
		size_t findEndBlock(size_t start, std::string &content);
};

#endif