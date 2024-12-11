#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>
struct V2
{
    int first, second;
    inline V2(int a, int b)
    {
        first  = a;
        second = b;
    }
    inline V2 operator+(const V2 &other)
    {
        return V2(other.first + first, other.second + second);
    }
    void operator+=(const V2 &other)
    {
        first  += other.first;
        second += other.second;
    }
};
struct Context
{
    int nCols;
    int nRows;
    std::vector<int> topology;
    Context(int cols, int rows, std::vector<int> &&heightMap)
    {
        nCols    = cols;
        nRows    = rows;
        topology = heightMap;
    }
    inline bool inBounds(int x, int y)
    {
        return 0 <= x && 0 <= y && x < nCols && y < nRows;
    }
    inline int getIdx(int x, int y) { return x + y * nCols; }
    V2 getValue(int x, int y, int depth, std::vector<int> &seen)
    {
        int idx = getIdx(x, y);
        if (!inBounds(x, y) || topology[idx] != depth)
            return {0, 0};
        if (depth == 9)
        {
            bool isNew = seen.end() == std::find(seen.begin(), seen.end(), idx);
            if (isNew)
            {
                seen.push_back(idx);
                return {1, 1};
            }
            return {0, 1};
        }
        return getValue(x + 1, y, depth + 1, seen)
               + getValue(x - 1, y, depth + 1, seen)
               + getValue(x, y + 1, depth + 1, seen)
               + getValue(x, y - 1, depth + 1, seen);
    }

    std::pair<int, int> getResults()
    {
        std::vector<int> seen;
        int idx   = 0;
        V2 result = {0, 0};
        for (int yIdx = 0; yIdx < nRows; yIdx++)
        {
            for (int xIdx = 0; xIdx < nCols; xIdx++)
            {
                if (!topology[idx])
                {
                    seen.clear();
                    result += getValue(xIdx, yIdx, 0, seen);
                }
                idx++;
            }
        }
        return {result.first, result.second};
    }
};

int main()
{
    std::vector<int> topology;
    std::ifstream file{"./input"};
    char ch;
    int nRows = 1;
    while ((ch = file.get()) != EOF)
    {
        if (ch == '\n')
            nRows++;
        else
            topology.push_back(ch - '0');
    }
    int nCols = topology.size() / nRows;
    Context context{nCols, nRows, std::move(topology)};
    auto [part1, part2] = context.getResults();
    std::cout << "Part 1: " << part1 << "\n";
    std::cout << "Part 2: " << part2 << "\n";
}