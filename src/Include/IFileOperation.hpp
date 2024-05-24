#pragma once

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