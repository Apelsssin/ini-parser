#include "ini_parser_class.h"
namespace ini_parser {
    void ini_parser::sort() {
        std::stable_sort(v_of_variables.begin(), v_of_variables.end(),
            [](std::pair < std::string, std::variant <int, double, std::string> > pair1,
                std::pair < std::string, std::variant <int, double, std::string> > pair2
                ) { return pair1.first < pair2.first; });
        auto it = std::unique(v_of_variables.begin(), v_of_variables.end(),
            [&](std::pair<std::string, std::variant <int, double, std::string>>& pair1,
                std::pair < std::string, std::variant <int, double, std::string>>& pair2
                ) {
                    if (pair1.first == pair2.first)
                        std::swap(pair1.second, pair2.second);
                    return pair1.first == pair2.first; });

        v_of_variables.erase(it, v_of_variables.end());
    }
}