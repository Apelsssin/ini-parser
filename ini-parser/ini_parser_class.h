#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <tuple>
#include <algorithm>
#include <variant>
#include <cstring>
#include "Exceptions.h"

namespace ini_parser {
    class ini_parser {
        std::vector <std::pair <std::string, std::variant <int, double, std::string> >> v_of_variables;

    public:
        ini_parser() = delete;
        ini_parser(std::string file_location) {
            std::ifstream file(file_location);
            std::string s;
            std::string s_first;
            std::string s_second;
            std::string s_first_; // хранит название секции
            bool check = false;
            bool empty_value = false;
            std::variant <int, double, std::string> n;

            auto push_v = [&]() {
                std::pair <std::string, std::variant <int, double, std::string> > variable(s_first, n);
                v_of_variables.push_back(variable);
                s_second = "";
                s_first = s_first_;
                };

            while (!file.eof()) {
                file >> s;
                if (s[0] == '[' && check && !empty_value || s[0] == ';' && check && !empty_value) {
                    push_v();
                    check = false;
                }
                if (!s_first.empty() && s[0] == '[')
                    s_first = "";
                if (s[0] == '[') {
                    s_first = s.substr(1, s.length() - 2);
                    s_first += ".";
                    s_first_ = s_first;
                    empty_value = false;
                }
                else if (s[0] != ';') {
                    if (check && s.find('=') == std::string::npos && s.find('[') == std::string::npos && !empty_value) {
                        s_second = s_second + " " + s;
                    }
                    else if (check && s.find('=') != std::string::npos && !empty_value || check && s.find('[') != std::string::npos && !empty_value) {
                        check = false;
                        push_v();
                    }
                    if (!check) {
                        size_t i = 0;
                        if (s.find('=') != std::string::npos) {
                            while (s[i] != '=') {
                                s_first += s[i];
                                ++i;
                            }
                            ++i;
                            if (s[i] != 0) {
                                while (i < s.size() && s[i] != ';') {
                                    s_second += s[i];
                                    ++i;
                                }
                            }
                            else {
                                empty_value = true;
                                s_first = s_first_;
                            }
                        }
                        else {
                            while (i < s.size()) {
                                s_first += s[i];
                                ++i;
                            }
                            std::getline(file >> s, s);
                            i = 1;
                            if (s == "=") {
                                std::getline(file >> s, s);
                                i = 0;
                            }
                            if (s[i] != 0) {
                                while (i < s.size() && s[i] != ';') {
                                    s_second += s[i];
                                    ++i;
                                }
                                if (s_second.empty()) {
                                    empty_value = true;
                                    s_first = s_first_;
                                }
                            }
                            else {
                                empty_value = true;
                                s_first = s_first_;
                            }
                        }

                    }
                    if (!empty_value) {
                        bool doubl = false;
                        if (s_second[0] != '-' && s_second[0] != '1' && s_second[0] != '2' && s_second[0] != '3' && s_second[0] != '4' && s_second[0] != '5'
                            && s_second[0] != '6' && s_second[0] != '7' && s_second[0] != '8' && s_second[0] != '9' && s_second[0] != '0') {
                            n = s_second;
                            check = true;
                        }
                        else {
                            for (int i = 0; i < s_second.length(); ++i)
                                if (s_second[i] == 46) {
                                    doubl = true; // "." - 46
                                    break;
                                }
                            if (doubl) {
                                n = std::atof(s_second.c_str());
                            }
                            else {
                                n = std::atoi(s_second.c_str());
                            }
                        }
                    }
                }
                else {
                    std::getline(file >> s, s);
                    empty_value = false;
                }
                if (!s_second.empty() && !check && s_second[s_second.size() - 1] != '.' && !empty_value) {
                    push_v();
                }
            }
            if (check) {
                push_v();
            }
            file.close();
        }
        ~ini_parser() = default;

        template <class T>
        T get_value(std::string value_name) {
            for (auto it : v_of_variables) {
                if (it.first == value_name)
                    if (std::holds_alternative <T>(it.second)) {
                        return std::get <T>(it.second);
                    }
                    else throw Exceptions("Попытка получить значение переменной другого типа!");
            }
            std::cout << "Искомого значения переменной " << value_name << "нет. Существуют переменные:\n";
            for (auto it : v_of_variables) {
                std::cout << it.first << std::endl;
            }
            return 0;
        }

        void sort();
    };
}