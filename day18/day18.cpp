#include <array>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <utility>
#include <vector>
std::vector<std::pair<size_t, size_t>> read_file(
            const std::filesystem::path &path)
{
    std::vector<std::pair<size_t, size_t>> coordinates;
    coordinates.reserve(4000);
    std::ifstream file{path};
    size_t a, b;
    char _;
    while (file >> a)
    {
        file >> _;
        file >> b;
        coordinates.emplace_back(a, b);
    }
    return coordinates;
}

typedef std::pair<int8_t, int8_t> pos_t;
struct Node
{
    pos_t pos;
    uint16_t cost;
    Node(pos_t p_pos, uint16_t p_cost) : pos(p_pos), cost(p_cost) {};
};
inline pos_t operator+(const pos_t &lhs, const pos_t &rhs)
{
    return {lhs.first + rhs.first, lhs.second + rhs.second};
}
std::array<pos_t, 4> offsets = {
            pos_t{-1, 0 },
            pos_t{1,  0 },
            pos_t{0,  1 },
            pos_t{0,  -1}
};
class Context
{
  public:
    Context(const std::filesystem::path &p_path, size_t p_grid_height,
            size_t p_grid_width)
        : grid_height(p_grid_height), grid_width(p_grid_width)
    {
        coordinates = read_file(p_path);
    }
    int part1(int bytes_fallen)
    {
        std::vector<std::vector<int8_t>> grid(
                    grid_height, std::vector<int8_t>(grid_width, 1));
        for (int i = 0; i < coordinates.size() && i < bytes_fallen; i++)
        {
            grid[coordinates[i].second][coordinates[i].first] = 0;
        }
        return get_cost(grid);
    }
    std::pair<int, int> part2(int min_bytes)
    {
        std::vector<std::vector<int8_t>> grid(
                    grid_height, std::vector<int8_t>(grid_width, 1));
        for (int i = 0; i < coordinates.size() && i < min_bytes - 1; i++)
        {
            grid[coordinates[i].second][coordinates[i].first] = 0;
        }
        for (int i = min_bytes - 1; i < coordinates.size(); i++)
        {
            grid[coordinates[i].second][coordinates[i].first] = 0;
            if (get_cost(grid) == -1)
                return coordinates[i];
        }
    }

  private:
    int get_cost(const std::vector<std::vector<int8_t>> &const_grid)
    {
        auto grid          = const_grid;
        size_t queue_front = 0;
        std::vector<Node> node_queue{
                    {{0, 0}, 0}
        };
        pos_t target_pos = {grid_width - 1, grid_height - 1};
        while (queue_front < node_queue.size()
               && node_queue[queue_front].pos != target_pos)
        {
            auto [pos, cost] = node_queue[queue_front];
            queue_front++;
            for (auto offset : offsets)
            {
                pos_t new_pos = pos + offset;
                if (in_bounds(new_pos)
                    && grid[new_pos.second][new_pos.first] == 1)
                {
                    grid[new_pos.second][new_pos.first] = 0;
                    node_queue.emplace_back(new_pos, cost + 1);
                }
            }
        }
        return queue_front < node_queue.size() ? node_queue[queue_front].cost
                                               : -1;
    }
    inline bool in_bounds(pos_t pos)
    {
        return 0 <= pos.first && 0 <= pos.second && pos.first < grid_width
               && pos.second < grid_height;
    }
    std::vector<std::pair<size_t, size_t>> coordinates;
    const size_t grid_height, grid_width;
};
std::ostream &operator<<(std::ostream &os, const std::pair<int, int> &pair)
{
    return os << pair.first << "," << pair.second;
}
int main()
{
    Context context{"./input", 71, 71};
    std::cout << context.part1(1024) << "\n";
    std::cout << context.part2(1025) << "\n";
}
// Unoptimized way
// Time to beat: 27ms