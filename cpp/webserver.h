// CR: When writing C++ code, the header file extension should be hpp.

#include <string>
// CR: pragma once should be the first thing in h file.
#pragma once
// CR: Move this function declaration to StringUtils.hpp
// CR: When declaring a function, also mention the names of the arguments, so other people can understand when are the arguments mean. Here I can't understand what is the string, why do we char an then int.
int replace_all(std::string, const char*, int, const char*);

// CR: Is this socket related? Move it to Socket class.
void send_message(int, const char path[]);
