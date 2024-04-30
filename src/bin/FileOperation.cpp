#include "FileOperation.hpp"

FileOperation::FileOperation(void) {
	return ;
}

FileOperation::FileOperation(const FileOperation& rhs) {
	(void) rhs;
	return ;
}

FileOperation::~FileOperation(void) {
	return ;
}

FileOperation& FileOperation::operator=(const FileOperation& rhs) {
	(void) rhs;
	return (*this);
}

std::string	FileOperation::readFile(const std::string& fileName) {
	std::ifstream	file(fileName.c_str());
	if (!file.is_open())
		throw std::runtime_error("Unable to open file: " + fileName);

	std::string	content;
	std::string	line;
	while(std::getline(file, line))
		content += line + "\n";
	
	file.close();
	return (content);
}

void	FileOperation::writeFile(const std::string& fileName, const std::string& content) {
	std::ofstream	file(fileName.c_str());
	if (!file.is_open())
		throw std::runtime_error("Unable to open file: " + fileName);

	file << content;
	file.close();
	return ;
}