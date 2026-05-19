#include "ip_filter.h"

#include <gtest/gtest.h>
#include <sstream>

//---------------------------------------------------------------------------
// Тесты для IpFilter::load
//---------------------------------------------------------------------------
TEST(IpFilterTest, LoadIpAddressesFromStream) {
    std::string mock_input =
        "192.168.1.1\t626437\n"
        "\n"
        "10.0.0.1\n"
        "invalid_ip\t12314\n"
        "127.0.0.1\t\n";

    std::istringstream stream(mock_input);

    IpFilter filter;
    filter.load(stream);

    std::ostringstream output;
    filter.printAll(output);

    // Проверяем, что загрузились только 3 валидных IP в исходном порядке
    std::string expected_output = "192.168.1.1\n10.0.0.1\n127.0.0.1\n";
    EXPECT_EQ(output.str(), expected_output);
}

//---------------------------------------------------------------------------
// Тесты сортировки класса IpFilter
//---------------------------------------------------------------------------
TEST(IpFilterTest, SortingOrderDescending) {
    std::string mock_input =
        "10.0.0.1\n"
        "192.168.1.2\n"
        "192.168.1.10\n"
        "192.168.0.5\n"
        "1.1.1.1\n";

    std::istringstream stream(mock_input);

    IpFilter filter;
    filter.load(stream);
    filter.sortDescending();

    std::ostringstream output;
    filter.printAll(output);

    std::string expected_output =
        "192.168.1.10\n"
        "192.168.1.2\n"
        "192.168.0.5\n"
        "10.0.0.1\n"
        "1.1.1.1\n";

    EXPECT_EQ(output.str(), expected_output);
}

//---------------------------------------------------------------------------
// Тесты фильтрации через шаблоны и бинарный поиск
//---------------------------------------------------------------------------
TEST(IpFilterTest, FilterAndRangeMethods) {
    std::string mock_input =
        "1.2.3.4\n"
        "192.168.1.1\n"
        "1.255.0.0\n"
        "46.70.1.2\n"
        "46.10.46.2\n"
        "1.1.1.1\n";

    std::istringstream stream(mock_input);
    IpFilter filter;
    filter.load(stream);
    filter.sortDescending(); // Важно для printRange!

    // 1. Тест эффективного поиска диапазона (Начинается на 1)
    {
        std::ostringstream output;
        filter.printRange(output, IpAddress(1,0,0,0), IpAddress(1,255,255,255));
        EXPECT_EQ(output.str(), "1.255.0.0\n1.2.3.4\n1.1.1.1\n");
    }

    // 2. Тест эффективного поиска диапазона (Начинается на 46.70)
    {
        std::ostringstream output;
        filter.printRange(output, IpAddress(46,70,0,0), IpAddress(46,70,255,255));
        EXPECT_EQ(output.str(), "46.70.1.2\n");
    }

    // 3. Тест универсального шаблона (Любой октет содержит 46)
    {
        std::ostringstream output;
        filter.printIf(output, [](const IpAddress& ip) {
            return ip.contains(46);
        });
        EXPECT_EQ(output.str(), "46.70.1.2\n46.10.46.2\n");
    }
}
