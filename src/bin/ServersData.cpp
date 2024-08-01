#include "ServersData.hpp"

ServersData::ServersData(const char* configFilePath) : _configFile(configFilePath), _content(""), _servers() {}
bool ServersData::SetupServersData(void)
{
    try
    {
        _content = _configFile.ProcessConfigFile();
        ConfigParser::RemoveComments(Tools::Trim(_content, WhiteSpaces));
        ConfigParser::ParseServerBlocks(_content);
        _content.clear();
        ServersData::LoadServers();
    }
    catch(const ServerDataExc& exp)
    {
        Logger::LogMsg(ERROR, exp.what());
        return false;
    }
    return true;
}
void ServersData::LoadServers(void)
{
    for(size_t i = 0; i < ConfigParser::GetBlocksSize(); i++)
	{
		Server server;
		CreateServer((ConfigParser::GetBlock(i)), server);
		_servers.push_back(server);
	}
    if(_servers.size() > 1)
		HasConflicts();
}
void ServersData::CreateServer(std::string block, Server& Server)
{
    bool inLocation = false;
    int LIndex = -1;
    std::vector<std::string> Lines = ConfigParser::SplitConfigLines(block + "\n", "\n");
    if(Lines.size() < 2)
        throw ServerDataExc(ERequiredParam);
    
    for(size_t i = 0; i < Lines.size(); i++)
    {
        std::string key, value;
        key = value = "";
		if(!Lines[i].compare(0, 6, "listen"))
        {
            if(inLocation)
                throw ServerDataExc(ESyntaxTo(listen));
        
            ConfigParser::ItinKeyValue(key, value, Lines[i]);
            if(key != "listen")
                throw ServerDataExc(ESyntaxTo(listen));
            Server.SetPort(value);
        }
        else if(!Lines[i].compare(0, 4, "root"))
        {
            if(inLocation && !Server.GetLocation(LIndex).GetRoot().empty())
                throw ServerDataExc(ERootDublicate);
            else if(!inLocation && !Server.GetRoot().empty())
                throw ServerDataExc(ERootDublicate);

            ConfigParser::ItinKeyValue(key, value, Lines[i]);
            if(key != "root")
                throw ServerDataExc(ESyntaxTo(root));

            if(inLocation)
                Server.GetLocation(LIndex).SetRoot(value);
            else
                Server.SetRoot(value);
        }
        else if(!Lines[i].compare(0, 11, "server_name"))
        {
            if(inLocation)
                throw ServerDataExc(ESyntaxTo(server_name));
            
            if(Server.GetServerNames().size())
                throw ServerDataExc(EServerNameDuplicate);

            ConfigParser::ItinKeyValue(key, value, Lines[i]);
            if(key != "server_name")
                throw ServerDataExc(ESyntaxTo(server_name));
            Server.SetServerNames(value);
        }
        else if(!Lines[i].compare(0, 20, "client_max_body_size"))
        {
            if(inLocation && Server.GetLocation(LIndex).GetClientMaxBodySize() != -1)
                throw ServerDataExc(EClientSizeDublicate);
            else if(!inLocation && Server.GetClientMaxBodySize() != -1)
                throw ServerDataExc(EClientSizeDublicate);

            ConfigParser::ItinKeyValue(key, value, Lines[i]);
            if(key != "client_max_body_size")
                throw ServerDataExc(ESyntaxTo(client_max_body_size));

            if(inLocation)
                Server.GetLocation(LIndex).SetClientMaxBodySize(value);
            else
                Server.SetClientMaxBodySize(value);
        }
        else if(!Lines[i].compare(0, 9, "autoindex"))
        {
            if(inLocation && Server.GetLocation(LIndex).GetAutoindex() != -1)
                throw ServerDataExc(EAutoIndexDublicate);
            else if(!inLocation && Server.GetAutoIndex() != -1)
                throw ServerDataExc(EAutoIndexDublicate);
            
            ConfigParser::ItinKeyValue(key, value, Lines[i]);
            if(key != "autoindex")
                throw ServerDataExc(ESyntaxTo(autoindex));

            if(inLocation)
                Server.GetLocation(LIndex).SetAutoindex(value);
            else
                Server.SetAutoIndex(value);
        }
        else if(!Lines[i].compare(0, 5, "index"))
        {
            if(inLocation && Server.GetLocation(LIndex).GetIndices().size())
                throw ServerDataExc(EIndexDublicate);
            else if(!inLocation && Server.GetIndices().size())
                throw ServerDataExc(EIndexDublicate);

            ConfigParser::ItinKeyValue(key, value, Lines[i]);
            if(key != "index")
                throw ServerDataExc(ESyntaxTo(index));

            if(inLocation)
                Server.GetLocation(LIndex).SetIndices(value);
            else
                Server.SetIndices(value);
        }
        else if(!Lines[i].compare(0, 10, "error_page"))
        {
            if(inLocation)
                throw ServerDataExc(ESyntaxTo(error_page));

            ConfigParser::ItinKeyValue(key, value, Lines[i]);
            if(key != "error_page")
                throw ServerDataExc(ESyntaxTo(error_page));

            Server.SetErrorPages(value);
        }
        else if(!Lines[i].compare(0, 8, "location"))
        {
            if(inLocation || Lines[i + 1] != "{")
                throw ServerDataExc(ESyntaxTo(location));
            
            ConfigParser::ItinKeyValue(key, value, Lines[i++]);
            if(key != "location")
                throw ServerDataExc(ESyntaxTo(location));

            inLocation = true;
            Server.AddLocation();
            LIndex++;
            Server.GetLocation(LIndex).SetPath(value);
        }
        else if(Lines[i] == "}")
            inLocation = false;
        else if(!Lines[i].compare(0, 8, "cgi_path"))
        {
            if(!inLocation)
                throw ServerDataExc(ESyntaxTo(cgi_path));

            if(Server.GetLocation(LIndex).GetCgiPath().size())
                throw ServerDataExc(ECGIPathDublicate);

            ConfigParser::ItinKeyValue(key, value, Lines[i]);
            if(key != "cgi_path")
                throw ServerDataExc(ESyntaxTo(cgi_path));

            Server.GetLocation(LIndex).SetCgiPath(value);
        }
        else if(!Lines[i].compare(0, 7, "cgi_ext"))
        {
            if(!inLocation)
                throw ServerDataExc(ESyntaxTo(cgi_ext));

            if(Server.GetLocation(LIndex).GetCgiExt().size())
                throw ServerDataExc(ECGIExcDublicate);

            ConfigParser::ItinKeyValue(key, value, Lines[i]);
            if(key != "cgi_ext")
                throw ServerDataExc(ESyntaxTo(cgi_ext));

            Server.GetLocation(LIndex).SetCgiExt(value);
        }
        else if(!Lines[i].compare(0, 6, "return"))
        {
            if(inLocation && Server.GetLocation(LIndex).GetReturn().size())
                throw ServerDataExc(EReturnDublicate);
            else if(!inLocation && Server.GetReturn().size())
                throw ServerDataExc(EReturnDublicate);

            ConfigParser::ItinKeyValue(key, value, Lines[i]);
            if(key != "return")
                throw ServerDataExc(ESyntaxTo(return));

            if(inLocation)
                Server.GetLocation(LIndex).SetReturn(value);
            else
                Server.SetReturn(value);
        }
        else if(!Lines[i].compare(0, 5, "alias"))
        {
            if(!inLocation)
                throw ServerDataExc(ESyntaxTo(alias));

            if(!Server.GetLocation(LIndex).GetAlias().empty())
                throw ServerDataExc(EAliasDublicate);

            ConfigParser::ItinKeyValue(key, value, Lines[i]);
            if(key != "alias")
                throw ServerDataExc(ESyntaxTo(alias));

            Server.GetLocation(LIndex).SetAlias(value);
        }
        else if(!Lines[i].compare(0, 12, "limit_except"))
        {
            if(!inLocation)
                throw ServerDataExc(ESyntaxTo(limit_except));

            if(Server.GetLocation(LIndex).GetAlias().size())
                throw ServerDataExc(ELimitDublicate);

            ConfigParser::ItinKeyValue(key, value, Lines[i++]);
            if(key != "limit_except")
                throw ServerDataExc(ESyntaxTo(limit_except));

            Server.GetLocation(LIndex).SetLimitExcept(value);
            if(Lines[i++] != "{" || Lines[i++] != "deny all;" || Lines[i] != "}")
                throw ServerDataExc(ESyntaxTo(limit_except));
        }
        else
            throw ServerDataExc(EUDirective);
    }
    CheckServers(Server);
}
void ServersData::CheckServers(Server& Server)
{
    if(!Server.GetPorts().size() || Server.GetRoot().empty())
        throw ServerDataExc(ERequiredParam);
    if(!Server.GetIndices().size())
        Server.SetIndices("./www/html/index.html");
    if(Server.GetClientMaxBodySize() == -1)
        Server.SetClientMaxBodySize("1m");
    if(Server.GetAutoIndex() == -1)
        Server.SetAutoIndex("off");

    Server.SetupErrorPages();
    Server.ClearDuplicates();
}
bool ServersData::HasIntersection(const std::vector<uint16_t>& vec1, const std::vector<uint16_t>& vec2)
{
    for(size_t i = 0; i < vec1.size(); ++i)
        for(size_t j = 0; j < vec2.size(); ++j)
            if(vec1[i] == vec2[j])
                return true;
    return false;
}
void ServersData::HasConflicts(void)
{
	std::vector<Server>::iterator it1, it2;

	for(it1 = this->_servers.begin(); it1 != this->_servers.end() - 1; it1++)
		for(it2 = it1 + 1; it2 != this->_servers.end(); it2++)
			if(HasIntersection(it1->GetPorts(), it2->GetPorts()))
				throw ServerDataExc(EServerSetup);
}
std::vector<Server>& ServersData::GetServers(void) { return _servers; }