#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <algorithm>
#include <stdexcept>
#include <array>
#include <range/v3/view.hpp>
#include <range/v3/algorithm.hpp>

using ip_addr = std::array<int, 4>;
using ip_pool = std::multiset<ip_addr, std::greater<>>;

std::vector<std::string> split(const std::string &str, char d)
{
    std::vector<std::string> r;
    std::string::size_type start = 0;
    auto stop = str.find_first_of(d);

    while (stop != std::string::npos)
    {
        r.push_back(str.substr(start, stop - start));

        start = stop + 1;
        stop = str.find_first_of(d, start);
    }

    r.push_back(str.substr(start));
    return r;
}

void print_ip_addr(const ip_addr& ip)
{
    std::cout << ip[0] << "." << ip[1] << "." << ip[2] << "." << ip[3] << std::endl;
}

template <typename...Args>
void filter(const ip_pool& pool, Args...args)
{
    //ip_addr temp = { args... };
    std::array<int, sizeof...(args)> temp = { args... };
    //поиск итератора на первый элемент последовательности подходящих по условию фильтра элементов
    auto it_start = std::lower_bound(pool.begin(), pool.end(), temp,
                                    [](auto a, auto b) {return !((std::lexicographical_compare(a.begin(), a.begin() + sizeof...(args), b.begin(), b.end())) ||
			                                                      std::equal(a.begin(), a.begin() + sizeof...(args), b.begin()));});
    if (it_start == pool.end()) { return; }

    //вывод элементов пока совпадение с условием фильтра и проверка на выход на границу контейнера
    do {
        print_ip_addr(*it_start);
        if (++it_start == pool.end()) { return; }
    }
    while (std::equal(it_start->begin(), it_start->begin() + sizeof...(args), temp.begin()));
}

void filter_any(const ip_pool& pool, int a)
{
    ranges::for_each(pool | ranges::view::filter([a=a](auto it) { return ranges::any_of(it,[a=a](auto v_i) {return v_i == a; }); }), 
                     [](auto i) {  print_ip_addr(i); });
}

int main()
{
    try
    {
        ip_pool ip_pool;

        for (std::string line; std::getline(std::cin, line);)
        {
            auto v1 = (split(line, '\t'));
            auto v2 = (split(v1.at(0), '.'));
            //выброс исключения и завершение программы при не корректных входных данных
            if (v2.size() != 4) { throw std::runtime_error("Wrong input data!"); }
            ip_pool.emplace(v2 | ranges::view::transform([](auto i){return std::stoi(i);}) | to<std::array>());
        }

        //выброс исключения и завершение программы при отсутствии входных данных
        if (ip_pool.empty()) { throw std::runtime_error("No input data!"); }

        // TODO reverse lexicographically sort
        ranges::for_each(ip_pool, [](auto it){print_ip_addr(it);});

        // TODO filter by first byte and output
        // ip = filter(1)
        filter(ip_pool, 1);

        // TODO filter by first and second bytes and output
        // ip = filter(46, 70)
        filter(ip_pool, 46, 70);

        // TODO filter by any byte and output
        //ip = filter_any(46);
        filter_any(ip_pool, 46);

    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
