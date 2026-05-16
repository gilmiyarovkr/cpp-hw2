#include "lib.h"
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
