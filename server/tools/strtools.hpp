#include <string>
#include <vector>
#include <iostream>
#include <map>

// USEFUL functions to work with std::string
std::string toupper(const std::string&);
size_t string_to_size_t(const std::string&);
std::vector<std::string> split(const std::string&, char);
std::vector<std::string> garbage_split(const std::string&, char);
std::vector<std::string> devide_in_two(const std::string& str, int start, char sep);
bool starts_with(const std::string& dep, const std::string& arr);
std::map<std::string, std::string> to_map(const std::string&);
