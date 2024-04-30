#ifndef FILEOPERATION_HPP
# define FILEOPERATION_HPP

# include <iostream>
# include <fstream>
# include <string>
# include <stdexcept>


class FileOperation {
	public:
		static std::string	readFile(const std::string&);
		static void			writeFile(const std::string&, const std::string&);
	
	private:
		FileOperation(void);
		FileOperation(const FileOperation&);
		~FileOperation(void);

		FileOperation& operator=(const FileOperation&);
};

#endif