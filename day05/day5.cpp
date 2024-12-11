#include <array>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
void readRules(
            std::ifstream &file,
            std::array<std::array<bool, 100>, 100> &ruleArray)
{
    std::string line;
    int a, c;
    char b;
    while (std::getline(file, line))
    {
        std::istringstream lineStream(line);
        // If the line is empty then there are no more rules to read
        if (line.size() == 0)
            return;
        lineStream >> a >> b >> c;
        ruleArray[a][c] = true;
    }
}
void readUpdates(std::ifstream &file, std::vector<std::vector<int>> &updateVec)
{
    std::string line;
    while (std::getline(file, line))
    {
        std::istringstream lineStream(line);
        std::vector<int> update;
        for (int i; lineStream >> i;)
        {
            update.push_back(i);
            if (lineStream.peek() == ',')
                lineStream.ignore();
        }
        updateVec.push_back(update);
    }
}
int main()
{
    std::ifstream file{"./input"};
    std::array<std::array<bool, 100>, 100> rules;
    std::vector<std::vector<int>> updates;
    std::string line;
    std::istringstream lineStream;
    std::memset(&rules[0], 0, sizeof(rules));
    int result = 0;
    int result2 = 0;
    readRules(file, rules);
    readUpdates(file, updates);
    for (auto &update : updates)
    {
        bool valid = true;
        for (int i = 0; i < update.size() - 1; i++)
        {
            for (int j = i + 1; j < update.size(); j++)
            {
                if (rules[update[j]][update[i]])
                {
                    valid = false;
                    std::swap(update[j], update[i]);
                    // Reset j so that j++ is i+1 again
                    j = i;
                }
            }
        }
        if (valid)
            result += update[update.size() / 2];
        else
            result2 += update[update.size() / 2];
    }
    std::cout << "Part 1: " << result << "\n";
    std::cout << "Part 2: " << result2 << "\n";
}