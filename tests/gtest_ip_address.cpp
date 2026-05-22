#include <ip_filter/ip_address.h>
#include <gtest/gtest.h>

//---------------------------------------------------------------------------
// Тесты для IpAddress::fromString
//---------------------------------------------------------------------------
TEST(IpAddressParseTest, ConstructorFromFourOctets) {
    // Проверяем, что конструктор от 4 чисел собирает правильное 32-битное число
    IpAddress ip(192, 168, 1, 100);
    EXPECT_EQ(ip.asUInt32(), 0xC0A80164);

    IpAddress ip_zero(0, 0, 0, 0);
    EXPECT_EQ(ip_zero.asUInt32(), 0x00000000);

    IpAddress ip_max(255, 255, 255, 255);
    EXPECT_EQ(ip_max.asUInt32(), 0xFFFFFFFF);
}

TEST(IpAddressParseTest, ValidIpAddresses) {
    IpAddress ip;

    // 127.0.0.1 -> 0x7F000001
    ASSERT_TRUE(IpAddress::fromString("127.0.0.1", ip));
    EXPECT_EQ(ip.asUInt32(), 0x7F000001);

    // 0.0.0.0 -> 0x00000000
    ASSERT_TRUE(IpAddress::fromString("0.0.0.0", ip));
    EXPECT_EQ(ip.asUInt32(), 0x00000000);

    // 255.255.255.255 -> 0xFFFFFFFF
    ASSERT_TRUE(IpAddress::fromString("255.255.255.255", ip));
    EXPECT_EQ(ip.asUInt32(), 0xFFFFFFFF);

    // 192.168.1.100 -> 0xC0A80164
    ASSERT_TRUE(IpAddress::fromString("192.168.1.100", ip));
    EXPECT_EQ(ip.asUInt32(), 0xC0A80164);
}

//---------------------------------------------------------------------------
TEST(IpAddressParseTest, InvalidOctetCount) {
    IpAddress ip;
    EXPECT_FALSE(IpAddress::fromString("192.168.1", ip));
    EXPECT_FALSE(IpAddress::fromString("192.168", ip));
    EXPECT_FALSE(IpAddress::fromString("192", ip));
    EXPECT_FALSE(IpAddress::fromString("192.168.1.1.5", ip));
    EXPECT_FALSE(IpAddress::fromString("1.2.3.4.5.6.7.8", ip));
}

//---------------------------------------------------------------------------
TEST(IpAddressParseTest, OutOfByteRange) {
    IpAddress ip;
    EXPECT_FALSE(IpAddress::fromString("256.100.100.100", ip));
    EXPECT_FALSE(IpAddress::fromString("192.168.1.300", ip));
    EXPECT_FALSE(IpAddress::fromString("-1.100.100.100", ip));
    EXPECT_FALSE(IpAddress::fromString("192.168.-1.1", ip));
}

//---------------------------------------------------------------------------
TEST(IpAddressParseTest, InvalidCharacters) {
    IpAddress ip;
    EXPECT_FALSE(IpAddress::fromString("192.16a.1.1", ip));
    EXPECT_FALSE(IpAddress::fromString("abc.def.ghi.jkl", ip));
    EXPECT_FALSE(IpAddress::fromString("192 .168.1.1", ip));
    EXPECT_FALSE(IpAddress::fromString("192.168.1. 1", ip));

    // Тест на защиту от переполнения (20 единиц)
    EXPECT_FALSE(IpAddress::fromString("11111111111111111111", ip));
}

//---------------------------------------------------------------------------
TEST(IpAddressParseTest, DelimiterAndEdgeCases) {
    IpAddress ip;
    EXPECT_FALSE(IpAddress::fromString("", ip));
    EXPECT_FALSE(IpAddress::fromString("192..1.1", ip));
    EXPECT_FALSE(IpAddress::fromString("192.168.1..", ip));
    EXPECT_FALSE(IpAddress::fromString(".192.168.1.1", ip));
    EXPECT_FALSE(IpAddress::fromString("192.168.1.1.", ip));
    EXPECT_FALSE(IpAddress::fromString("...", ip));
}

//---------------------------------------------------------------------------
// Тесты методов фильтрации класса IpAddress
//---------------------------------------------------------------------------
TEST(IpAddressFilterMethodsTest, ContainsOctet) {
    IpAddress ip;
    ASSERT_TRUE(IpAddress::fromString("46.70.1.2", ip));

    EXPECT_TRUE(ip.contains(46));
    EXPECT_TRUE(ip.contains(70));
    EXPECT_TRUE(ip.contains(1));
    EXPECT_TRUE(ip.contains(2));
    EXPECT_FALSE(ip.contains(255));
}
