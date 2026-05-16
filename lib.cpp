#include "lib.h"

#include <limits>
#include <sstream>
#include <algorithm>
#include <math.h>

#include "version.h"

int version() {
	return PROJECT_VERSION_PATCH;
}

//---------------------------------------------------------------------------
std::vector<uint8_t> splitIpToNum(const std::string &s, char delim)
{
    std::stringstream ss(s);
    std::string item;
    std::vector<uint8_t> elems;
    while (std::getline(ss, item, delim))
    {
        if(item != "")
        {
            elems.push_back(static_cast<uint8_t>(std::stoi(item)));
        }
    }
    return elems;
}

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