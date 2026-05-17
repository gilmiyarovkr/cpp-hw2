#include "lib.h"

#include <gtest/gtest.h>
#include <sstream>

//---------------------------------------------------------------------------
TEST(VersionTest, CheckPatchVersion) {
    // Пример проверки, замените на вашу реальную логику
    EXPECT_GT(version(), 0); 
}

//---------------------------------------------------------------------------
// Tests splitIpToNum
//---------------------------------------------------------------------------
TEST(SplitIpToNumTest, ValidIpAddresses) {
    EXPECT_EQ(splitIpToNum("127.0.0.1", '.'), (std::vector<uint8_t>{127, 0, 0, 1}));
    EXPECT_EQ(splitIpToNum("0.0.0.0", '.'), (std::vector<uint8_t>{0, 0, 0, 0}));
    EXPECT_EQ(splitIpToNum("255.255.255.255", '.'), (std::vector<uint8_t>{255, 255, 255, 255}));
    EXPECT_EQ(splitIpToNum("192.168.1.100", '.'), (std::vector<uint8_t>{192, 168, 1, 100}));
    EXPECT_EQ(splitIpToNum("10-0-0-1", '-'), (std::vector<uint8_t>{10, 0, 0, 1}));
}

//---------------------------------------------------------------------------
TEST(SplitIpToNumTest, InvalidOctetCount) {
    EXPECT_TRUE(splitIpToNum("192.168.1", '.').empty());
    EXPECT_TRUE(splitIpToNum("192.168", '.').empty());
    EXPECT_TRUE(splitIpToNum("192", '.').empty());

    EXPECT_TRUE(splitIpToNum("192.168.1.1.5", '.').empty());
    EXPECT_TRUE(splitIpToNum("1.2.3.4.5.6.7.8", '.').empty());
}

//---------------------------------------------------------------------------
TEST(SplitIpToNumTest, OutOfByteRange) {
    EXPECT_TRUE(splitIpToNum("256.100.100.100", '.').empty());
    EXPECT_TRUE(splitIpToNum("192.168.1.300", '.').empty());
    
    EXPECT_TRUE(splitIpToNum("-1.100.100.100", '.').empty());
    EXPECT_TRUE(splitIpToNum("192.168.-1.1", '.').empty());
}

//---------------------------------------------------------------------------
TEST(SplitIpToNumTest, InvalidCharacters) {
    EXPECT_TRUE(splitIpToNum("192.16a.1.1", '.').empty());
    EXPECT_TRUE(splitIpToNum("abc.def.ghi.jkl", '.').empty());

    EXPECT_TRUE(splitIpToNum("192 .168.1.1", '.').empty());
    EXPECT_TRUE(splitIpToNum("192.168.1. 1", '.').empty());
}

//---------------------------------------------------------------------------
TEST(SplitIpToNumTest, DelimiterAndEdgeCases) {
    EXPECT_TRUE(splitIpToNum("", '.').empty());

    EXPECT_TRUE(splitIpToNum("192..1.1", '.').empty());
    EXPECT_TRUE(splitIpToNum("192.168.1..", '.').empty());

    EXPECT_TRUE(splitIpToNum(".192.168.1.1", '.').empty());
    
    EXPECT_TRUE(splitIpToNum("192.168.1.1.", '.').empty());
    
    EXPECT_TRUE(splitIpToNum("...", '.').empty());
}

//---------------------------------------------------------------------------
// Tests loadIpAddresses
//---------------------------------------------------------------------------
TEST(IpPipelineTest, LoadIpAddressesFromStream) {
    std::string mock_input = 
        "192.168.1.1\t626437\n"
        "\n"
        "10.0.0.1\n" 
        "invalid_ip\t12314\n"
        "127.0.0.1\t\n";

    std::istringstream stream(mock_input);
    
    auto result = loadIpAddresses(stream);

    ASSERT_EQ(result.size(), 3);
    EXPECT_EQ(result[0], (std::vector<uint8_t>{192, 168, 1, 1}));
    EXPECT_EQ(result[1], (std::vector<uint8_t>{10, 0, 0, 1}));
    EXPECT_EQ(result[2], (std::vector<uint8_t>{127, 0, 0, 1}));
}

