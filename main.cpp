#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <algorithm>
#include <stdexcept>
#include <array>
#include <range/v3/range/conversion.hpp>
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

template <typename ...Args>
void filter(const ip_pool& pool, Args...args) 
{
	std::array<int, sizeof...(args)> temp = { args... };
	ranges::for_each(pool | ranges::views::filter([&temp](auto it) { return std::equal(it.begin(), it.begin() + sizeof...(args), temp.begin());}),
		[](auto i) {  print_ip_addr(i); });
}

void filter_any(const ip_pool& pool, int a)
{
	ranges::for_each(pool | ranges::views::filter([a=a](auto it) { return ranges::any_of(it,[a=a](auto v_i) {return v_i == a; }); }), 
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
            ip_pool.emplace(ip_addr{ std::stoi(v2[0]), std::stoi(v2[1]), std::stoi(v2[2]), std::stoi(v2[3])});
	//не заработал вариант с range. похоже, что моя ошибка в том, что я пытаюсь изменить тип функцией трансформ, а не значение 
	//ip_pool.emplace(ranges::views::for_each(v2 | ranges::views::transform([](auto i){return std::stoi(i);})) | ranges::to<std::array>());
			
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
