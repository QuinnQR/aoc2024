#include <array>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

struct Trie
{
    std::array<std::unique_ptr<Trie>, 26> nodes;
    bool leaf = false;
    void add(const std::string_view text)
    {
        if (text.size() == 0)
        {
            leaf = true;
            return;
        }
        if (!nodes[text[0] - 'a'])
            nodes[text[0] - 'a'] = std::make_unique<Trie>();
        nodes[text[0] - 'a']->add(text.substr(1));
    }
};
struct Input
{
    Trie patterns;
    std::vector<std::string> designs;
    size_t max_length = 0;
};

Input read_file(const std::filesystem::path &path)
{
    Input input;
    std::string pattern;
    std::string line;
    std::ifstream file{path};
    std::getline(file, line);
    for (const auto ch : line)
    {
        if (ch == ' ')
            continue;
        if (ch == ',')
        {
            input.patterns.add(pattern);
            pattern = "";
            continue;
        }
        pattern += ch;
    }
    input.patterns.add(pattern);
    while (std::getline(file, line))
    {
        if (line.size() > 0)
        {
            input.max_length = std::max(input.max_length, line.size());
            input.designs.push_back(std::move(line));
        }
    }
    return input;
}
typedef long long num_t;
struct PatternCache
{
    PatternCache(size_t reserve_size) { cache.reserve(reserve_size); }
    num_t count_patterns_cached(std::string::const_iterator start,
                                std::string::const_iterator end,
                                const Trie *const patterns)
    {
        cache.assign(end - start + 1, -1);
        return count_patterns(start, end, patterns);
    }

  private:
    std::vector<num_t> cache;
    num_t count_patterns(std::string::const_iterator start,
                         std::string::const_iterator end,
                         const Trie *const patterns)
    {
        size_t diff = end - start;
        if (cache[diff] != -1)
            return cache[diff];
        const Trie *current_trie = patterns;
        num_t count              = 0;
        if (start == end)
            return 0;
        while (current_trie && start != end)
        {
            if (current_trie->leaf)
                count += count_patterns(start, end, patterns);
            current_trie = current_trie->nodes[*start - 'a'].get();
            start++;
        }
        if (current_trie && current_trie->leaf)
            count++;
        cache[diff] = count;
        return count;
    }
};

std::pair<num_t, num_t> get_results(const Input &input)
{
    num_t result1 = 0;
    num_t result2 = 0;
    PatternCache cache(input.max_length);
    for (const auto &design : input.designs)
    {
        num_t count  = cache.count_patterns_cached(design.begin(), design.end(),
                                                   &input.patterns);
        result2     += count;
        result1     += count != 0;
    }
    return {result1, result2};
}
int main()
{
    Input input         = read_file("./input");
    auto [part1, part2] = get_results(input);
    std::cout << "Part 1: " << part1 << "\n";
    std::cout << "Part 2: " << part2 << "\n";
}