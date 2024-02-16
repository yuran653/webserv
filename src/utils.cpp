#include "utils.hpp"

bool hasWhiteSpaces(const std::string &str)
{
	for (size_t i = 0; i < str.length(); i++)
		if (std::isspace(str[i]))
			return true;
	return false;
}

void removeTrailingWhiteSpaces(std::string &str)
{
	size_t pos = str.find_first_not_of(" \t");
	str.erase(0, pos);
	pos = str.find_last_not_of(" \t");
	str.erase(pos + 1);
}

void toLowerCase(std::string &str)
{
	for (size_t i = 0; i < str.length(); i++)
		str[i] = std::tolower(str[i]);
}

std::string intToString(int value)
{
	std::ostringstream oss;
	oss << value;
	return oss.str();
}

std::string size_tToString(size_t value)
{
	std::ostringstream oss;
	oss << value;
	return oss.str();
}