#include <iostream>
#include <string>
#include <vector>
#include <iterator>

// Based on sample at https://codereview.stackexchange.com/questions/179135/bloom-filter-implementation-in-c
using HashFunction = std::function<unsigned int(std::string)>;

unsigned int djb2(std::string text) {
    unsigned int hash = 5381;
    for (auto iter = text.begin(); iter != text.end(); iter++) {
        hash = ((hash << 5) + hash) + *iter;
    }
    return hash;
}

unsigned int sdbm(std::string text) {
    unsigned int hash = 0;
    
    for (auto iter = text.begin(); iter != text.end(); iter++) {
        hash = *iter + ((hash << 6) + (hash << 16) - hash);
    }
    return hash;
}

class BloomFilter {
    unsigned int size;
    std::vector<bool> cell;
    std::vector<HashFunction> hashFunctions;

public:

    BloomFilter() {
        size = 160000000;
        hashFunctions.push_back(djb2);
        hashFunctions.push_back(sdbm);
        cell = std::vector<bool>(size);
    }
    
    BloomFilter(unsigned int size, std::vector<HashFunction> funcs) : size(size), hashFunctions(funcs), cell(size)  {
    }

    void add(std::string element) {
        for (std::vector<HashFunction>::iterator iter = hashFunctions.begin(); iter != hashFunctions.end(); iter++) {
            int hash = (*iter)(element);
            int index = hash % size;
            cell[index] = true;
        }
    }
    
    bool contains(std::string element) {
        for (auto iter = hashFunctions.begin(); iter != hashFunctions.end(); iter++) {
            if (cell[(*iter)(element) % size] == false) {
                return false;
            }
        }
        return true;
    }

    ~BloomFilter() {
    }
};
