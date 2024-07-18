#include "Server.hpp"

void Server::RemoveDuplicates(std::vector<std::string>& vec)
{
    std::sort(vec.begin(), vec.end());
    vec.erase(std::unique(vec.begin(), vec.end()), vec.end());
}
void Server::RemoveDuplicates(std::vector<Location>& vec)
{
    std::vector<Location> uniqueLocations;
	bool found;

    for(int i = vec.size() - 1; i >= 0; --i)
	{
        found = false;
        for(size_t j = 0; j < uniqueLocations.size(); ++j)
		{
			std::string path = vec[i].GetPath();
			std::string pathUnique = uniqueLocations[j].GetPath();
            if(Tools::Trim(path, "./") == Tools::Trim(pathUnique, "./"))
			{
                found = true;
                break;
            }
		}

        if(!found)
            uniqueLocations.push_back(vec[i]);
    }
    vec.clear();
    for(int i = uniqueLocations.size() - 1; i >= 0; --i)
        vec.push_back(uniqueLocations[i]);
}
void Server::ClearDuplicates(void)
{
	RemoveDuplicates(_server_names);
	RemoveDuplicates(_indices);
	RemoveDuplicates(_locations);
}
void Server::AddLocation(void) { _locations.push_back(Location()); }
in_addr_t Server::ConvertStringToInAddrT(std::string ipAddress)
{
	std::vector<std::string> bytes;
    std::string byte;
    std::string::size_type pos = 0, nextPos = 0;

	while((nextPos = ipAddress.find('.', pos)) != std::string::npos)
	{
        byte = ipAddress.substr(pos, nextPos - pos);
        bytes.push_back(byte);
        pos = nextPos + 1;
    }

    byte = ipAddress.substr(pos);
    bytes.push_back(byte);
    in_addr_t addr = 0;

    for(size_t i = 0; i < 4; ++i)
	{
        int b = atoi(bytes[i].c_str());
        addr = (addr << 8) | b;
    }

	in_addr_t inAddr = htonl(addr);
    return inAddr == INADDR_NONE ? throw ServerDataExc(EHostIp) : inAddr;
}
void Server::IsValidIPv4(const std::string& host)
{
    std::istringstream ss(host);
    std::string token;
    int dots = 0;
    
    while(std::getline(ss, token, '.'))
	{
        if(dots > 3)
			throw ServerDataExc(EHost);
        
        if(token.empty() || token.size() > 3)
			throw ServerDataExc(EHost);
        
        for(size_t i = 0; i < token.size(); i++)
            if(!std::isdigit(token[i]))
				throw ServerDataExc(EHost);
        
        int num = atoi(token.c_str());
        if(num < 0 || num > 255)
			throw ServerDataExc(EHost);
        
        dots++;
    }
    if(dots != 4)
		throw ServerDataExc(EHost);
}
void Server::SetupErrorPages(void) 
{	
	for(std::map<int, std::string>::iterator it = _error_pages.begin(); it != _error_pages.end(); ++it)
        if(it->second.empty()) 
            it->second = "./www/errors/ErrorPage" + Tools::ToString(it->first) + ".html";
}
void Server::InitErrorPages(void)
{
	_error_pages[301] = "";
	_error_pages[302] = "";
	_error_pages[400] = "";
	_error_pages[401] = "";
	_error_pages[402] = "";
	_error_pages[403] = "";
	_error_pages[404] = "";
	_error_pages[405] = "";
	_error_pages[406] = "";
	_error_pages[413] = "";
	_error_pages[500] = "";
	_error_pages[501] = "";
	_error_pages[502] = "";
	_error_pages[503] = "";
	_error_pages[505] = "";
}
Server::Server(void) :
    _ports(),
    _hosts(),
    _server_names(),
    _root(""),
    _client_max_body_size(-1),
	_upload_dir(""),
    _autoindex(-1),
	_return(),
    _indices(),
    _locations() {
        InitErrorPages();
}

