#include <filesystem>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
typedef unsigned long secret_t;
constexpr secret_t secret_mod_const = 16777216;
inline secret_t next_secret(secret_t secret)
{
    secret = ((secret << 6) ^ secret) % secret_mod_const;
    secret = ((secret >> 5) ^ secret) % secret_mod_const;
    return ((secret << 11) ^ secret) % secret_mod_const;
}

std::vector<secret_t> read_file(const std::filesystem::path &input_filepath)
{
    std::ifstream file{input_filepath};
    std::vector<secret_t> secrets{};
    secret_t secret;
    while (file >> secret)
    {
        secrets.push_back(secret);
    }
    return secrets;
}

int main()
{
    std::vector<secret_t> secrets = read_file("./input");

    // Map instructions to prices. Each instruction is -9 to +9, mapped to 0 to
    // +18. It can then be treated as a base 19 number, giving a unique idx for
    // each set of instructions
    std::vector<unsigned int> price_map{};
    std::vector<unsigned int> seen_instructions{};
    price_map.resize(19 * 19 * 19 * 19, 0);
    seen_instructions.resize(19 * 19 * 19 * 19, 0);

    unsigned long part1 = 0;
    unsigned int part2  = 0;

    // To check if an instruction has been seen before in this secret, check
    // seen_instructions[instruction] == secret_idx
    size_t secret_idx = 0;
    for (auto secret : secrets)
    {
        secret_idx++;
        unsigned int previous_digit     = secret % 10;
        unsigned int trade_instructions = 0;

        for (int i = 0; i < 3; i++)
        {
            secret              = next_secret(secret);
            unsigned int digit  = secret % 10;
            trade_instructions *= 19;
            trade_instructions += digit - previous_digit + 9;
            previous_digit      = digit;
        }
        for (int i = 3; i < 2000; i++)
        {
            secret              = next_secret(secret);
            unsigned int digit  = secret % 10;
            trade_instructions *= 19;
            trade_instructions += digit - previous_digit + 9;
            trade_instructions %= 19 * 19 * 19 * 19;
            previous_digit      = digit;
            if (seen_instructions[trade_instructions] == secret_idx)
                continue;
            seen_instructions[trade_instructions]  = secret_idx;
            price_map[trade_instructions]         += digit;
        }
        part1 += secret;
    }
    for (auto val : price_map)
    {
        part2 = std::max(part2, val);
    }
    std::cout << part1 << std::endl;
    std::cout << part2 << std::endl;
}