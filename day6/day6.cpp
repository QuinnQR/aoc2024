#include <algorithm>
#include <array>
#include <chrono>
#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_set>
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
    inline V2 operator*(const int &other) { return V2(x * other, y * other); }
};

template <typename T> struct MDVector
{
  public:
    std::vector<T> underlying;
    int length;
    int height;
    T &operator[](V2 idx) { return underlying[idx.x + length * idx.y]; }
    std::stringstream strStream()
    {
        std::stringstream stream;
        for (int row = 0; row < height; row++)
        {
            for (int col = 0; col < length; col++)
            {
                stream << (*this)[{col, row}] << '\t';
            }
            stream << '\n';
        }
        return stream;
    }
};
void addLine(
            const std::string &line, std::vector<int> &board, int rowNo,
            V2 &startPos)
{
    int colNo = 0;
    for (auto ch : line)
    {
        if (ch == '^')
        {
            startPos = {colNo, rowNo};
        }
        if (ch == '#')
        {
            board.push_back(-1);
        }
        else
        {
            board.push_back(0);
        }
        colNo++;
    }
};

inline bool inBounds(V2 pos, int length, int height)
{
    return (pos.x >= 0) && (pos.y >= 0) && (pos.x < length) && (pos.y < height);
}
// clang-format off
V2 directions[4] = {
            { 0, -1},
            { 1,  0},
            { 0,  1},
            {-1,  0}
};
// clang-format on
void slide(MDVector<int> &precomputeVector, V2 first, V2 nextDir, V2 slideDir)
{
    while (inBounds(first, precomputeVector.length, precomputeVector.height))
    {
        V2 pos = first;
        int val = 1;
        while (inBounds(pos, precomputeVector.length, precomputeVector.height))
        {
            if (precomputeVector[pos] == -1)
                val = -1;
            precomputeVector[pos] = val;
            pos = pos + slideDir;
            val++;
        }
        first = first + nextDir;
    }
}
void slideRowToBlock(MDVector<int> &precomputeVector, V2 first, V2 slideDir)
{

    V2 pos = first + slideDir;
    int val = precomputeVector[first] + 1;
    while (inBounds(pos, precomputeVector.length, precomputeVector.height))
    {
        if (precomputeVector[pos] == -1)
            return;
        precomputeVector[pos] = val;
        pos = pos + slideDir;
        val++;
    }
}
void updateCoord(
            MDVector<int> *precomputeVectors, V2 coord, int height, int length)
{
    slideRowToBlock(precomputeVectors[0], {coord.x, coord.y}, directions[2]);
    slideRowToBlock(precomputeVectors[1], {coord.x, coord.y}, directions[3]);
    slideRowToBlock(precomputeVectors[2], {coord.x, coord.y}, directions[0]);
    slideRowToBlock(precomputeVectors[3], {coord.x, coord.y}, directions[1]);
}
std::vector<V2> getGuardPath(V2 start, MDVector<int> &board)
{
    std::vector<V2> path;
    int dir = 0;
    V2 pos = start;
    while (inBounds(pos, board.length, board.height))
    {
        if (board[pos] == 0)
        {
            path.push_back(pos);
            board[pos] = 1;
        }
        if (board[pos] == -1)
        {
            pos = pos - directions[dir];
            dir = (dir + 1) % 4;
        }
        pos = pos + directions[dir];
    }
    return path;
}
struct Obstacle
{
    int16_t x;
    int16_t y;
    int16_t d;
    int16_t z = 0;
};
union ObstacleHash
{
    Obstacle obs;
    int64_t hash;
};
// std::unordered_set<int64_t> obstacleSet;
std::vector<int64_t> obstacleVec;
bool checkLoop(MDVector<int> *precomputeVectors, V2 start)
{
    int dir = 0;
    obstacleVec.clear();
    while (true)
    {
        start = start + directions[dir] * precomputeVectors[dir][start];
        if (!inBounds(start, precomputeVectors[0].length,
                      precomputeVectors[0].height))
            return false;
        // Yeah there's warnings here. I don't care.
        int64_t hash = ObstacleHash({start.x, start.y, dir, 0}).hash;
        if (std::find(obstacleVec.begin(), obstacleVec.end(), hash) !=
            obstacleVec.end())
            return true;
        obstacleVec.push_back(hash);
        dir = (dir + 1) % 4;
    }
}
MDVector<int> precompute[4];
int main()
{
    std::ifstream file{"./input"};
    std::string line;
    MDVector<int> board;
    int rowNo = 0;
    V2 startPos = {0, 0};
    while (std::getline(file, line))
    {
        board.length = line.size();
        addLine(line, board.underlying, rowNo, startPos);
        rowNo++;
    }
    board.height = rowNo;

    precompute[0] = board;
    precompute[1] = board;
    precompute[2] = board;
    precompute[3] = board;
    slide(precompute[0], {0, 0}, {1, 0}, directions[2]);
    slide(precompute[1], {board.length - 1, 0}, {0, 1}, directions[3]);
    slide(precompute[2], {0, board.height - 1}, {1, 0}, directions[0]);
    slide(precompute[3], {0, 0}, {0, 1}, directions[1]);
    std::vector<V2> path = getGuardPath(startPos, board);
    int result1 = path.size();
    int result2 = 0;

    int temp[4];
    for (int sqIdx = 1; sqIdx < path.size(); sqIdx++)
    {
        for (int d = 0; d < 4; d++)
        {
            temp[d] = precompute[d][path[sqIdx]];
            precompute[d][path[sqIdx]] = -1;
        }
        updateCoord(precompute, path[sqIdx], board.height, board.length);
        result2 += checkLoop(precompute, startPos);
        for (int d = 0; d < 4; d++)
        {
            precompute[d][path[sqIdx]] = temp[d];
        }
        updateCoord(precompute, path[sqIdx], board.height, board.length);
    }
    std::cout << "Part 1:" << result1 << '\n';
    std::cout << "Part 2:" << result2 << '\n';
}