#include <ip_filter/ip_address.h>

bool IpAddress::fromString(std::string_view ip, IpAddress& out_ip, char delimiter)
{
    // Быстрая проверка границ: IP не может быть пустым, начинаться или заканчиваться на точку
    if (ip.empty() || ip.front() == delimiter || ip.back() == delimiter)
    {
        return false;
    }

    uint32_t res = 0;
    uint32_t octet = 0;
    size_t dot_count = 0;
    size_t digit_count = 0;

    for (const char ch : ip)
    {
        if (ch == delimiter)
        {
            if (digit_count == 0 || octet > 255)
            {
                return false;
            }

            res = (res << 8) | octet;
            octet = 0;
            digit_count = 0;
            dot_count++;

            if (dot_count > 3)
            {
                return false;
            }
        }
        else if (ch >= '0' && ch <= '9')
        {
            octet = octet * 10 + (static_cast<uint32_t>(ch)- static_cast<uint32_t>('0'));
            digit_count++;

            if (digit_count > 3)
            {
                return false;
            }
        }
        else
        {
            return false;
        }
    }

    // Валидация последнего октета и структуры (точек должно быть == 3)
    if (dot_count != 3 || digit_count == 0 || octet > 255)
    {
        return false;
    }

    res = (res << 8) | octet;

    out_ip = IpAddress(res);
    return true;
}

uint8_t IpAddress::getOctet(size_t index) const
{
    if (index > 3)
        return 0;
    return static_cast<uint8_t>((_word >> (8 * (3 - index))) & 0xFF);
}

bool IpAddress::contains(uint8_t octet) const
{
    return getOctet(0) == octet || getOctet(1) == octet ||
           getOctet(2) == octet || getOctet(3) == octet;
}

std::ostream& operator<<(std::ostream& output, const IpAddress& ip)
{
    output << static_cast<int>(ip.getOctet(0)) << '.'
           << static_cast<int>(ip.getOctet(1)) << '.'
           << static_cast<int>(ip.getOctet(2)) << '.'
           << static_cast<int>(ip.getOctet(3));
    return output;
}
