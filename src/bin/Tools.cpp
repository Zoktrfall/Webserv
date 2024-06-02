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

RequestResult Tools::Recv(int socketId, char* requestBuffer, size_t bufferSize)
{
    memset(requestBuffer, 0, bufferSize);
    int bytesRead = recv(socketId, requestBuffer, bufferSize, 0);
    
    if (bytesRead == 0)
        return ClosedConnection;
    else if (bytesRead < 0)
        return ReadError;
    return Success;
}