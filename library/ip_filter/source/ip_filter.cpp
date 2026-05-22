#include <ip_filter/ip_filter.h>
#include <algorithm>
#include <string>

void IpFilter::load(std::istream& input)
{
    std::string line;
    IpAddress ip;
    while (std::getline(input, line))
    {
        if (line.empty())
            continue;

        size_t tab_pos = line.find('\t');
        std::string_view ip_str = (tab_pos != std::string::npos)
                                      ? std::string_view(line).substr(0, tab_pos)
                                      : std::string_view(line);

        // Добавляем только валидные адреса
        if (IpAddress::fromString(ip_str, ip, '.'))
        {
            _addresses.push_back(ip);
        }
    }
}

void IpFilter::sortDescending()
{
    std::sort(_addresses.begin(), _addresses.end(), std::greater<IpAddress>());
}

void IpFilter::printAll(std::ostream& output) const
{
    for (const auto& ip : _addresses)
    {
        output << ip << "\n";
    }
}

void IpFilter::printRange(std::ostream& output, const IpAddress& min_border, const IpAddress& max_border) const
{
    // Находим первый элемент, который <= max_border (начало нашего поддиапазона)
    auto start_it = std::lower_bound(_addresses.begin(), _addresses.end(), max_border, std::greater<IpAddress>());

    // Находим первый элемент, который < min_border (конец нашего поддиапазона)
    auto end_it = std::upper_bound(start_it, _addresses.end(), min_border, std::greater<IpAddress>());

    // Выводим только этот поддиапазон
    for (auto it = start_it; it != end_it; ++it)
    {
        output << *it << "\n";
    }
}

