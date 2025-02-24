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
enum
{
    EAST  = 0,
    NORTH = 1,
    WEST  = 2,
    SOUTH = 3
};
int8_t row_offsets[4] = {0, -1, 0, 1};
int8_t col_offsets[4] = {1, 0, -1, 0};

struct Position
{
    uint32_t cost;
    uint8_t col;
    uint8_t row;
    uint8_t dir;
    uint8_t pad = 0;
    Position(uint32_t p_cost, uint8_t p_col, uint8_t p_row, uint8_t p_dir)
    {
        cost = p_cost;
        col  = p_col;
        row  = p_row;
        dir  = p_dir;
    }
    // Probably undefined behaviour? I think most things that rely on the layout
    // of structs or unions are.
    uint32_t getHashable() { return *(uint32_t *)&col; }
    uint16_t getSimpleCoords() { return *(uint16_t *)&col; }
};

// Compare function to allow for a min heap of Positions
struct PositionCompare
{
    bool operator()(const Position &lhs, const Position &rhs)
    {
        return lhs.cost > rhs.cost;
    };
};

std::tuple<std::vector<std::string>, int, int> read_file(
            const std::filesystem::path &path)
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

// Returns the part1 result, the map of positions to their cost, and the final
// position achieved.
std::tuple<int, std::unordered_map<uint32_t, uint32_t>, Position> part1(
            const std::vector<std::string> &data, int nCols, int nRows)
{
    std::priority_queue<Position, std::vector<Position>, PositionCompare> heap;
    heap.emplace(0, 1, 1, EAST);
    std::unordered_map<uint32_t, uint32_t> visited;

    while (heap.size() > 0)
    {
        auto current = heap.top();
        heap.pop();

        if (current.row >= nRows || current.col >= nCols)
            continue;
        if (data[nRows - current.row - 1][current.col] == '#')
            continue;
        if (visited.find(current.getHashable()) != visited.end())
            continue;

        // Map the current position and direction to the cost to reach. This is
        // the minimum cost possible.
        visited.insert({current.getHashable(), current.cost});
        if (data[nRows - current.row - 1][current.col] == 'E')
            return {current.cost, visited, current};

        current.cost++;
        current.col += col_offsets[current.dir];
        current.row += row_offsets[current.dir];
        heap.push(current);
        current.col  -= col_offsets[current.dir];
        current.row  -= row_offsets[current.dir];
        current.cost += 999;
        current.dir  += 1;
        current.dir  %= 4;
        heap.push(current);
        current.dir += 2;
        current.dir %= 4;
        heap.push(current);
    }
    return {0, std::unordered_map<uint32_t, uint32_t>(), Position(0, 0, 0, 0)};
}
int part2(const std::vector<std::string> &data, int nCols, int nRows,
          std::unordered_map<uint32_t, uint32_t> visited, Position final)
{
    // Part 2 works by doing a DFS search backwards from the end.
    // If the cost disagrees with that from part 1, we have strayed from an
    // optimal path so the node can be discarded.
    std::unordered_set<uint16_t> revisited;
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
        auto it = visited.find(current.getHashable());
        if (it == visited.end() || it->second != current.cost)
        {
            continue;
        }
        if (revisited.find(current.getSimpleCoords()) == revisited.end())
        {
            count++;
            revisited.insert(current.getSimpleCoords());
            it->second = UINT32_MAX;
            // An easy way to skip this being checked again. If the cost of a
            // node was UINT32_MAX before this, something has gone very wrong
        }
        // The inverse of part 1. Direction changes are the same in reverse.
        current.cost--;
        current.col -= col_offsets[current.dir];
        current.row -= row_offsets[current.dir];
        stack.push_back(current);
        current.col  += col_offsets[current.dir];
        current.row  += row_offsets[current.dir];
        current.cost -= 999;
        current.dir  += 1;
        current.dir  %= 4;
        stack.push_back(current);
        current.dir += 2;
        current.dir %= 4;
        stack.push_back(current);
    }
    return count;
}
} // namespace day16
int main()
{
    const auto [data, nCols, nRows] = day16::read_file("./input");
    auto [p1, visited, final]       = day16::part1(data, nCols, nRows);
    int p2 = day16::part2(data, nCols, nRows, visited, final);
    std::cout << "Part 1: " << p1 << "\nPart 2: " << p2 << '\n';
}