#pragma once

#include "ip_address.h"
#include <vector>
#include <iostream>

class IpFilter
{
public:
    void load(std::istream& input);

    void sortDescending();

    void printAll(std::ostream& output) const;

    template <typename Predicate>
    void printIf(std::ostream& output, Predicate predicate) const
    {
        for (const auto& ip : _addresses)
        {
            if (predicate(ip))
            {
                output << ip << "\n";
            }
        }
    }

    void printRange(std::ostream& output, const IpAddress& min_border, const IpAddress& max_border) const;

private:
    std::vector<IpAddress> _addresses;
};
