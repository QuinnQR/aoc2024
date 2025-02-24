#include <filesystem>
#include <fstream>
#include <inttypes.h>
#include <iostream>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <vector>
namespace day16
{

struct Position
{
    int32_t cost;
    uint8_t col;
    uint8_t row;
    bool dir;
    uint8_t pad = 0;
    Position(uint32_t p_cost, uint8_t p_col, uint8_t p_row, bool p_dir)
    {
        cost = p_cost;
        col  = p_col;
        row  = p_row;
        dir  = p_dir;
    }

    inline uint32_t getIdx(int nCols, int nRows)
    {
        return col + nCols * (row + nRows * dir);
    }
    inline uint16_t getSimpleIdx(int nCols, int nRows = 0)
    {
        // nRows included for parity with getIdx
        return col + nCols * (row);
    }
};

// Compare function to allow for a min heap of Positions
struct PositionCompare
{
    bool operator()(const Position &lhs, const Position &rhs)
    {
        return lhs.cost > rhs.cost;
    };
};

struct Input
{
    std::vector<std::string> data;
    int nCols;
    int nRows;
};
Input read_file(const std::filesystem::path &path)
{
    std::ifstream file{path};
    std::vector<std::string> data;
    data.reserve(141);
    std::string line;
    int nCols, nRows;
    while (std::getline(file, line))
    {
        data.push_back(std::move(line));
    }
    nRows = data.size();
    nCols = data.back().size();
    return {std::move(data), nCols, nRows};
}
struct Result1
{
    int part1;
    std::vector<uint32_t> visited;
    Position final_position;
};
Result1 part1(const std::vector<std::string> &data, int nCols, int nRows)
{
    std::priority_queue<Position, std::vector<Position>, PositionCompare> heap;
    heap.emplace(0, 1, 1, 0);
    std::vector<uint32_t> visited;
    visited.resize(nCols * nRows * 2, -1);
    while (heap.size() > 0)
    {
        auto current = heap.top();
        heap.pop();

        if (current.row >= nRows || current.col >= nCols)
            continue;
        if (data[nRows - current.row - 1][current.col] == '#')
            continue;
        if (visited[current.getIdx(nCols, nRows)] != -1)
            continue;

        // Map the current position and direction to the cost to reach. This is
        // the minimum cost possible.
        visited[current.getIdx(nCols, nRows)] = current.cost;
        if (data[nRows - current.row - 1][current.col] == 'E')
            return {current.cost, visited, current};

        current.cost++;
        current.col += !current.dir;
        current.row += current.dir;
        heap.push(current);
        current.col -= !current.dir + !current.dir;
        current.row -= current.dir + current.dir;
        heap.push(current);

        current.col  += !current.dir;
        current.row  += current.dir;
        current.cost += 999;
        current.dir   = !current.dir;
        heap.push(current);
    }
    return {0, std::vector<uint32_t>(), Position(0, 0, 0, 0)};
}
int part2(const std::vector<std::string> &data, int nCols, int nRows,
          std::vector<uint32_t> visited, Position final)
{
    // Part 2 works by doing a DFS search backwards from the end.
    // If the cost disagrees with that from part 1, we have strayed from an
    // optimal path so the node can be discarded.
    std::vector<bool> revisited;
    revisited.resize(nCols * nRows, false);
    std::vector<Position> stack;
    stack.push_back(final);
    int count = 0;
    while (stack.size() > 0)
    {
        auto current = stack.back();

        stack.pop_back();
        if (current.row >= nRows || current.col >= nCols)
        {
            continue;
        }
        if (data[nRows - current.row - 1][current.col] == '#')
        {
            continue;
        }
        if (visited[current.getIdx(nCols, nRows)] != current.cost)
        {
            continue;
        }
        if (!revisited[current.getSimpleIdx(nCols)])
        {
            count++;
            revisited[current.getSimpleIdx(nCols)] = true;
            visited[current.getIdx(nCols, nRows)]  = -1;
        }
        // The inverse of part 1. Direction changes are the same in reverse.
        current.cost--;
        current.col += !current.dir;
        current.row += current.dir;
        stack.push_back(current);
        current.col -= !current.dir + !current.dir;
        current.row -= current.dir + current.dir;
        stack.push_back(current);

        current.col  += !current.dir;
        current.row  += current.dir;
        current.cost -= 999;
        current.dir   = !current.dir;
        stack.push_back(current);
    }
    return count;
}
} // namespace day16
int main()
{
    const auto [data, nCols, nRows] = day16::read_file("./input");
    auto [p1, visited, final]       = day16::part1(data, nCols, nRows);
    int p2 = day16::part2(data, nCols, nRows, std::move(visited), final);
    std::cout << "Part 1: " << p1 << "\nPart 2: " << p2 << '\n';
}