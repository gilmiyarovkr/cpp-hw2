#pragma once

#include <stdint.h>
#include <vector>
#include <string>

int version();

std::vector<uint8_t> splitIpToNum(const std::string &s, char delim);

template <typename Func>
void coutByCondition(const std::vector<std::vector<uint8_t>>& vec, Func condition);