//---------------------------------------------------------------------------
// Tests sort
//---------------------------------------------------------------------------
TEST(IpPipelineTest, SortingOrderDescending) {
    std::vector<std::vector<uint8_t>> ips = {
        {10, 0, 0, 1},
        {192, 168, 1, 2},
        {192, 168, 1, 10},
        {192, 168, 0, 5},
        {1, 1, 1, 1}
    };

    // Сортировка по убыванию (лексикографическая)
    std::sort(ips.begin(), ips.end(), std::greater<std::vector<uint8_t>>());

    std::vector<std::vector<uint8_t>> expected = {
        {192, 168, 1, 10},
        {192, 168, 1, 2},
        {192, 168, 0, 5},
        {10, 0, 0, 1},
        {1, 1, 1, 1}
    };

    EXPECT_EQ(ips, expected);
}

//---------------------------------------------------------------------------
// Tests outIpByCondition
//---------------------------------------------------------------------------
TEST(IpLambdaTest, FilterFirstOctetIsOne) {
    std::vector<std::vector<uint8_t>> mock_ips = {
        {1, 2, 3, 4},
        {192, 168, 1, 1},
        {1, 255, 0, 0}
    };
    
    std::ostringstream output;
    
    outIpByCondition(output, mock_ips, [](const auto& el) { 
        return (el[0] == 1); 
    });
    
    std::string expected_output = "1.2.3.4\n1.255.0.0\n";
    EXPECT_EQ(output.str(), expected_output);
}

TEST(IpLambdaTest, FilterSpecificSubnet) {
    std::vector<std::vector<uint8_t>> mock_ips = {
        {46, 70, 1, 2},
        {46, 1, 70, 2},
        {46, 70, 0, 0}
    };
    
    std::ostringstream output;
    
    outIpByCondition(output, mock_ips, [](const auto& el) { 
        return (el[0] == 46) && (el[1] == 70); 
    });
    
    std::string expected_output = "46.70.1.2\n46.70.0.0\n";
    EXPECT_EQ(output.str(), expected_output);
}

TEST(IpLambdaTest, FilterAnyOctetContains46) {
    std::vector<std::vector<uint8_t>> mock_ips = {
        {46, 1, 2, 3},
        {1, 46, 2, 3},
        {1, 2, 3, 4},
        {1, 2, 3, 46}
    };
    
    std::ostringstream output;
    
    outIpByCondition(output, mock_ips, [](const auto& lip) { 
        return lip.end() != std::find(lip.begin(), lip.end(), 46); 
    });
    
    std::string expected_output = "46.1.2.3\n1.46.2.3\n1.2.3.46\n";
    EXPECT_EQ(output.str(), expected_output);
}

//---------------------------------------------------------------------------
// Tests outIpByCondition
//---------------------------------------------------------------------------
TEST(IpPipelineTest, FullPipelineEndToEnd) {

    std::string mock_input = 
        "1.1.1.1\t0634\t124\n"
        "46.70.2.1\t12516\t250\n"
        "\n" // Пустая строка
        "1.2.3.4\n" // Без таба
        "192.168.1.1\ttext\n"
        "256.0.0.46\ttext\n" // Невалидный IP
        "46.10.46.2\n";

    std::istringstream input_stream(mock_input);
    std::ostringstream output_stream(mock_input);

    processIpFilter(input_stream, output_stream);

    std::string expected_output = 
        // Сортировка по убыванию
        "192.168.1.1\n"
        "46.70.2.1\n"
        "46.10.46.2\n"
        "1.2.3.4\n"
        "1.1.1.1\n"
        // Начинаются на 1
        "1.2.3.4\n"
        "1.1.1.1\n"
        // Начинаются на 46.70
        "46.70.2.1\n"
        // Содержат число 46
        "46.70.2.1\n"
        "46.10.46.2\n";

    EXPECT_EQ(output_stream.str(), expected_output);
}