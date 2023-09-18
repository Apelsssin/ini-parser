#include <iostream>
#include <string>
#include "ini_parser_class.h"

void prev_check(std::string file_location) {
    std::ifstream file(file_location);
    if (!file.is_open()) throw Exceptions("Попытка открыть несуществующий файл!");
    file.close();
}
int main()
{
    try {
        setlocale(LC_ALL, "en_US.UTF8");
        prev_check("file.ini");
        ini_parser::ini_parser parser("file.ini");
        parser.sort();
        auto value = parser.get_value<double>("Section1.var1");
        std::cout << parser.get_value<std::string>("Section2.var2");
        return 0;
    }
    catch (const Exceptions& e) {
        std::cout << "Ошибка: " << e.what() << std::endl;
    }
    catch (...) {
        std::cout << "Неизвестная ошибка!";
    }
}
