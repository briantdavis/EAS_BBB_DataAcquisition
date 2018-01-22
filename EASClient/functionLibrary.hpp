#ifndef FUNCTIONLIBRARY_HPP
#define FUNCTIONLIBRARY_HPP

#include <vector>
#include <string>
#include <algorithm>

std::vector<std::string> split(std::string, char);
int numEmptyStrings(std::vector<std::string>);
void parselatlong(std::string, float&);
void parsetime(std::string, unsigned int&, unsigned int&, float&);

#endif //FUNCTIONLIBRARY_HPP