#include "ServerDataExc.hpp"

ServerDataExc::ServerDataExc(const std::string& errorMessage) : _errorMessage(errorMessage) {}
inline const char* ServerDataExc::what() const throw() { return _errorMessage.c_str(); }