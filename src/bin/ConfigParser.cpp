#include "ConfigParser.hpp" 
#include <iostream>

void ConfigParser::removeComments(std::string& content) //* Needs some work *
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

void ConfigParser::parseServerBlocks(std::string &content) //* Needs some work *
{
	size_t start = 0, end = 1;
	if(content.find("server", 0) == std::string::npos)
		throw ServerDataExc(EFServer);
	while(start != end && start < content.length())
	{
		start = findStartServer(start, content);
		end = findEndServer(start, content);
		if(start == end)
			throw ServerDataExc(EScope);
		_serverBlocks.push_back(content.substr(start, end - start + 1));
		start = end + 1;
	}
}

size_t ConfigParser::findStartServer(size_t start, std::string &content) //* Needs some work *
{
	size_t i;

	for (i = start; content[i]; i++)
	{
		if (content[i] == 's')
			break ;
		if (!isspace(content[i]))
			throw  ServerDataExc("Wrong character out of server scope{}");
	}
	if (!content[i])
		return (start);
	if (content.compare(i, 6, "server") != 0)
		throw ServerDataExc("Wrong character out of server scope{}");
	i += 6;
	while (content[i] && isspace(content[i]))
		i++;
	if (content[i] == '{')
		return (i);
	else
		throw  ServerDataExc("Wrong character out of server scope{}");

}

size_t ConfigParser::findEndServer(size_t start, std::string &content) //* Needs some work *
{
	size_t	i;
	size_t	scope;
	
	scope = 0;
	for (i = start + 1; content[i]; i++)
	{
		if (content[i] == '{')
			scope++;
		if (content[i] == '}')
		{
			if (!scope)
				return (i);
			scope--;
		}
	}
	return (start);
}