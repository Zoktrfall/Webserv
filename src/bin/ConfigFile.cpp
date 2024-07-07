#include "ConfigFile.hpp"

ConfigFile::ConfigFile(const char* path) : _path(path == NULL ? "./configs/default.conf" : path) {}
void ConfigFile::CheckFileAccess(void) { if(access(_path.c_str(), R_OK) != 0) throw(ServerDataExc(EAccess)); }
std::string ConfigFile::ReadFileToString(void)
{
	std::ifstream File(_path.c_str());
	if(!File || !File.is_open())
		throw(ServerDataExc(EOpenFile));

	std::stringstream streamBind;
	streamBind << File.rdbuf();
	return streamBind.str();
}
std::string ConfigFile::ProcessConfigFile(void)
{
    Tools::Stat(_path, S_IFREG, ENotFile);
    ConfigFile::CheckFileAccess();
    return ConfigFile::ReadFileToString();
}