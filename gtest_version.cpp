#include "lib.h"

#include <gtest/gtest.h>

//---------------------------------------------------------------------------
TEST(VersionTest, CheckPatchVersion) {
    // Пример проверки, замените на вашу реальную логику
    EXPECT_GT(version(), 0); 
}

//---------------------------------------------------------------------------
// check splitIpToNum
//---------------------------------------------------------------------------
TEST(SplitIpToNumTest, ValidIpAddresses) {
    // Классический локалхост
    EXPECT_EQ(splitIpToNum("127.0.0.1", '.'), (std::vector<uint8_t>{127, 0, 0, 1}));
    
    // Минимальные границы октетов
    EXPECT_EQ(splitIpToNum("0.0.0.0", '.'), (std::vector<uint8_t>{0, 0, 0, 0}));
    
    // Максимальные границы октетов
    EXPECT_EQ(splitIpToNum("255.255.255.255", '.'), (std::vector<uint8_t>{255, 255, 255, 255}));
    
    // Обычный сетевой адрес
    EXPECT_EQ(splitIpToNum("192.168.1.100", '.'), (std::vector<uint8_t>{192, 168, 1, 100}));
    
    // Другой разделитель (например, дефис)
    EXPECT_EQ(splitIpToNum("10-0-0-1", '-'), (std::vector<uint8_t>{10, 0, 0, 1}));
}

//---------------------------------------------------------------------------
TEST(SplitIpToNumTest, InvalidOctetCount) {
    // Слишком мало октетов
    EXPECT_TRUE(splitIpToNum("192.168.1", '.').empty());
    EXPECT_TRUE(splitIpToNum("192.168", '.').empty());
    EXPECT_TRUE(splitIpToNum("192", '.').empty());
    
    // Слишком много октетов
    EXPECT_TRUE(splitIpToNum("192.168.1.1.5", '.').empty());
    EXPECT_TRUE(splitIpToNum("1.2.3.4.5.6.7.8", '.').empty());
}

//---------------------------------------------------------------------------
TEST(SplitIpToNumTest, OutOfByteRange) {
    // Число больше 255
    EXPECT_TRUE(splitIpToNum("256.100.100.100", '.').empty());
    EXPECT_TRUE(splitIpToNum("192.168.1.300", '.').empty());
    
    // Отрицательные числа
    EXPECT_TRUE(splitIpToNum("-1.100.100.100", '.').empty());
    EXPECT_TRUE(splitIpToNum("192.168.-1.1", '.').empty());
}

//---------------------------------------------------------------------------
TEST(SplitIpToNumTest, InvalidCharacters) {
    // Буквы вместо цифр или внутри них
    EXPECT_TRUE(splitIpToNum("192.16a.1.1", '.').empty());
    EXPECT_TRUE(splitIpToNum("abc.def.ghi.jkl", '.').empty());
    
    // Пробелы (from_chars считает их невалидными символами)
    EXPECT_TRUE(splitIpToNum("192 .168.1.1", '.').empty());
    EXPECT_TRUE(splitIpToNum("192.168.1. 1", '.').empty());
}

//---------------------------------------------------------------------------
TEST(SplitIpToNumTest, DelimiterAndEdgeCases) {
    // Пустая строка
    EXPECT_TRUE(splitIpToNum("", '.').empty());
    
    // Дублирующиеся разделители (пустые октеты внутри)
    EXPECT_TRUE(splitIpToNum("192..1.1", '.').empty());
    EXPECT_TRUE(splitIpToNum("192.168.1..", '.').empty());
    
    // Точка в самом начале
    EXPECT_TRUE(splitIpToNum(".192.168.1.1", '.').empty());
    
    // Точка в самом конце
    EXPECT_TRUE(splitIpToNum("192.168.1.1.", '.').empty());
    
    // Строка состоит только из разделителей
    EXPECT_TRUE(splitIpToNum("...", '.').empty());
}
