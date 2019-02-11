#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <random>
#include <chrono>
#include <algorithm>
#include <utility>
#include <cstring>

using namespace std;
using uint = unsigned int;

std::default_random_engine random_engine(std::chrono::system_clock::now().time_since_epoch().count());

uint get_random(const std::set< uint > excluded, uint max_n)
{
    while (true) {
        uint value = random_engine() % max_n;
        if (excluded.find(value) == excluded.end()) {
            return value;
        }
    }
}

void fill_vector(std::vector< uint >& vec, uint size, std::map< uint, float > frequencies, uint max_n)
{
    vec.clear();
    vec.reserve(size);
    std::set< uint > excluded;
    for (auto pair : frequencies) {
        uint value = pair.first;
        uint count = uint(size * pair.second);
        for (uint i = 0; i < count; i++) {
            vec.push_back(value);
        }
        excluded.insert(value);
    }
    uint rest = size - vec.size();
    for (uint i = 0; i < rest; i++) {
        vec.push_back(get_random(excluded, max_n));
    }
    vec.resize(size);
    std::shuffle(vec.begin(), vec.end(), random_engine);
}

void print_vector(const char* prefix, const std::vector< uint > vec)
{
    std::cout << prefix;
    for (auto value : vec) {
        std::cout << " " << value;
    }
    std::cout << std::endl;
}

uint find_number(const std::vector< uint >& vec, uint m)
{
    std::map< uint, int > most_frequent_numbers;
    uint index = 0;
    for (auto value : vec) {
        if (index != 0) {
            for (auto pos = most_frequent_numbers.begin(); pos != most_frequent_numbers.end(); ++pos) {
                pos->second -= 1;
            }
        }
        uint diff = m + 1;
        uint buffer_size = m + 1;
        if (most_frequent_numbers.find(value) != most_frequent_numbers.end()) {
            // Increase counter of existing value.
            most_frequent_numbers[value]+=diff;
        } else if (uint(most_frequent_numbers.size()) < buffer_size) {
            // Add new item to the table. Frequency is 1.
            most_frequent_numbers.insert(std::make_pair(value, diff));
        } else {
            // Replace the least frequent item in the table.
            auto pos = std::min_element(most_frequent_numbers.begin(), most_frequent_numbers.end(), [](std::pair< uint, int > p1, std::pair< uint, int > p2) {
                return p1.second < p2.second;
            });
            most_frequent_numbers.erase(pos);
            most_frequent_numbers.insert(std::make_pair(value, diff));
        }

        index++;
    }
    for (auto pos = most_frequent_numbers.begin(); pos != most_frequent_numbers.end(); ++pos) {
        pos->second = 0;
    }
    for (auto value : vec) {
        if (most_frequent_numbers.find(value) != most_frequent_numbers.end()) {
            most_frequent_numbers[value]++;
        }
    }
    auto pos = std::max_element(most_frequent_numbers.begin(), most_frequent_numbers.end(), [](std::pair< uint, int > p1, std::pair< uint, int > p2) {
        return p1.second < p2.second;
    });
    return pos->first;
}

float get_frequency(const std::vector< uint >& vec, uint value)
{
    uint count = 0;
    for (auto v : vec) {
        if (v == value) {
            count++;
        }
    }
    return float(count) / vec.size();
}

int main(int argc, char** argv)
{
    if (argc == 2 && std::strcmp(argv[1], "-c") == 0) {
        std::vector< uint > input;
        fill_vector(input, 30, std::map< uint, float >{ { 1, 0.34 } }, 10);
        print_vector("INPUT ", input);
        std::cout << "RESULT: " << find_number(input, 3) << std::endl;
    } else if (argc == 2 && std::strcmp(argv[1], "-t") == 0) {
        constexpr uint NUM_TESTS = 100000;
        for (uint max_n = 4; max_n <= 30; max_n++) {
            uint success = 0;
            uint fail = 0;
            for (uint i = 0; i < NUM_TESTS; i++) {
                std::vector< uint > input;
                fill_vector(input, 450, std::map< uint, float >{ { 1, 0.34 } }, max_n);

                uint value = find_number(input, 3);
                float frequency = get_frequency(input, value);
                if (frequency >= float(1) / 3) {
                    success++;
                } else {
                    std::cout << "========================\n";
                    print_vector("INPUT = ", input);
                    std::cout << "Output " << value << "\n";
                    std::cout << "Frequency " << frequency << "\n";
                    std::cout << "========================\n";
                    fail++;
                    return 0;
                }
            }
            std::cout << "#" << max_n << "Number of tests: " << NUM_TESTS << ", success = " << success << ", fail = " << fail << std::endl;
        }
    }
    return 0;
}
