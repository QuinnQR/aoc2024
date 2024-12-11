
#include <array>
#include <fstream>
#include <iostream>
#include <vector>
struct V2
{
    int x;
    int y;
    V2(int a, int b)
    {
        x = a;
        y = b;
    }
    inline V2 operator+(const V2 &other)
    {
        return V2(x + other.x, y + other.y);
    }
    inline V2 operator-(const V2 &other)
    {
        return V2(x - other.x, y - other.y);
    }
};
inline bool inBounds(V2 pos, int height, int length)
{
    return (pos.x >= 0) && (pos.x < length) && (pos.y < height) && (pos.y >= 0);
}
inline std::pair<int, int> findAntipodes(
            V2 newPos, const std::vector<V2> &prevPos, int height, int length,
            std::vector<bool> &antipodesPart1,
            std::vector<bool> &antipodesPart2)
{
    int count1 = 0;
    int count2 = 0;
    for (auto p : prevPos)
    {
        V2 difference = newPos - p;
        V2 antipode = newPos + difference;
        if (inBounds(antipode, height, length))
        {
            count1 += !antipodesPart1[antipode.x + antipode.y * length];
            antipodesPart1[antipode.x + antipode.y * length] = true;
        }
        while (inBounds(antipode, height, length))
        {
            count2 += !antipodesPart2[antipode.x + antipode.y * length];
            antipodesPart2[antipode.x + antipode.y * length] = true;
            antipode = antipode + difference;
        }
        difference = p - newPos;
        antipode = p + difference;
        if (inBounds(antipode, height, length))
        {
            count1 += !antipodesPart1[antipode.x + antipode.y * length];
            antipodesPart1[antipode.x + antipode.y * length] = true;
        }
        while (inBounds(antipode, height, length))
        {
            count2 += !antipodesPart2[antipode.x + antipode.y * length];
            antipodesPart2[antipode.x + antipode.y * length] = true;
            antipode = antipode + difference;
        }
        count2 += !antipodesPart2[p.x + p.y * length];
        antipodesPart2[p.x + p.y * length] = true;
    }
    if (prevPos.size() > 0)
    {
        count2 += !antipodesPart2[newPos.x + newPos.y * length];
        antipodesPart2[newPos.x + newPos.y * length] = true;
    }
    return {count1, count2};
}
int main()
{
    std::array<std::vector<V2>, 128> positions;
    std::vector<bool> antipodesPart1;
    std::vector<bool> antipodesPart2;
    std::ifstream file{"./input"};
    std::string line;
    std::vector<std::string> data;
    while (std::getline(file, line))
    {
        data.push_back(line);
    }
    int height = data.size();
    int length = data[0].size();
    antipodesPart1.resize(height * length, false);
    antipodesPart2.resize(height * length, false);
    int result1 = 0;
    int result2 = 0;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < length; j++)
        {
            if (data[i][j] == '.')
                continue;
            auto [r1, r2] = findAntipodes(
                        {j, i}, positions[data[i][j]], height, length,
                        antipodesPart1, antipodesPart2);
            positions[data[i][j]].push_back({j, i});
            result1 += r1;
            result2 += r2;
        }
    }
    std::cout << "Part 1: " << result1 << '\n';
    std::cout << "Part 2: " << result2 << '\n';
}