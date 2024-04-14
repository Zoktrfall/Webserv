#pragma once

# include "IFileOperation.hpp"

class FileOperation : public IFileOperation {
	public:
		FileOperation(void);
		FileOperation(const FileOperation&);
		~FileOperation(void);

		FileOperation& operator=(const FileOperation&);

		std::string	readFile(const std::string&)						const;
		void		writeFile(const std::string&, const std::string&)	const;
};