#include <iostream>
#include <string>
#include <vector>
#include <iterator>

class BloomFilter {
private:
    unsigned int hashRounds;
    std::vector<bool> bloomVector;
    std::vector<bool> importFromFile(std::string path);
    std::vector<bool> importFromBase64File(std::string path);
    unsigned int calculateHashRounds(unsigned int size, unsigned int items);
    unsigned int djb2Hash(std::string text);
    unsigned int sdbmHash(std::string text);
    unsigned int doubleHash(unsigned int hash1, unsigned int hash2, unsigned int round);

public:
    BloomFilter(unsigned int itemCount, double targetProbability);
    BloomFilter(std::string importFilePath);
    void add(std::string element);
    bool contains(std::string element);
    void exportToFile(std::string exportFilePath);
};
