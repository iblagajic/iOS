#include <iostream>
#include <string>
#include <vector>
#include <iterator>

class BloomFilter {
private:
    unsigned int hashRounds;
    std::vector<bool> bloomVector;
public:
    BloomFilter(unsigned int maxItems, double targetProbability);
    BloomFilter(std::string importFilePath, unsigned int maxItems);
    void add(std::string element);
    bool contains(std::string element);
    void writeToFile(std::string exportFilePath);
};
