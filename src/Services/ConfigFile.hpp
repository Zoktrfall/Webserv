#ifndef CONFIG_FILE_HPP
#define CONFIG_FILE_HPP
#include <string>
#include <unistd.h>
#include <fstream>
#include <sstream>
#include "Tools.hpp"
#include "ServerDataExc.hpp"

class ConfigFile
{
    public :
        ConfigFile(const char* path);
        std::string ProcessConfigFile(void);

    private :
        std::string _path;

        void CheckFileAccess(void);
        std::string ReadFileToString(void);
};

#endif