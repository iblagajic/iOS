#include <iostream>
#include <string>
#include <vector>
#include <iterator>

// Port of https://github.com/cry/jsbloom
using HashFunction = std::function<unsigned int(std::string)>;


class BloomFilter {
private:
    std::vector<bool> bloomVector;
    std::vector<HashFunction> hashFunctions;
    void configureHashFunctions();
    std::vector<bool> importFromFile(std::string path);

public:
    BloomFilter();
    BloomFilter(std::string importFilePath);
    void add(std::string element);
    bool contains(std::string element);
    void exportToFile(std::string exportFilePath);
};
