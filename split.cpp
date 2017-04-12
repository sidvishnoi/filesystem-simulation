#include "filesystem.h"

std::vector<std::string> split(const std::string &s, const char delim) {
    /*
    castective: to split a string by given delimitter
    input:
        s: given string
        delim: delimitter
    return: vector of strings split at given delimitter
    */
    std::stringstream ss(s);
    std::string item;
    std::vector<std::string> tokens;
    while (getline(ss, item, delim)) {
        tokens.push_back(item);
    }
    return tokens;
}
