#include "lib.h"
#include <charconv>
#include "version.h"

//---------------------------------------------------------------------------
int version() 
{
	return PROJECT_VERSION_PATCH;
}

//---------------------------------------------------------------------------
std::vector<uint8_t> splitIpToNum(std::string_view s, const char delim)
{
    std::vector<uint8_t> elems;
    size_t start = 0;

    elems.reserve(4);
    while (start < s.size())
    {
        size_t end = s.find(delim, start);
        std::string_view item = s.substr(start, end - start);

        if (item.empty() || elems.size() > 4)
        {
            return {};
        }

        int value = 0;
        std::from_chars_result rc = std::from_chars(item.data(), item.data() + item.size(), value);
        if (rc.ec == std::errc() && rc.ptr == (item.data() + item.size()) &&
            value >= 0 && value <= 255)
        {
            elems.push_back(static_cast<uint8_t>(value));
        }
        else
        {
            return {};
        }

        if (end == std::string_view::npos)
        {
            break;
        }
        start = end + 1;
    }

    // 192.168.1.1.
    if (s.empty() || s.back() == delim)
    {
        return {};
    }

    if (elems.size() != 4)
    {
        return {};
    }

    return elems;
}
