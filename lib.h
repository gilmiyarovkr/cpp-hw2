#pragma once

#include <iostream>
#include <stdint.h>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

int version();

//---------------------------------------------------------------------------
std::vector<uint8_t> splitIpToNum(const std::string &s, char delim);

//---------------------------------------------------------------------------
template <typename Func>
void coutByCondition(const std::vector<std::vector<uint8_t>>& vec, Func condition)
{
    auto it = vec.begin();
    while ((it = std::find_if(it, vec.end(), condition)) != vec.end())
    {
        if(it->size() >= 4) // Несмотря на то что это проверяется в регулярке
        {
            std::cout << int(it->data()[0]) << "."
                      << int(it->data()[1]) << "."
                      << int(it->data()[2]) << "."
                      << int(it->data()[3]) << std::endl;
        }
        ++it;
    }
}
