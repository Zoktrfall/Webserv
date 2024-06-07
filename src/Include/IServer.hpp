#ifndef I_SERVER_HPP
#define I_SERVER_HPP

struct location
{
	public:
		bool							autoIndex;
		std::string 					uploadDir;
		std::string						root;
		std::vector<std::string>		allowMethods;
		std::vector<std::string>		index;
		std::pair<short, std::string>	redirect;
	
		location& operator=(const location&);
};

class IServer
{
	public:
		virtual ~IServer() {}

		virtual bool								isDefaultServer(void)		const = 0;
		virtual const std::string&					getServerName(void)			const = 0;
		virtual const std::string&					getClientMaxBodySize(void)	const = 0;
		virtual const std::string&					getRoot(void)				const = 0;
		virtual const std::map<short, std::string>&	getErrorPage(void)			const = 0;
		virtual unsigned short int					getPort(void)				const = 0;
		virtual const std::vector<location>&		getLocations(void)			const = 0;
		virtual const std::vector<std::string>&		getCgi(void)				const = 0;

		virtual void	setServerName(const std::string&) = 0;
		virtual void	setClientMaxBodySize(const std::string&) = 0;
		virtual void	setRoot(const std::string&) = 0;
		virtual void	setErrorPage(const std::map<short, std::string>&) = 0;
		virtual void	setPort(unsigned short int) = 0;
		virtual void	setLocations(const std::vector<location>&) = 0;
		virtual void	setCgi(const std::vector<std::string>&) = 0;
	
};

#endif