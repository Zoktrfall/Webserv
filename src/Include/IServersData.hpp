#ifndef I_SERVERS_DATA_HPP
#define I_SERVERS_DATA_HPP
#include "Server.hpp"

class IServersData
{
    public :
        virtual ~IServersData() {};
        virtual bool SetupServersData(void) = 0;
        virtual void LoadServers(void) = 0;
		virtual void CreateServer(std::string, Server&) = 0;
        virtual void CheckServers(Server&) = 0;
		virtual void HasConflicts(void) = 0;
};

#endif