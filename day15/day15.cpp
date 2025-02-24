#include <fstream>
#include <iostream>
#include <vector>

int part1(std::string &map, const std::string &instructions,
          std::pair<int, int> position)
{
    for (char ch : instructions)
    {
    }
}

int main()
{
    std::ifstream file{"./test"};
    std::string line;
    std::string map;
    int nCols, nRows;
    std::pair<int, int> position;
    nRows = 0;
    while (std::getline(file, line) && line.size() > 0)
    {
        nRows++;
        nCols = line.size();
        map.append(line);
        for (int i = 0; i < nCols; i++)
        {
            if (line[i] == '@')
            {
                position = {i, nRows - 1};
            }
        }
    }
    std::string instructions;
    std::getline(file, instructions);
    std::cout << "Map is " << nRows << "x" << nCols << "\n";
    std::cout << "Instructions are " << instructions << "\n";
    std::cout << "Robot starts at " << position.first << ", " << position.second
              << "\n";
}