const std::vector<uint16_t>& Server::GetPorts(void) const { return _ports; }
const std::vector<in_addr_t>& Server::GetHosts(void) const { return _hosts; }
const std::string& Server::GetRoot(void) const { return _root; }
long long Server::GetClientMaxBodySize(void) const { return _client_max_body_size; }
const std::vector<std::string>& Server::GetServerNames(void) const { return _server_names; }
Location& Server::GetLocation(size_t i) { return _locations[i]; }
int Server::GetAutoIndex(void) const { return _autoindex; }
const std::vector<std::string>& Server::GetIndices(void) const { return _indices; }
std::map<int, std::string> Server::GetReturn(void) const { return _return; }
std::string Server::GetErrorPage(int ErrorCode) { return _error_pages[ErrorCode]; }
std::string Server::GetUploadDir(void) const { return _upload_dir; }

void Server::SetPort(std::string& part)
{
	unsigned long port;
	std::string host = "", portStr = "";
	size_t colonPos = part.find(':');
	if(colonPos != std::string::npos)
	{
		host = part.substr(0, colonPos);
		if(host == "localhost")
			host = "127.0.0.1";
		Server::IsValidIPv4(host);
		in_addr_t inAddr = ConvertStringToInAddrT(host);
		_hosts.push_back(inAddr);
		portStr = part.substr(colonPos + 1);
	}
	else
	{
		in_addr_t inAddr = ConvertStringToInAddrT("0.0.0.0");
		_hosts.push_back(inAddr);
		portStr = part;
	}
	
	if(portStr.empty() || portStr.size() > 5 || !Tools::IsAllDigits(portStr))
		throw ServerDataExc(EPortSyntax);

	port = Tools::StrToULong(portStr);
	if (port < 1 || port > 65535)
		throw ServerDataExc(EPortSyntax);
	
	for(size_t i = 0; i < _ports.size(); i++)
		if(_ports[i] == static_cast<uint16_t>(port))
			throw ServerDataExc(EPortDuplicate);
	_ports.push_back(static_cast<uint16_t>(port));
}
void Server::SetRoot(std::string& root) { _root = root; }
void Server::SetServerNames(std::string& server_name)
{
	if(server_name.empty())
		throw ServerDataExc(EServerName);

	const std::string invalid_chars = "!@#$%^&*()=+[]{}|\\:;'\"<>?/, ";
	std::istringstream iss(server_name);
    std::string name;

    while(iss >> name)
	{
   		if(name.find_first_of(invalid_chars) != std::string::npos || 
			name.find('_') != std::string::npos || isdigit(name[0]) ||
			(name[0] == '-' || name[name.size() - 1] == '-') ||
			Tools::IsUpperCase(name))
			throw ServerDataExc(EServerName);

		_server_names.push_back(name);
    }
}
void Server::SetClientMaxBodySize(std::string client_max_body_size) { _client_max_body_size = Tools::CheckClientMaxBodySize(client_max_body_size); }
void Server::SetAutoIndex(std::string autoIndex) { _autoindex = Tools::CheckAutoIndex(autoIndex); }
void Server::SetIndices(std::string indices) { _indices = Tools::CheckIndices(indices); }
void Server::SetErrorPages(std::string& errorPage)
{
	size_t colonPos = errorPage.find(' ');
	if(colonPos == std::string::npos)
		throw ServerDataExc(EEPage);

	int ErrorCode = std::atoi(errorPage.substr(0, colonPos).c_str());

	if(ErrorCode < 100 || ErrorCode > 599)
		throw ServerDataExc(EEPage);

	if(!_error_pages[ErrorCode].empty())
		throw ServerDataExc(EEPageDublicate);

	std::string value = errorPage.substr(colonPos + 1);
	Tools::AccessStat(S_IFREG, R_OK, EEPage, value);

	_error_pages[ErrorCode] = value;
}
void Server::SetReturn(std::string& ret) { _return = Tools::InitReturn(ret); }
void Server::SetUploadDir(std::string upload_dir) { Tools::AccessStat(S_IFDIR, W_OK, EUDir, upload_dir); _upload_dir = upload_dir; }