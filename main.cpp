#include "lib.h"
#include <limits>

//---------------------------------------------------------------------------
int main(int, char **) 
{
    std::string line;
    std::vector<std::vector<uint8_t>> ip;
    while(std::getline(std::cin, line, '\t'))
    {
        ip.push_back(splitIpToNum(line, '.'));
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    std::sort(ip.begin(), ip.end(), std::greater<std::vector<uint8_t>>());

    for(const auto& i : ip)
    {
        std::cout << int(i[0]) << "." << int(i[1]) << "." << int(i[2]) << "." << int(i[3]) << std::endl;
    }

    coutByCondition(ip, [](const auto& el){
        return el.size() && (el[0] == 1);
    });

    coutByCondition(ip, [](const auto& el){
        return (el.size() >= 2) && (el[0] == 46) && (el[1] == 70);
    });

    coutByCondition(ip, [](const auto& lip){
        return (lip.size() >= 4) &&
               lip.end() != std::find_if(lip.begin(), lip.end(), [](const auto& el){
                                            return (el == 46);
                                        });
    });

    return 0;
}
