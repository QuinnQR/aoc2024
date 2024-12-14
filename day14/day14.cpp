#include <fstream>
#include <iostream>
#include <sstream>
#include <tuple>
#include <vector>
constexpr int width   = 101;
constexpr int height  = 103;
constexpr int squares = width * height;
std::tuple<int, int, int, int> parseLine(const std::string &line)
{
    std::stringstream stream{line};
    int a, b, c, d;
    stream.ignore(2);
    stream >> a;
    stream.ignore(1);
    stream >> b;
    stream.ignore(3);
    stream >> c;
    stream.ignore(1);
    stream >> d;
    return {a, b, c, d};
}
std::pair<int, int> getPos(int pX, int pY, int vX, int vY, int seconds)
{
    int rX = (pX + (seconds % width) * vX) % width;
    int rY = (pY + (seconds % height) * vY) % height;
    if (rX < 0)
        rX = width + rX;
    if (rY < 0)
        rY = height + rY;
    return {rX, rY};
}
int getQuadrant(std::pair<int, int> pos)
{
    if (pos.first == width / 2 || pos.second == height / 2)
        return 4;
    int idx  = 0;
    idx     += pos.first < width / 2;
    idx     += 2 * (pos.second < height / 2);
    return idx;
}
void printLine(const std::vector<std::pair<int, int>> &positions)
{
    std::string output;
    output.resize((width + 1) * height, '.');
    for (int j = 0; j < height; j++)
    {
        output[width + j * (width + 1)] = '\n';
    }
    for (auto [pX, pY] : positions)
    {
        output[pX + pY * (width + 1)] = 'X';
    }
    std::cout << output << '\n';
}
void updatePos(std::pair<int, int> &pos, std::pair<int, int> vel)
{
    pos.first += vel.first;
    pos.first %= width;
    if (pos.first < 0)
        pos.first += width;
    pos.second += vel.second;
    pos.second %= height;
    if (pos.second < 0)
        pos.second += height;
}
void printIts(std::vector<std::pair<int, int>> positions,
              const std::vector<std::pair<int, int>> &vels)
{
    int s     = 0;
    char junk = 'a';
    while (junk != 'c')
    {
        printLine(positions);
        std::cout << s << "\n\n";
        std::cin >> junk;
        for (int i = 0; i < positions.size(); i++)
        {
            updatePos(positions[i], vels[i]);
        }
        s++;
    }
}

int main()
{
    std::ifstream file{"./input"};

    std::string line;
    int result[5]   = {0, 0, 0, 0, 0};
    int totalResult = 1;
    std::vector<std::pair<int, int>> positions;
    std::vector<std::pair<int, int>> velocities;
    while (std::getline(file, line))
    {
        auto [pX, pY, vX, vY] = parseLine(line);
        int quad              = getQuadrant(getPos(pX, pY, vX, vY, 100));
        result[quad]++;
        positions.push_back({pX, pY});
        velocities.push_back({vX, vY});
    }
    for (int i = 0; i < 4; i++)
    {
        totalResult *= result[i];
    }
    std::cout << totalResult << "\n";
    printIts(std::move(positions), velocities);
}