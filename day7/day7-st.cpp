#include <filesystem>
#include <fstream>
#include <future>
#include <iostream>
#include <sstream>
#include <vector>
typedef long long num_t;
typedef std::vector<num_t>::iterator it_t;

num_t concatenate(num_t a, num_t b)
{
    num_t ten = 1;
    while (ten <= b)
    {
        ten *= 10;
    }
    return a * ten + b;
}

struct ReturnType
{
    // foundNoConcat is found only using + and *.
    // foundConcat includes above and concatenation.
    bool foundNoConcat;
    bool foundConcat;
    ReturnType(bool a, bool b)
    {
        foundNoConcat = a;
        foundConcat = b;
    }
    ReturnType(bool ab)
    {
        foundNoConcat = ab;
        foundConcat = ab;
    }
};
struct Context
{
    num_t target;
    it_t next;
    it_t end;
    ReturnType recurse(num_t acc)
    {
        if (next == end)
            return {(target == acc)};
        num_t calculation;
        bool foundConcat = false;
        if ((calculation = acc + *next) <= target)
        {
            next++;
            ReturnType found = recurse(calculation);
            next--;
            if (found.foundNoConcat)
                return found;
            foundConcat |= found.foundConcat;
        }
        if ((calculation = acc * *next) <= target)
        {
            next++;
            ReturnType found = recurse(calculation);
            next--;
            if (found.foundNoConcat)
                return found;
            foundConcat |= found.foundConcat;
        }
        if ((calculation = concatenate(acc, *next)) <= target)
        {
            next++;
            ReturnType found = recurse(calculation);
            next--;
            foundConcat |= found.foundConcat;
        }
        return {false, foundConcat};
        // Can only return early if the result doesn't include concatenation.
        // Otherwise, we may only add to part 2 when there was another method
        // that would make it valid for part 1 as well.
    }
};
int main(int argc, char **argv)
{
    std::filesystem::path filePath = "./input";
    if (argc > 1)
    {
        filePath = argv[1];
        std::cout << "Reading file " << filePath << "\n";
    }
    num_t result1 = 0;
    num_t result2 = 0;
    std::ifstream file{filePath};
    std::string line;
    Context context;
    std::vector<num_t> operands;
    operands.reserve(16);
    while (std::getline(file, line))
    {
        std::istringstream lineStream(line);
        lineStream >> context.target;
        lineStream.ignore();
        for (num_t i; lineStream >> i;)
        {
            operands.push_back(i);
        }

        context.next = operands.begin() + 1;
        context.end = operands.end();
        auto rv = context.recurse(operands.front());
        if (rv.foundConcat)
        {
            result2 += context.target;
        }
        if (rv.foundNoConcat)
        {
            result1 += context.target;
        }
        operands.clear();
        operands.reserve(16);
    }

    std::cout << "Part 1: " << result1 << "\n";
    std::cout << "Part 2: " << result2 << std::endl;
}