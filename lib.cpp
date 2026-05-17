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

//---------------------------------------------------------------------------
std::vector<std::vector<uint8_t>> loadIpAddresses(std::istream& input)
{
    std::string line;
    std::vector<std::vector<uint8_t>> ip;

    while (std::getline(input, line))
    {
        if (line.empty())
        {
            continue;
        }

        // Ищем разделитель табуляции
        size_t tab_pos = line.find('\t');
        std::string_view ip_str = (tab_pos != std::string::npos)
                                      ? std::string_view(line).substr(0, tab_pos)
                                      : std::string_view(line);

        auto res = splitIpToNum(ip_str, '.');
        if (res.size() == 4)
        {
            ip.push_back(std::move(res));
        }
    }

    return ip;
}

//---------------------------------------------------------------------------
void processIpFilter(std::istream& input, std::ostream& output)
{
    auto ip = loadIpAddresses(input);

    std::sort(ip.begin(), ip.end(), std::greater<std::vector<uint8_t>>());

    for(const auto& i : ip)
    {
        output << int(i[0]) << "." << int(i[1]) << "." << int(i[2]) << "." << int(i[3]) << "\n";
    }

    outIpByCondition(output, ip, [](const auto& el){
        return (el[0] == 1);
    });

    outIpByCondition(output, ip, [](const auto& el){
        return (el[0] == 46) && (el[1] == 70);
    });

    outIpByCondition(output, ip, [](const auto& lip){
        return lip.end() != std::find(lip.begin(), lip.end(), 46);
    });
}
