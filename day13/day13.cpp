#include <fstream>
#include <iostream>
#include <sstream>
#include <utility>
typedef long long num_t;
inline int det(num_t a, num_t b, num_t c, num_t d) { return a * d - b * c; }
std::pair<num_t, num_t> getCollinearPresses(num_t aX, num_t aY, num_t bX,
                                            num_t bY, num_t X, num_t Y)
{
    // All of my inputs form a basis. I don't know if that's guaranteed
    // or if I just got lucky. The logic of this function was horrific
    // when I tried implementing it.
    std::cerr << "Not implemented" << std::endl;
    exit(0);
}
std::pair<num_t, num_t> getPresses(num_t aX, num_t aY, num_t bX, num_t bY,
                                   num_t X, num_t Y)
{
    num_t determinant = det(aX, bX, aY, bY);
    if (determinant == 0)
    {
        // a and b are collinear - turns out this doesn't happen?
        return getCollinearPresses(aX, aY, bX, bY, X, Y);
    }
    num_t aCount = (bY * X - bX * Y) / determinant;
    num_t bCount = (aX * Y - aY * X) / determinant;
    if (aCount * aX + bCount * bX == X && aCount * aY + bCount * bY == Y)
        return {aCount, bCount};
    else
    {
        return {0, 0};
    }
}
inline num_t getCost(const std::pair<num_t, num_t> presses)
{
    if (presses.first < 0 || presses.second < 0)
        return 0;
    return 3 * presses.first + presses.second;
}
std::pair<int, int> parseLine(const std::string &line, int numJunkChars1,
                              int numJunkChars2)
{
    // Gets two ints from the line, with numJunkChars preceding.
    int a, b;
    std::stringstream stream{line};
    stream.ignore(numJunkChars1);
    stream >> a;
    stream.ignore(numJunkChars2);
    stream >> b;
    return {a, b};
}
int main()
{
    num_t result1 = 0;
    num_t result2 = 0;
    num_t offset  = 10'000'000'000'000;
    std::ifstream file{"./input"};
    std::string line;
    while (std::getline(file, line))
    {
        auto [aX, aY] = parseLine(line, 11, 3);
        std::getline(file, line);
        auto [bX, bY] = parseLine(line, 11, 3);
        std::getline(file, line);
        auto [X, Y] = parseLine(line, 9, 4);
        std::getline(file, line);
        result1 += getCost(getPresses(aX, aY, bX, bY, X, Y));
        result2 += getCost(getPresses(aX, aY, bX, bY, X + offset, Y + offset));
    }
    std::cout << "Part 1: " << result1 << "\n";
    std::cout << "Part 2: " << result2 << "\n";
}