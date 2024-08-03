#include "Tools.hpp"

bool Tools::IsWhiteSpace(const std::string& s)
{
    for(std::string::const_iterator it = s.begin(); it != s.end(); ++it)
        if(!std::isspace(*it))
            return false;
    return true;
}
bool Tools::IsUpperCase(const std::string& str)
{
    for(std::string::const_iterator it = str.begin(); it != str.end(); ++it)
        if(isupper(*it))
            return true;
    return false;
}
std::string Tools::ToLower(const std::string& str)
{
	std::string lowercasedStr = "";
	size_t idx = 0;

	while(idx < str.size())
		lowercasedStr += std::tolower(str[idx++]);

	return lowercasedStr;
}
std::string& Tools::Trim(std::string& str, const std::string& trimmerStr)
{
	size_t startPos = str.find_first_not_of(trimmerStr);
	size_t endPos = str.find_last_not_of(trimmerStr);

	if(startPos != std::string::npos && endPos != std::string::npos)
		str = str.substr(startPos, endPos - startPos + 1);
	else
		str.clear();
	return str;
}
RequestResult Tools::Recv(int socketId, char* requestBuffer, ssize_t& bytesRead)
{
    memset(requestBuffer, 0, MessageBuffer);
    bytesRead = recv(socketId, requestBuffer, MessageBuffer, 0);
    if(bytesRead == 0)
        return ClosedConnection;
    else if(bytesRead < 0)
        return Success;
    return Success;
}
std::string Tools::ToString(int value)
{
    std::ostringstream oss;
    oss << value;
    return oss.str();
}
bool Tools::IsAllDigits(const std::string& str)
{
    for(size_t i = 0; i < str.length(); ++i) 
        if(!isdigit(str[i]))
            return false;
    return true;
}
void Tools::CheckSemicolon(std::string& configPart)
{
	size_t pos = configPart.rfind(';');
	if(pos != configPart.size() - 1)
		throw ServerDataExc(ESyntaxSem);
	configPart.erase(pos);
}
unsigned long Tools::StrToULong(const std::string& str)
{
    std::stringstream ss(str);
    unsigned long result;
    ss >> result;
    return result;
}
void Tools::Stat(const std::string& path, int mode, std::string messege)
{
    struct stat Stat;
    if(stat(path.c_str(), &Stat) == -1)
        throw ServerDataExc(messege);
    
    if(!(Stat.st_mode & mode))
		throw ServerDataExc(messege);
}
unsigned long long Tools::CheckClientMaxBodySize(std::string ClientSize)
{
    long long client_max_body_size = 0;
    if(!ClientSize.size())
        throw ServerDataExc(EClientSize);

    char lastChar = tolower(ClientSize[ClientSize.size() - 1]);
    if(lastChar == 'k' || lastChar == 'm' || lastChar == 'g')
        ClientSize = ClientSize.substr(0, ClientSize.size() - 1);
    
    for(size_t i = 0; i < ClientSize.size(); i++)
        if(!isdigit(ClientSize[i]))
            throw ServerDataExc(EClientSize);

    client_max_body_size = strtoull(ClientSize.c_str(), NULL, 10);
    if(lastChar == 'k')
        client_max_body_size *= 1024;
    else if(lastChar == 'm')
        client_max_body_size *= 1024 * 1024;
    else if(lastChar == 'g')
        client_max_body_size *= 1024 * 1024 * 1024;
    
    return client_max_body_size < 0 ? throw ServerDataExc(EClientSize) : client_max_body_size;
}
int Tools::CheckAutoIndex(std::string autoIndex)
{
    if(autoIndex == "on")
        return 1;
    else if(autoIndex == "off")
        return 0;
    else
        throw ServerDataExc(EAutoIndex);
}
std::vector<std::string> Tools::CheckIndices(std::string indices)
{
    if(indices.empty())
        throw ServerDataExc(EIndex);
    std::vector<std::string> files;
    std::istringstream iss(indices);
    std::string fileName;

    while(iss >> fileName)
        files.push_back(fileName);

    return files;
}
void Tools::AccessStat(int mode, int accMode, std::string error, std::string value)
{
	Tools::Stat(value, mode, error);
	if(access(value.c_str(), accMode) != 0)
        throw ServerDataExc(error);
}
std::map<int, std::string> Tools::InitReturn(std::string ret)
{
    std::map<int, std::string> res;
    std::string key, value;
    int k = -1;

	key = value = "";
	size_t colonPos = ret.find(' ');
	if(colonPos != std::string::npos)
	{
		key = ret.substr(0, colonPos);
		value = Tools::Trim((ret = ret.substr(colonPos)), WhiteSpaces);
        if(value.find_first_not_of('"') == std::string::npos || 
            value.find_last_not_of('"') == std::string::npos
            || !Tools::IsAllDigits(key))
            throw ServerDataExc(EReturn);

        k = atoi(key.c_str());
        if(k < 100 || k > 599)
            throw ServerDataExc(EReturn);
        res[k] = value.substr(1, value.size() - 2);
	}
	else
    {
        if(!Tools::IsAllDigits(ret))
            throw ServerDataExc(EReturn);
        k = atoi(ret.c_str());
        if(k < 100 || k > 599)
            throw ServerDataExc(EReturn);
        res[k] = value;
    }

    return res;
}
void Tools::CheckRootOrLocation(std::string& dir, std::string ex, std::string error)
{
    if(ex.substr(0, 2) == "./")
		dir = ex;
	else if(ex[0] == '/')
		dir = "." + ex;
	else
		dir = "./" + ex;
	Tools::AccessStat(S_IFDIR, W_OK, error, dir);
}

