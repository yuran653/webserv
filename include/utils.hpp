#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <sstream>

bool hasWhiteSpaces(const std::string &str);
void removeTrailingWhiteSpaces(std::string &str);
void toLowerCase(std::string &str);
std::string intToString(int value);
std::string size_tToString(size_t value);


#endif