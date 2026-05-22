#include <ip_filter/ip_filter.h>

//---------------------------------------------------------------------------
int main(int, char **) 
{
    IpFilter filter;

    // Читаем данные из стандартного ввода (например, cin или файла)
    filter.load(std::cin);

    // Сортируем по убыванию
    filter.sortDescending();

    // Выводим весь список
    filter.printAll(std::cout);

    // Фильтр: IP, начинающиеся на 1 (от 1.0.0.0 до 1.255.255.255)
    filter.printRange(std::cout, IpAddress(1,0,0,0), IpAddress(1,255,255,255));

    // Фильтр: IP, начинающиеся на 46.70 (от 46.70.0.0 до 46.70.255.255)
    filter.printRange(std::cout, IpAddress(46,70,0,0), IpAddress(46,70,255,255));

    // Фильтр: любой байт равен 46
    filter.printIf(std::cout, [](const IpAddress& ip) {
        return ip.contains(46);
    });

    return 0;
}
