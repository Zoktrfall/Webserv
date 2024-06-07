#ifndef FILE_OPERATION_HPP
#define FILE_OPERATION_HPP
# include "IFileOperation.hpp"

class FileOperation : public IFileOperation {
	public:
		FileOperation(void);
		FileOperation(const FileOperation&);
		~FileOperation();

		FileOperation& operator=(const FileOperation&);

		std::string	readFile(const std::string&)						const;
		void		writeFile(const std::string&, const std::string&)	const;
};

#endif