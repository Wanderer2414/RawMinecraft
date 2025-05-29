#ifndef FILE_EXTENDED_H
#define FILE_EXTENDED_H
#include "Global.h"

void connect(std::fstream& stream, const std::string& file);
void close(std::fstream& stream, const std::string& file);

#endif