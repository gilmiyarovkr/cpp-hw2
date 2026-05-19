#pragma once

#include <string_view>
#include <cstdint>
#include <iostream>

class IpAddress
{
public:
    IpAddress() : _word(0) {}
    explicit IpAddress(uint32_t word) : _word(word) {}
    IpAddress(uint8_t o0, uint8_t o1, uint8_t o2, uint8_t o3) {
        _word = (static_cast<uint32_t>(o0) << 24) |
                (static_cast<uint32_t>(o1) << 16) |
                (static_cast<uint32_t>(o2) << 8)  |
                static_cast<uint32_t>(o3);
    }

    static bool fromString(std::string_view s, IpAddress& out_ip, char delim = '.');

    // Геттеры
    uint32_t asUInt32() const { return _word; }
    uint8_t getOctet(size_t index) const; // index: 0 (старший) до 3 (младший)

    // Проверки для фильтрации
    bool contains(uint8_t octet) const;

    // Операторы сравнения для сортировки
    bool operator>(const IpAddress& other) const { return _word > other._word; }
    bool operator<(const IpAddress& other) const { return _word < other._word; }
    bool operator==(const IpAddress& other) const { return _word == other._word; }

    // Вывод в поток
    friend std::ostream& operator<<(std::ostream& output, const IpAddress& ip);

private:
    uint32_t _word = 0;
};
