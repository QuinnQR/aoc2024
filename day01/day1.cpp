#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>

const std::filesystem::path filePath = "./input";
int main()
{
    std::ifstream inputStream{filePath};
    std::vector<int> firstList, secondList, secondListCount;
    firstList.reserve(1'000);
    secondList.reserve(1'000);
    secondListCount.resize(100'000);
    // Vectors take about 400 KB of memory

    // Parsing input and filling vectors
    int a, b;
    while (inputStream >> a >> b)
    {
        firstList.push_back(a);
        secondList.push_back(b);
        secondListCount[b]++;
    }
    std::sort(firstList.begin(), firstList.end());
    std::sort(secondList.begin(), secondList.end());

    // Calculating both part 1 and part 2
    int distance = 0;
    int similarity = 0;
    for (int i = 0; i < firstList.size(); i++)
    {
        distance += std::abs(firstList[i] - secondList[i]);
        similarity += firstList[i] * secondListCount[firstList[i]];
    }

    std::cout << "Distance between list 1 and list 2: " << distance << "\n";
    std::cout << "Similarity between list 1 and list 2: " << similarity << "\n";
}