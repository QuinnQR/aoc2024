#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

struct Context
{
    std::string data;
    std::string originalData;
    int nCols, nRows;
    const char up = 1, right = 2, down = 4, left = 8;
    inline bool inBounds(int x, int y)
    {
        return 0 <= x && 0 <= y && x < nCols && y < nRows;
    }
    inline int toIdx(int x, int y) { return x + y * nCols; }
    int getEdges(int x, int y, char target, std::string &edgeFlags)
    {
        char edgeFlag = 0;
        int edgeCount = 0;
        if (!inBounds(x - 1, y) || originalData[toIdx(x - 1, y)] != target)
        {
            edgeFlag  |= left;
            edgeCount += 2;
            if (inBounds(x, y - 1) && originalData[toIdx(x, y - 1)] == target)
                if (edgeFlags[toIdx(x, y - 1)] & left)
                    edgeCount -= 2;
            if (inBounds(x, y + 1) && originalData[toIdx(x, y + 1)] == target)
                if (edgeFlags[toIdx(x, y + 1)] & left)
                    edgeCount -= 2;
        }
        if (!inBounds(x + 1, y) || originalData[toIdx(x + 1, y)] != target)
        {
            edgeFlag  |= right;
            edgeCount += 2;
            if (inBounds(x, y - 1) && originalData[toIdx(x, y - 1)] == target)
                if (edgeFlags[toIdx(x, y - 1)] & right)
                    edgeCount -= 2;
            if (inBounds(x, y + 1) && originalData[toIdx(x, y + 1)] == target)
                if (edgeFlags[toIdx(x, y + 1)] & right)
                    edgeCount -= 2;
        }
        if (!inBounds(x, y + 1) || originalData[toIdx(x, y + 1)] != target)
        {
            edgeFlag  |= down;
            edgeCount += 2;
            if (inBounds(x - 1, y) && originalData[toIdx(x - 1, y)] == target)
                if (edgeFlags[toIdx(x - 1, y)] & down)
                    edgeCount -= 2;
            if (inBounds(x + 1, y) && originalData[toIdx(x + 1, y)] == target)
                if (edgeFlags[toIdx(x + 1, y)] & down)
                    edgeCount -= 2;
        }
        if (!inBounds(x, y - 1) || originalData[toIdx(x, y - 1)] != target)
        {
            edgeFlag  |= up;
            edgeCount += 2;
            if (inBounds(x - 1, y) && originalData[toIdx(x - 1, y)] == target)
                if (edgeFlags[toIdx(x - 1, y)] & up)
                    edgeCount -= 2;
            if (inBounds(x + 1, y) && originalData[toIdx(x + 1, y)] == target)
                if (edgeFlags[toIdx(x + 1, y)] & up)
                    edgeCount -= 2;
        }
        edgeFlags[toIdx(x, y)] = edgeFlag;
        return edgeCount;
    };

    std::pair<int, int> fillRegion(int x, int y, char target)
    {
        int area      = 0;
        int perimeter = 0;
        int edges     = 0;
        std::vector<std::pair<int, int>> stack;
        std::string edgeFlags;
        edgeFlags.resize(data.size());
        stack.emplace_back(x, y);
        while (stack.size() > 0)
        {
            // Assume if it's in the stack that it was valid when added.
            auto [xIdx, yIdx] = stack.back();
            stack.pop_back();

            if (data[toIdx(xIdx, yIdx)] != ' ')
            {
                area++;
                perimeter += 4;
                if (inBounds(xIdx + 1, yIdx)
                    && data[toIdx(xIdx + 1, yIdx)] == target)
                {
                    perimeter -= 2;
                    stack.emplace_back(xIdx + 1, yIdx);
                }
                if (inBounds(xIdx - 1, yIdx)
                    && data[toIdx(xIdx - 1, yIdx)] == target)
                {
                    perimeter -= 2;
                    stack.emplace_back(xIdx - 1, yIdx);
                }
                if (inBounds(xIdx, yIdx + 1)
                    && data[toIdx(xIdx, yIdx + 1)] == target)
                {
                    perimeter -= 2;
                    stack.emplace_back(xIdx, yIdx + 1);
                }
                if (inBounds(xIdx, yIdx - 1)
                    && data[toIdx(xIdx, yIdx - 1)] == target)
                {
                    perimeter -= 2;
                    stack.emplace_back(xIdx, yIdx - 1);
                }
                data[toIdx(xIdx, yIdx)] = ' ';
                edges += getEdges(xIdx, yIdx, target, edgeFlags);
            }
        }
        std::cout << target << ":\t" << edges / 2 << "\n";
        return {area * perimeter, area * edges / 2};
    }
    std::pair<int, int> getResults()
    {
        std::pair<int, int> result = {0, 0};
        int idx                    = 0;
        for (int y = 0; y < nRows; y++)
        {
            for (int x = 0; x < nCols; x++)
            {
                if (data[idx] != ' ')
                {
                    auto [part1, part2]  = fillRegion(x, y, data[idx]);
                    result.first        += part1;
                    result.second       += part2;
                }
                idx++;
            }
        }
        return result;
    }
};
int main()
{
    std::ifstream file{"./input"};
    std::string line;
    std::stringstream buffer;
    Context context;
    int nCols;
    while (std::getline(file, line))
    {
        nCols = line.size();
        buffer << line;
    }
    context.data            = buffer.str();
    int nRows               = context.data.size() / (nCols);
    context.originalData    = context.data;
    context.nCols           = nCols;
    context.nRows           = nRows;
    auto [result1, result2] = context.getResults();
    std::cout << "Part 1: " << result1 << '\n';
    std::cout << "Part 2: " << result2 << '\n';
}