#include <array>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
class StateMachine
{
  public:
    StateMachine(std::string p_instructions, int pA, int pB, int pC)
        : instructions(p_instructions), A0(pA), B0(pB), C0(pC), pointer(0) {};

    void run()
    {
        output.clear();
        // For part 2, we want to vary A so don't reset it here in that case.
        if (!debug_mode)
            A = A0;
        B       = B0;
        C       = C0;
        pointer = 0;
        while (pointer < instructions.size())
            step();
    }
    long long debug()
    {
        debug_mode    = true;
        long long res = recurse(instructions.size() - 1, 0);
        debug_mode    = false;
        return res;
    }
    const std::string &get_output() { return output; }

  private:
    const std::string instructions;
    std::string output;
    size_t pointer;
    long long A, B, C;
    const long long A0, B0, C0;
    bool debug_mode = false;
    long long recurse(int reverse_ptr, long long accumulator)
    {
        if (reverse_ptr < 0)
            return accumulator;
        accumulator      <<= 3;
        char target_char   = instructions[reverse_ptr];

        // Technically, the first guess can't be a 0. Unlikely to occur,
        // but checked here in case.
        int guess_for_A = (reverse_ptr == instructions.size() - 1) ? 1 : 0;
        for (; guess_for_A < 8; guess_for_A++)
        {
            // Set the last 3 bits to be the guess and run. If it gives the
            // correct character, then move onto the next character. Otherwise,
            // next guess.
            accumulator += guess_for_A;
            A            = accumulator;
            run();
            if (output.front() == target_char)
            {
                std::cout << output.front() << "==" << target_char << "\n";
                long long res = recurse(reverse_ptr - 2, accumulator);
                if (res != -1)
                    // If res is -1, then this guess didn't lead to a solution
                    // and we are backtracking. So we should just continue
                    // guessing. Otherwise, keep returning to the top.
                    return res;
            }
            accumulator -= guess_for_A;
        }
        // No guess worked, return error value and backtrack.
        return -1;
    };
    void step()
    {
        // Carries out the next instruction.
        if (pointer + 2 > instructions.size())
        {
            // Avoids potential edgecase in part 2 if a non valid program is
            // checked s.t. the pointer gets stuck with an opcode but no operand
            // Having now done part 2, I don't think that's possible.
            pointer = instructions.size();
            return;
        }
        char opcode       = instructions[pointer];
        char raw_operand  = instructions[pointer + 2];
        pointer          += 4;
        long long operand;
        switch (opcode)
        {
        case '0':
            operand = get_combo_operand(raw_operand);
            A       = A >> operand;
            return;
        case '1':
            B = (B & 0b111) ^ raw_operand - '0';
            return;
        case '2':
            B = get_combo_operand(raw_operand) & 0b111;
            return;
        case '3':
            pointer = A ? raw_operand - '0' : pointer;
            return;
        case '4':
            B ^= C;
            return;
        case '5':
            if (output.size() > 0)
                output.push_back(',');
            output.push_back('0' + (get_combo_operand(raw_operand) & 0b111));
            return;
        case '6':
            operand = get_combo_operand(raw_operand);
            B       = A >> operand;
            return;
        case '7':
            operand = get_combo_operand(raw_operand);
            C       = A >> operand;
            return;
        };
    }

    long long get_combo_operand(char raw_operand)
    {
        // Returns the combo operand value given the character representation.
        // Just assume '7' will never get here. If it does, 7 will be returned.
        switch (raw_operand)
        {
        case '4':
            return A;
        case '5':
            return B;
        case '6':
            return C;
        default:
            return raw_operand - '0';
        }
    }
};
StateMachine read_file(const std::filesystem::path &path)
{
    std::ifstream file(path);
    char ch;
    int A, B, C;
    std::string instructions = "ERR";
    while ((file >> ch) && (ch != ':'))
        ;
    file >> A;
    while ((file >> ch) && (ch != ':'))
        ;
    file >> B;
    while ((file >> ch) && (ch != ':'))
        ;
    file >> C;
    while ((file >> ch) && (ch != ':'))
        ;
    // If noskipws isn't used, then file>>ch will absorb a useful character as
    // well as whitespace
    file >> std::noskipws;
    file >> ch;
    std::getline(file, instructions);
    return {std::move(instructions), A, B, C};
}
int main(int argc, char **argv)
{
    std::filesystem::path path = "./input";
    if (argc >= 2)
    {
        path = argv[1];
    }
    StateMachine machine = read_file(path);
    machine.run();
    std::cout << "Part 1: " << machine.get_output() << std::endl;

    // I'm not sure this is the intended solution to part 2. I went through the
    // "assembly" by hand and noticed that the last 3 bits of A are deleted each
    // iteration, and so each character corresponds to 3 bits and the bits
    // before. This means we can work backwards adding 3 bits at a time.

    // This is quite dependent on the input, so I'm not sure if this behaviour
    // is guaranteed or not. Perhaps there is a more general solution where it
    // adds n bits at a time?
    std::cout << "Part 2: " << machine.debug() << std::endl;
}