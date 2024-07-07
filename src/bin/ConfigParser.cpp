#include "ConfigParser.hpp" 

void ConfigParser::RemoveComments(std::string& content)
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
size_t ConfigParser::FindStartBlock(size_t start, const std::string& content)
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
size_t ConfigParser::FindEndBlock(size_t start, const std::string& content)
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
void ConfigParser::ParseServerBlocks(const std::string &content)
{
	size_t start = 0, end = 1;
	if(content.find("server", 0) == std::string::npos)
		throw ServerDataExc(EFServer);
	while(start != end && start < content.length())
	{
		start = FindStartBlock(start, content);
		end = FindEndBlock(start, content);
		if(start == end)
			throw ServerDataExc(EScope);
		_serverBlocks.push_back(content.substr(start, end - start + 1));
		start = end + 1;
	}
}
std::vector<std::string> ConfigParser::SplitConfigLines(const std::string& line, const std::string& delimiter)
{
    std::vector<std::string> str;
	std::string::size_type start, end;

	start = end = 0;
    while(true)
	{
        end = line.find_first_of(delimiter, start);
        if(end == std::string::npos)
            break;
        std::string subStr = line.substr(start, end - start);
        std::string tmp = Tools::Trim(subStr, WhiteSpaces);
        if((tmp.compare(0, 8, "location") == 0 && tmp.find("{") != std::string::npos) ||
			(tmp.compare(0, 12, "limit_except") == 0 && tmp.find("{") != std::string::npos))
        {
            str.push_back(Tools::Trim(subStr = tmp.substr(0, tmp.length() - 1), WhiteSpaces));
			tmp = Tools::Trim(subStr = tmp.substr(tmp.length() - 1), WhiteSpaces);
        }
        str.push_back(tmp);
        start = line.find_first_not_of(delimiter, end);
        if(start == std::string::npos)
            break;
    }
    str.erase(str.begin());
    str.pop_back();
	str.erase(std::remove_if(str.begin(), str.end(), Tools::IsWhiteSpace), str.end());
    return str;
}
std::string ConfigParser::GetBlock(size_t index) const { return _serverBlocks[index]; }
size_t ConfigParser::GetBlocksSize(void) const { return _serverBlocks.size(); }
void ConfigParser::ItinKeyValue(std::string& key, std::string& value, std::string& line)
{
	size_t pos = line.find(" ");
	if(pos == std::string::npos)
		throw ServerDataExc(ESyntaxV);
	key = line.substr(0, pos);
	value = line.substr(pos + 1);
	
	if(key != "location" && key != "limit_except")
		Tools::CheckSemicolon(value);
	Tools::Trim(value, WhiteSpaces);
}