#ifndef I_FILE_OPERATION_HPP
#define I_FILE_OPERATION_HPP
# include <iostream>
# include <fstream>
# include <string>
# include <stdexcept>

class	IFileOperation {
	public:
		virtual std::string	readFile(const std::string&) const = 0;
		virtual void		writeFile(const std::string&, const std::string&) const = 0;
		virtual				~IFileOperation() {};
};

#endif