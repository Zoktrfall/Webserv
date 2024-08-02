#include "Location.hpp"

Location::Location(void) :
    _path(""),
    _root(""),
    _autoindex(-1),
    _client_max_body_size(-1),
    _indices(),
    _return(),
    _limit_except(),
    _alias(""),
    _cgi_path(),
    _cgi_ext() {
}

const std::string& Location::GetPath(void) const { return _path; }
const std::string& Location::GetRoot(void) const { return _root; }
int Location::GetAutoindex(void) const { return _autoindex; }
const std::vector<std::string>& Location::GetIndices() const { return _indices; }
std::string Location::GetIndex(size_t i) const { return _indices[i]; }
std::string Location::GetAlias() const { return _alias; }
long long Location::GetClientMaxBodySize(void) const { return _client_max_body_size; }
std::vector<std::string> Location::GetCgiPath(void) const { return _cgi_path; }
std::vector<std::string> Location::GetCgiExt(void) const { return _cgi_ext; }
std::map<int, std::string> Location::GetReturn(void) const { return _return; }
std::vector<HttpMethod> Location::GetLimitExcept(void) const { return _limit_except; }

void Location::SetPath(std::string& path) { _path = path; }
void Location::SetRoot(std::string& root) { Tools::CheckRootOrLocation(_root, root, ERootSyntax); }
void Location::SetClientMaxBodySize(std::string& client_max_body_size) { _client_max_body_size = Tools::CheckClientMaxBodySize(client_max_body_size); }
void Location::SetAutoindex(std::string& autoIndex) { _autoindex = Tools::CheckAutoIndex(autoIndex); }
void Location::SetIndices(std::string& indices) { _indices = Tools::CheckIndices(indices); }
void Location::SetCgiPath(std::string& cgiPath)
{
    if(cgiPath.empty())
        throw ServerDataExc(ECGIPath);
    std::istringstream iss(cgiPath);
    std::string path;

    while(iss >> path)
	{
        Tools::AccessStat(S_IXUSR, X_OK, ECGIPath, path);
        _cgi_path.push_back(path);
    }
}
void Location::SetCgiExt(std::string& cgiExt)
{
    if(cgiExt.empty())
        throw ServerDataExc(ECGIExt);

    const std::string invalid_chars = "!@#$%^&*()=+[]{}|\\:;'\"<>?/, ";
    std::istringstream iss(cgiExt);
    std::string ext;
    while(iss >> ext)
    {
        if(ext.find_first_of(invalid_chars) != std::string::npos || ext.size() == 1
            || ext[0] != '.')
            throw ServerDataExc(ECGIExt);
        _cgi_ext.push_back(ext);
    }
}
void Location::SetReturn(std::string& ret) { _return = Tools::InitReturn(ret); }
void Location::SetAlias(std::string& alias) { _alias = alias; }
void Location::SetLimitExcept(std::string& limits) 
{
    if(limits.empty())
        throw ServerDataExc(ELimit);
    std::istringstream iss(limits);
    std::string method;

    while(iss >> method)
    {
        if(method == "GET")
            _limit_except.push_back(GET);
        else if(method == "POST")
            _limit_except.push_back(POST);
        else if(method == "DELETE")
            _limit_except.push_back(DELETE);
        else if(method == "HEAD")
            _limit_except.push_back(HEAD);
        else
            throw ServerDataExc(ELimit);
    }
}

