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

std::default_random_engine random_engine(std::chrono::system_clock::now().time_since_epoch().count());

int get_random(const std::set< int > excluded, int max_n)
{
    while (true) {
        int value = std::abs(random_engine() % max_n);
        if (excluded.find(value) == excluded.end()) {
            return value;
        }
    }
}

void fill_vector(std::vector< int >& vec, int size, std::map< int, float > frequencies, int max_n)
{
    vec.clear();
    vec.reserve(size);
    std::set< int > excluded;
    for (auto pair : frequencies) {
        int value = pair.first;
        int count = int(size * pair.second);
        for (int i = 0; i < count; i++) {
            vec.push_back(value);
        }
        excluded.insert(value);
    }
    int rest = size - vec.size();
    for (int i = 0; i < rest; i++) {
        vec.push_back(get_random(excluded, max_n));
    }
    vec.resize(size);
    std::shuffle(vec.begin(), vec.end(), random_engine);
}

void print_vector(const char* prefix, const std::vector< int > vec)
{
    std::cout << prefix;
    for (auto value : vec) {
        std::cout << " " << value;
    }
    std::cout << std::endl;
}

int find_number(const std::vector< int >& vec, int m)
{
    std::map< int, int > most_frequent_numbers;
    int index = 0;
    for (auto value : vec) {
        if (index != 0) {
            for (auto pos = most_frequent_numbers.begin(); pos != most_frequent_numbers.end(); ++pos) {
                pos->second -= 1;
            }
        }
        int diff = m + 1;
        int buffer_size = m + 1;
        if (most_frequent_numbers.find(value) != most_frequent_numbers.end()) {
            // Increase counter of existing value.
            most_frequent_numbers[value]+=diff;
        } else if (most_frequent_numbers.size() < buffer_size) {
            // Add new item to the table. Frequency is 1.
            most_frequent_numbers.insert(std::make_pair(value, diff));
        } else {
            // Replace the least frequent item in the table.
            auto pos = std::min_element(most_frequent_numbers.begin(), most_frequent_numbers.end(), [](std::pair< int, int > p1, std::pair< int, int > p2) {
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
//    for (auto value : most_frequent_numbers) {
//        std::cout << " " << value.first << ", " << value.second << " | ";
//    }
//    std::cout << "\n";
    auto pos = std::max_element(most_frequent_numbers.begin(), most_frequent_numbers.end(), [](std::pair< int, int > p1, std::pair< int, int > p2) {
        return p1.second < p2.second;
    });
    return pos->first;
}

float get_frequency(const std::vector< int >& vec, int value) {
    int count = 0;
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
        std::vector< int > input;
        fill_vector(input, 30, std::map< int, float >{ { 1, 0.34 } }, 10);
        print_vector("INPUT ", input);
        std::cout << "RESULT: " << find_number(input, 3) << std::endl;
    } else if (argc == 2 && std::strcmp(argv[1], "-t") == 0) {
        constexpr int NUM_TESTS = 100000;
        for (int max_n = 4; max_n <= 30; max_n++) {
            int success = 0;
            int fail = 0;
            for (int i = 0; i < NUM_TESTS; i++) {
                std::vector< int > input;
                fill_vector(input, 450, std::map< int, float >{ { 1, 0.34 } }, max_n);

                int value = find_number(input, 3);
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
