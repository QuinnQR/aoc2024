#include <cinttypes>
#include <fstream>
#include <gmpxx.h>
#include <iostream>
#include <map>
#include <unordered_map>
#include <vector>

typedef long long num_t;
inline int countDigits(num_t value)
{
    num_t ten = 10;
    int count = 1;
    while (ten <= value)
    {
        count++;
        ten *= 10;
    }
    return count;
}
inline num_t getSplittingNumber(num_t value, int digits)
{
    num_t ten = 1;
    for (int i = 0; i < digits; i += 2)
    {
        ten *= 10;
    }
    return ten;
}
typedef std::unordered_map<num_t, num_t> map_t;
std::pair<num_t, map_t> countStones(map_t &&initialArray, int depth)
{
    map_t array = initialArray;
    map_t newArray;
    for (int i = 0; i < depth; i++)
    {
        newArray.clear();
        for (auto [value, count] : array)
        {
            if (value == 0)
            {
                newArray[1] += count;
            }
            int digits = countDigits(value);
            if ((digits & 1) == 0)
            {
                num_t splitter              = getSplittingNumber(value, digits);
                newArray[value / splitter] += count;
                newArray[value % splitter] += count;
            }
            else if (value)
            {
                newArray[value * 2024] += count;
            }
        }
        array = std::move(newArray);
    }
    num_t result = 0;
    for (auto [value, count] : array)
    {
        result += count;
    }
    return {result, array};
}
int main()
{
    std::ifstream file{"./input"};
    num_t input;
    if (file.peek() == EOF)
    {
        std::cout << "File is empty or does not exist\n";
        return 1;
    }
    std::unordered_map<num_t, num_t> initialArray;
    do
    {
        file >> input;
        initialArray[input] += 1;
    } while (!file.eof());
    auto [result1, newArray] = countStones(std::move(initialArray), 25);
    auto [result2, _]        = countStones(std::move(newArray), 50);
    std::cout << "Part 1: " << result1 << "\n";
    std::cout << "Part 2: " << result2 << "\n";
}