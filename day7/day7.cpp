#include <filesystem>
#include <fstream>
#include <future>
#include <iostream>
#include <sstream>
#include <vector>
typedef long long num_t;
typedef std::vector<num_t>::iterator it_t;

// Tried using an array of size 16 to keep it on the stack, but didn't make
// enough difference for the hassle.
struct Equation
{
    num_t target;
    std::vector<num_t> operands;
    Equation() { operands.reserve(16); }
};

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
    // foundConcat includes above and concat.
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
    // Moving next, end out of args and into context made it a little faster.
    // Probably not actually worth the decrease in readability, honestly.
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
std::pair<num_t, num_t> recurseBatch(std::vector<Equation> equations)
{
    num_t result1 = 0;
    num_t result2 = 0;
    Context context;
    for (auto eq : equations)
    {
        context.target = eq.target;
        context.next = eq.operands.begin() + 1;
        context.end = eq.operands.end();
        ReturnType ret = context.recurse(eq.operands.front());
        if (ret.foundNoConcat)
        {
            result1 += eq.target;
            result2 += eq.target;
        }
        else if (ret.foundConcat)
        {
            result2 += eq.target;
        }
    }
    return {result1, result2};
}
int main(int argc, char **argv)
{
    std::filesystem::path filePath = "./input";
    if (argc > 1)
    {
        filePath = argv[1];
        std::cout << "Reading file " << filePath << "\n";
    }
    int batchSize = 70;
    if (argc > 2)
    {
        batchSize = std::atoi(argv[2]);
        std::cout << "Using batchSize: " << batchSize << "\n";
    }

    std::ifstream file{filePath};
    std::string line;
    std::vector<Equation> batch;
    batch.reserve(batchSize);
    std::vector<std::future<std::pair<num_t, num_t>>> returnVector;
    while (std::getline(file, line))
    {
        batch.emplace_back();
        std::istringstream lineStream(line);
        lineStream >> batch.back().target;
        lineStream.ignore();
        for (num_t i; lineStream >> i;)
        {
            batch.back().operands.push_back(i);
        }
        if (batch.size() > batchSize)
        {
            returnVector.push_back(std::async(&recurseBatch, std::move(batch)));
            batch.clear();
            batch.reserve(batchSize);
        }
    }
    if (batch.size() > 0)
    {
        returnVector.push_back(std::async(
                    std::launch::async, &recurseBatch, std::move(batch)));
    }
    num_t result1 = 0;
    num_t result2 = 0;
    for (auto &future : returnVector)
    {
        auto [num1, num2] = future.get();
        result1 += num1;
        result2 += num2;
    }
    std::cout << "Part 1: " << result1 << "\n";
    std::cout << "Part 2: " << result2 << std::endl;
}