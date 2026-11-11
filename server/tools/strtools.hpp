#include <string>
#include <vector>
#include <iostream>
#include <map>

// USEFUL functions to work with std::string
// хз насколько useful ну раз уж ты так говоришь то базара ноль
// делаем 
std::string toupper(const std::string&);
std::string tolower(const std::string&);
int string_to_int(const std::string&);
std::vector<std::string> split(const std::string&, char);
std::vector<std::string> garbage_split(const std::string&, char);
std::vector<std::string> divide_in_two(const std::string& str, int start, char sep);
bool starts_with(const std::string& dep, const std::string& arr);
std::map<std::string, std::string> to_map(const std::string&);
std::string map_to_string(const std::map<std::string, std::string>& inp);

// unsafe hzhz
// этих тварей по хорошему убрать отсюеда к чертвоой матери ну ладно
// эти функции под спасом придумывали
std::string hash_password(const std::string& password);
std::string hash_token(const std::string& token);
