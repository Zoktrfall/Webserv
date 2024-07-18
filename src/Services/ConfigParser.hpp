#ifndef CONFIG_PARSER_HPP
#define CONFIG_PARSER_HPP
#include <string>
#include <vector>
#include <algorithm>
#include "Tools.hpp"
#include "ServerDataExc.hpp"

class ConfigParser
{
    public :
		virtual ~ConfigParser() {};

        void RemoveComments(std::string& content);
        void ParseServerBlocks(const std::string &content);
        std::vector<std::string> SplitConfigLines(const std::string& line, const std::string& delimiter);
        void ItinKeyValue(std::string& key, std::string& value, std::string& line);

        std::string GetBlock(size_t index) const;
        size_t GetBlocksSize(void) const;

    private :
        std::vector<std::string> _serverBlocks;

        size_t FindStartBlock(size_t start, const std::string &content);
		size_t FindEndBlock(size_t start, const std::string &content);
};

#endif