#include <array>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

int main()
{
    std::ifstream file{"./input"};
    std::array<std::array<bool, 100>, 100> rules;
    std::vector<std::vector<int>> updates;
    std::string line;
    std::istringstream lineStream;
    std::memset(&rules[0], 0, sizeof(rules));
    int a, c;
    char b;

    while (std::getline(file, line))
    {
        // Detects the newline between the rules and the updates
        if (line.size() == 0)
            break;
        lineStream = std::istringstream(line);
        lineStream >> a >> b >> c;
        // If rules[x][y] is true, then x is before y
        rules[a][c] = true;
    }
    while (std::getline(file, line))
    {
        lineStream = std::istringstream(line);
        std::vector<int> update;
        for (; lineStream >> a;)
        {
            update.push_back(a);
            if (lineStream.peek() == ',')
                lineStream.ignore();
        }
        updates.push_back(update);
    }
    int result = 0;
    int result2 = 0;
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
                    // Reset j so that j++ is i+1.
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