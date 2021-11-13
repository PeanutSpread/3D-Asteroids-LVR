#include "common.h"

#include <fstream>
#include <sstream>

std::string ReadTextFile(const std::string& fname)
{
    std::ifstream f(fname.c_str());

    if (!f.good()) {
        std::stringstream errmsg;
        errmsg << "Failed to open " << fname << " for reading";
        throw Exception(errmsg.str());
    }

    std::stringstream ss;
    std::string line;
    while (getline(f, line))
        ss << line << '\n';

    if (!f.eof()) {
        std::stringstream errmsg;
        errmsg << "Error reading " << fname;
        throw Exception(errmsg.str());
    }

    return ss.str();
}


std::vector<std::string> Tokenize(const std::string& str)
{
    std::stringstream ss(str);
    std::vector<std::string> tokens;

    for (;;) {
        std::string s;
        if (!(ss >> s))
            break;
        tokens.push_back(s);
    }

    return tokens;
}
