#ifndef CONFIG_FILE_HPP
#define CONFIG_FILE_HPP
#include <string>
#include <unistd.h>
#include <sys/stat.h>
#include <fstream>
#include <sstream>
#include "ServerDataExc.hpp"

class ConfigFile
{
    public :
        ConfigFile(const char* path);
        std::string ProcessConfigFile(void);

    private :
        std::string _path;

        void isConfigFile(void);
        void checkFileAccess(void);
        std::string readFileToString(void);
};

#endif