#include "Tools.hpp"

std::string Tools::ToLower(const std::string& str)
{
	std::string lowercasedStr = "";
	size_t idx = 0;

	while (idx < str.size())
		lowercasedStr += std::tolower(str[idx++]);

	return lowercasedStr;
}

std::string& Tools::Trim(std::string& str, const std::string& trimmerStr)
{
	size_t startPos = str.find_first_not_of(trimmerStr);
	size_t endPos = str.find_last_not_of(trimmerStr);

	if (startPos != std::string::npos && endPos != std::string::npos)
		str = str.substr(startPos, endPos - startPos + 1);
	else
		str.clear();
	return str;
}

std::string Tools::Recv(const int socketId, const int bufferSize)
{
	char requestBuffer[bufferSize];

	bzero(&requestBuffer, bufferSize * sizeof(char));
	if(recv(socketId, requestBuffer, sizeof(requestBuffer), 0) <= 0) 
        return "";
	
    return std::string(requestBuffer);
}