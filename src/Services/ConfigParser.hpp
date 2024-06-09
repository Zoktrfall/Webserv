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
        void parseServerBlocks(const std::string &content);

        std::string GetBlock(size_t index) const;
        size_t GetBlocksSize(void) const;

    private :
        std::vector<std::string> _serverBlocks;

        size_t findStartBlock(size_t start, const std::string &content);
		size_t findEndBlock(size_t start, const std::string &content);
};

#endif