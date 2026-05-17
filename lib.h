#pragma once

#include <iostream>
#include <stdint.h>
#include <vector>
#include <string_view>
#include <algorithm>

int version();

//---------------------------------------------------------------------------
std::vector<uint8_t> splitIpToNum(std::string_view s, const char delim = '.');

//---------------------------------------------------------------------------
std::vector<std::vector<uint8_t>> loadIpAddresses(std::istream& input);

//---------------------------------------------------------------------------
template <typename Func>
void outIpByCondition(std::ostream& out, const std::vector<std::vector<uint8_t>>& vec, Func condition)
{
    for (const auto& ip : vec)
    {
        if(ip.size() >= 4 && condition(ip))
        {
            out << static_cast<int>(ip[0]) << "."
                << static_cast<int>(ip[1]) << "."
                << static_cast<int>(ip[2]) << "."
                << static_cast<int>(ip[3]) << "\n";
        }
    }
}

//---------------------------------------------------------------------------
void processIpFilter(std::istream& input, std::ostream& output);
