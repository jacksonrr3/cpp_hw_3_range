//#include <cassert>
//#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <algorithm>
#include <stdexcept>

#include <fstream>

//#include <range/v3/all.hpp>
//#include <meta/meta.hpp>
//#include <range/v3/view/view.hpp>
//#include <range/v3/view/transform.hpp>
#include <range/v3/view.hpp>
#include <range/v3/algorithm.hpp>


using ip_addr = std::vector<int>;
using ip_pool = std::multiset<ip_addr, std::greater<>>;

// ("",  '.') -> [""]
// ("11", '.') -> ["11"]
// ("..", '.') -> ["", "", ""]
// ("11.", '.') -> ["11", ""]
// (".11", '.') -> ["", "11"]
// ("11.22", '.') -> ["11", "22"]
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
    ip_addr temp = { args... };

    //поиск итератора на первый элемент последовательности подходящих по условию фильтра элементов
    auto it_start = std::lower_bound(pool.begin(), pool.end(), temp,
                                     [](auto a, auto b) {return !((std::equal(a.begin(), a.begin() + sizeof...(args), b.begin())) || (a < b));});
    if (it_start == pool.end()) { return; }

    //вывод элементов пока совпадение с условием фильтра и проверка на выход на границу контейнера
    do {
        print_ip_addr(*it_start);
        if (++it_start == pool.end()) { return; }
    }
    while (std::equal(it_start->begin(), it_start->begin() + sizeof...(args), temp.begin()));

    /*	//предыдущий вариант реализации фильтра, с поиском итераторов на первый и последний элемент подходящие по свойствам.
    //поиск итератора на последний элемент последовательности подходящих по условию фильтра элементов
    auto it_finish = std::upper_bound(it_start, pool.end(), temp,
        [](auto b, auto a) {return !((std::equal(a.begin(), a.begin() + sizeof...(args), b.begin())) || (a > b));});

    //вывод отфильтрованных элементов
    for (;it_start != it_finish; ++it_start)
    {
        print_ip_addr(*it_start);
    }
    */
}

void filter_any(const ip_pool& pool, int a)
{

    ranges::for_each(pool | ranges::view::filter([a=a](auto it) { return ranges::any_of(it,[a=a](auto v_i) {return v_i == a; }); }), [](auto i) {  print_ip_addr(i); });
 // ranges::for_each(pool | ranges::view::filter([a=a](auto it) { return std::any_of(it.begin(), it.end(), [a=a](auto vec) {return vec == a; }); }), [](auto i) {  print_ip_addr(i); });

/*
    for (auto it : pool)
    {
        if (std::any_of(it.begin(), it.end(), [a=a](auto vec) {return vec == a; }))
        {
            print_ip_addr(it);
        }
    }
*/
}

int main()
{
    try
    {
        std::ifstream cin_f("/home/jacksonrr3/Downloads/ip_filter.tsv");

        ip_pool ip_pool;

        for (std::string line; std::getline(cin_f, line);)
        {
            auto v1 = (split(line, '\t'));
            auto v2 = (split(v1.at(0), '.'));
          //  ip_pool.emplace(ip_addr{ std::stoi(v2[0]), std::stoi(v2[1]), std::stoi(v2[2]), std::stoi(v2[3])});
          ip_pool.emplace(v2 | ranges::view::transform([](auto i){return std::stoi(i);}));
        }

        //выброс исключения и завершение программы при отсутствии входных данных
        if (ip_pool.empty()) { throw std::runtime_error("No input data!"); }

        // TODO reverse lexicographically sort
       // for (auto it : ip_pool)	{print_ip_addr(it);}
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

/*
        const std::string s{"hello"};

        ranges::for_each(s | ranges::view::filter([](auto c) { return c == 'l'; }),
                         [](auto i) { std::cout << i << std::endl; });
*/
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
