#include <iostream>
#include <string>
#include <vector>
#include <iterator>

// Port of https://github.com/cry/jsbloom
using HashFunction = std::function<unsigned int(std::string)>;


class BloomFilter {
private:
    unsigned int size;
    std::vector<bool> bloomVector;
    std::vector<HashFunction> hashFunctions;
    void configureHashFunctions();

public:
    BloomFilter();
    BloomFilter(unsigned int size, std::vector<HashFunction> funcs);
    BloomFilter(std::string importFilePath);
    void add(std::string element);
    bool contains(std::string element);
    void exportToFile(std::string exportFilePath);
};
