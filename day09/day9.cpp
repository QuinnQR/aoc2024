#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

typedef long long num_t;
// Intervals are [start,end)
struct Interval
{
    num_t start;
    num_t end;
    Interval(num_t s, num_t e)
    {
        start = s;
        end   = e;
    }
};
struct DigitInterval
{
    num_t start;
    num_t end;
    num_t val;
    DigitInterval(num_t s, num_t e, num_t v)
    {
        start = s;
        end   = e;
        val   = v;
    }
};

inline num_t width(Interval interval) { return interval.end - interval.start; }
inline num_t width(DigitInterval interval)
{
    return interval.end - interval.start;
}
// If a file is at start and has width digits, its checksum is ID * multiplier.
inline num_t getMultiplier(num_t start, num_t width)
{
    return (start * width + (width * (width - 1)) / 2);
};
inline num_t getCheckSum(num_t start, num_t width, num_t fileID)
{
    return fileID * getMultiplier(start, width);
}
inline num_t getCheckSum(DigitInterval interval)
{
    return getCheckSum(interval.start, width(interval), interval.val);
}

num_t part1(const std::vector<DigitInterval> &digitIntervals,
            const std::vector<Interval> &spaceIntervals, int digitCount)
{
    auto spaceIntervalIt        = spaceIntervals.begin();
    auto digitIntervalIt        = digitIntervals.rbegin();
    Interval spaceInterval      = *spaceIntervalIt;
    DigitInterval digitInterval = *digitIntervalIt;

    num_t resultPart1 = 0;
    // Iterate through spaces that need to be filled and fill with highest
    // available digit. Stop when data is contiguous.
    while (spaceInterval.start < digitCount)
    {
        num_t spaceWidth = width(spaceInterval);
        num_t digitWidth = width(digitInterval);
        num_t minWidth   = std::min(spaceWidth, digitWidth);

        // Add the checkSum for the moved region
        resultPart1         += getCheckSum(spaceInterval.start, minWidth,
                                           digitInterval.val);
        spaceInterval.start += minWidth;
        digitInterval.end   -= minWidth;
        if (spaceWidth == minWidth)
        {
            // The space interval is now full, move to next region
            spaceIntervalIt++;
            spaceInterval = *spaceIntervalIt;
        }
        if (digitWidth == minWidth)
        {
            // The digit interval has all been moved, move to next digit
            digitIntervalIt++;
            digitInterval = *digitIntervalIt;
        }
    }
    resultPart1 += getCheckSum(digitInterval);
    // Add the checksum of all unmoved regions
    for (digitIntervalIt++; digitIntervalIt != digitIntervals.rend();
         digitIntervalIt++)
        resultPart1 += getCheckSum(*digitIntervalIt);

    return resultPart1;
}

num_t part2(const std::vector<DigitInterval> &digitIntervals,
            std::vector<Interval> &spaceIntervals)
{
    num_t resultPart2 = 0;
    // Add the checksum for all regions first. Moved regions will be subtracted
    for (auto digitInterval : digitIntervals)
        resultPart2 += getCheckSum(digitInterval);

    // Maintain a cache so early spaces can be skipped as they are likely full.
    // This cache goes up to wherever
    num_t maxWidthInCache = 0;
    int cachedSpaceIdx    = 0;
    for (int digitIdx = digitIntervals.size() - 1; digitIdx > 0; digitIdx--)
    {
        num_t digitWidth = width(digitIntervals[digitIdx]);
        // Skip the spaces that were too small for the last file
        int spaceIdx = cachedSpaceIdx;
        if (maxWidthInCache >= digitWidth)
        {
            // The current file is smaller than the largest skipped space.
            // Restart from zero
            spaceIdx        = 0;
            maxWidthInCache = 0;
        }
        for (; spaceIdx < digitIdx; spaceIdx++)
        {
            num_t spaceWidth = width(spaceIntervals[spaceIdx]);
            if (spaceWidth >= digitWidth)
            {
                // The space is large enough; move file to spaceIdx.
                resultPart2 += getCheckSum(spaceIntervals[spaceIdx].start,
                                           digitWidth, digitIdx);
                resultPart2 -= getCheckSum(digitIntervals[digitIdx].start,
                                           digitWidth, digitIdx);
                spaceIntervals[spaceIdx].start += digitWidth;
                maxWidthInCache                 = std::max(maxWidthInCache,
                                                           spaceWidth - digitWidth);
                break;
            }
            maxWidthInCache = std::max(maxWidthInCache, spaceWidth);
        }
        cachedSpaceIdx = spaceIdx;
    }

    return resultPart2;
}
inline std::filesystem::path getInputFilename(int argc, char **argv)
{
    return (argc > 1) ? argv[0] : "./input";
}
std::string readInput(const std::filesystem::path &fileName)
{
    std::ifstream file{fileName};
    std::string line;
    getline(file, line);
    return line;
}
int main(int argc, char **argv)
{
    auto text             = readInput(getInputFilename(argc, argv));
    num_t totalDigitCount = text[0] - '0';
    num_t currentIdx      = text[0] - '0';
    std::vector<DigitInterval> digitIntervals;
    std::vector<Interval> spaceIntervals;
    digitIntervals.reserve((text.size() + 1) / 2);
    spaceIntervals.reserve(text.size() / 2);
    digitIntervals.push_back({0, text[0] - '0', 0});
    for (num_t i = 1; i < text.size() - 1; i += 2)
    {
        spaceIntervals.emplace_back(currentIdx, currentIdx + text[i] - '0');
        currentIdx += text[i] - '0';
        digitIntervals.emplace_back(currentIdx, currentIdx + text[i + 1] - '0',
                                    (i + 1) / 2);
        currentIdx      += text[i + 1] - '0';
        totalDigitCount += text[i + 1] - '0';
    }
    num_t resultPart1 = part1(digitIntervals, spaceIntervals, totalDigitCount);
    num_t resultPart2 = part2(digitIntervals, spaceIntervals);
    std::cout << "Part 1: " << resultPart1 << '\n';
    std::cout << "Part 2: " << resultPart2 << '\n';
}