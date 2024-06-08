#include "ConfigFile.hpp"

ConfigFile::ConfigFile(const char* path) : _path(path == NULL ? "./configs/default.conf" : path) {}
void ConfigFile::checkFileAccess(void) { if (access(_path.c_str(), R_OK) != 0) throw(ServerDataExc(EAcces)); }
void ConfigFile::isConfigFile(void)
{
    struct stat fileStat;
    if (stat(_path.c_str(), &fileStat) == -1)
        throw ServerDataExc(EStats);
    
    if (!(fileStat.st_mode & S_IFREG))
		throw(ServerDataExc(NotFileMessage));
}
std::string ConfigFile::readFileToString(void)
{
	std::ifstream File(_path.c_str());
	if (!File || !File.is_open())
		throw(ServerDataExc(EOpenFile));

	std::stringstream streamBind;
	streamBind << File.rdbuf();
	return (streamBind.str());
}
std::string ConfigFile::ProcessConfigFile(void)
{
    ConfigFile::isConfigFile();
    ConfigFile::checkFileAccess();
    return ConfigFile::readFileToString();
}