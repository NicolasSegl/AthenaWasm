#pragma once

#include <string>
#include <vector>

void splitString(const std::string& string, std::vector<std::string>& vec, char toSplitCharacter);
int clampi(int lower, int higher, int value);