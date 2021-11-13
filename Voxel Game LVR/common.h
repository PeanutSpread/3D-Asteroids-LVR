#ifndef COMMON_H_
#define COMMON_H_

#include "glshell.h"

#include <string>
#include <sstream>  // stringstream
#include <vector>

//
// Useful constants
//

const float PI = 3.14159265f;


//
// File handling stuff
//

std::string ReadTextFile(const std::string& fname);


//
// string handling stuff
//

std::vector<std::string> Tokenize(const std::string& str);

template <typename T>
std::string ToString(const T& value)
{
    std::ostringstream ss;
    ss << value;
    return ss.str();
}

template <typename T>
T FromString(const std::string& str)
{
    std::istringstream ss(str);
    T value;
    ss >> value;
    return value;
}

#endif
