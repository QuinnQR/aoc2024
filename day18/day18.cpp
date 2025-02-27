#include <algorithm>
#include <array>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <utility>
#include <vector>
typedef std::pair<int8_t, int8_t> pos_t;
std::vector<pos_t> read_file(const std::filesystem::path &path)
{
    std::vector<pos_t> coordinates;
    coordinates.reserve(4000);
    std::ifstream file{path};
    int a, b;
    char _;
    while (file >> a)
    {
        file >> _;
        file >> b;
        coordinates.emplace_back(a, b);
    }
    return coordinates;
}

struct Node
{
    pos_t pos;
    size_t prev;
    uint32_t cost;
    Node(pos_t p_pos, size_t p_prev, uint32_t p_cost)
        : pos(p_pos), prev(p_prev), cost(p_cost) {};
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
struct ResultPart1
{
    size_t part1;
    std::vector<pos_t> path;
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
    ResultPart1 part1(int bytes_fallen)
    {
        std::vector<std::vector<int8_t>> grid(
                    grid_height, std::vector<int8_t>(grid_width, 1));
        for (int i = 0; i < coordinates.size() && i < bytes_fallen; i++)
        {
            grid[coordinates[i].second][coordinates[i].first] = 0;
        }
        auto path = get_path(grid);
        return {path.size() - 1, path};
    }
    std::pair<int, int> part2(int min_bytes, std::vector<pos_t> path)
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
            if (std::find(path.begin(), path.end(), coordinates[i])
                != path.end())
            {
                path = get_path(grid);
                if (path.size() == 0)
                    return coordinates[i];
            }
        }
        return {-1, -1};
    };

  private:
    std::vector<Node> perform_bfs(
                const std::vector<std::vector<int8_t>> &const_grid)
    {
        auto grid          = const_grid;
        size_t queue_front = 0;
        std::vector<Node> node_queue{
                    {{0, 0}, SIZE_MAX, 0}
        };
        pos_t target_pos = {grid_width - 1, grid_height - 1};
        bool end_reached = false;
        while (queue_front < node_queue.size() && !end_reached)
        {
            size_t node_idx     = queue_front;
            auto [pos, _, cost] = node_queue[queue_front];
            queue_front++;
            for (auto offset : offsets)
            {
                pos_t new_pos = pos + offset;
                if (in_bounds(new_pos)
                    && grid[new_pos.second][new_pos.first] == 1)
                {
                    grid[new_pos.second][new_pos.first] = 0;
                    node_queue.emplace_back(new_pos, node_idx, cost + 1);
                    if (new_pos == target_pos)
                    {
                        end_reached = true;
                        break;
                    }
                }
            }
        }
        return node_queue;
    }
    std::vector<pos_t> get_path(
                const std::vector<std::vector<int8_t>> &const_grid)
    {
        auto queue = perform_bfs(const_grid);
        if (queue.back().pos != pos_t{grid_width - 1, grid_height - 1})
            return {};
        size_t path_node_idx = queue.size() - 1;
        std::vector<pos_t> path{};
        while (path_node_idx != SIZE_MAX)
        {
            path.push_back(queue[path_node_idx].pos);
            path_node_idx = queue[path_node_idx].prev;
        }
        return path;
    }
    inline bool in_bounds(pos_t pos)
    {
        return 0 <= pos.first && 0 <= pos.second && pos.first < grid_width
               && pos.second < grid_height;
    }
    std::vector<pos_t> coordinates;
    const size_t grid_height, grid_width;
};
std::ostream &operator<<(std::ostream &os, const std::pair<int, int> &pair)
{
    return os << pair.first << "," << pair.second;
}
int main()
{
    Context context{"./input", 71, 71};
    ResultPart1 result_part1 = context.part1(1024);
    std::cout << result_part1.part1 << "\n";
    std::cout << context.part2(1025, result_part1.path) << "\n";
}