ThereIs Tools::PathExists(const std::string path)
{
    struct stat info;
    if(stat(path.c_str(), &info) != 0) 
        return NotFound;
    else if(info.st_mode & S_IFDIR)
        return Directory;
    else if(info.st_mode & S_IFREG)
        return File;
    else
        return Error;
}

std::string Tools::GetCurrentDateTime(void)
{
    time_t rawtime;
    time(&rawtime);
    struct tm * timeinfo;
    timeinfo = gmtime(&rawtime);
    char buffer[30];
    strftime(buffer, sizeof(buffer), "%a, %d %b %Y %H:%M:%S GMT", timeinfo);
    return std::string(buffer);
}

bool Tools::IsHidden(const std::string& filename) { return !filename.empty() && filename[0] == '.'; }

bool Tools::EndsWith(const std::string& str, const std::string& suffix)
{
    if(str.length() >= suffix.length())
        return (0 == str.compare(str.length() - suffix.length(), suffix.length(), suffix));
    return false;
}

std::string Tools::GenerateHtmlFromDirectory(const std::string& dirPath, int socketId)
{
    DIR* dir;
    struct dirent* entry;
    std::ostringstream oss;

    dir = opendir(dirPath.c_str());
    if(dir == NULL)
    {
        Logger::LogMsg(WARNING, "Error opening directory", socketId);
        return "";
    }

    oss << "<!DOCTYPE html>\n";
    oss << "<html>\n";
    oss << "<head>\n";
    oss << "<title>Miracle</title>\n";
    oss << "<style>\n";
    oss << "html { color-scheme: light dark; }\n";
    oss << "body { width: 35em; margin: 0 auto;\n";
    oss << "font-family: Tahoma, Verdana, Arial, sans-serif; }\n";
    oss << "</style>\n";
    oss << "<link rel=\"icon\" href=\"polygon.jpeg\" type=\"image/jpeg\">\n";
    oss << "</head>\n";
    oss << "<body>\n";
    oss << "<h1>Welcome to Miracle!(autoindex)</h1>\n";
    oss << "<ul>\n";

    while((entry = readdir(dir)) != NULL)
    {
        std::string name = entry->d_name;
        if(!Tools::IsHidden(name))
            oss << "<li><a href=\"" << name << "\">" << name << "</a></li>\n";
    }

    oss << "</ul>\n";
    oss << "<p><em>Thank you for using Miracle.</em></p>\n";
    oss << "</body>\n";
    oss << "</html>\n";

    closedir(dir);
    return oss.str();
}