#include "ConfigParser.hpp" 
#include <iostream>

void ConfigParser::removeComments(std::string& content)
{
    size_t pos = content.find('#');
    while(pos != std::string::npos)
    {
        size_t posEnd = content.find('\n', pos);
        if(posEnd == std::string::npos)
        {
            content.erase(pos);
            break;
        }
        content.erase(pos, posEnd - pos + 1);
        pos = content.find('#');
    }
}
size_t ConfigParser::findStartBlock(size_t start, std::string &content)
{
	size_t i = start;
	for(; content[i]; i++)
	{
		if(content[i] == 's')
			break ;
		if(!isspace(content[i]))
			throw ServerDataExc(EOutScope);
	}
	if(!content[i])
		return (start);
	if(content.compare(i, 6, "server") != 0)
		throw ServerDataExc(EOutScope);
	i += 6;
	while(content[i] && isspace(content[i]))
		i++;
	if(content[i] == '{')
		return i;
	throw ServerDataExc(EOutScope);
}
size_t ConfigParser::findEndBlock(size_t start, std::string &content)
{
	size_t scope = 0;
	for(size_t i = start + 1; content[i]; i++)
	{
		if(content[i] == '{')
			scope++;
		if(content[i] == '}')
		{
			if(!scope)
				return i;
			scope--;
		}
	}
	return start;
}
void ConfigParser::parseServerBlocks(std::string &content)
{
	size_t start = 0, end = 1;
	if(content.find("server", 0) == std::string::npos)
		throw ServerDataExc(EFServer);
	while(start != end && start < content.length())
	{
		start = findStartBlock(start, content);
		end = findEndBlock(start, content);
		if(start == end)
			throw ServerDataExc(EScope);
		_serverBlocks.push_back(content.substr(start, end - start + 1));
		start = end + 1;
	}
	content.clear();
}