#include <iostream>
#include <string>
#include <vector>
#include <iterator>
#include <stdio.h>
#include <fstream>
#include "BloomFilter.hpp"


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

BloomFilter::BloomFilter() {
    size = 160000000;
    bloomVector = std::vector<bool>(size);
    configureHashFunctions();
}

BloomFilter::BloomFilter(unsigned int size, std::vector<HashFunction> funcs) : size(size), hashFunctions(funcs), bloomVector(size)  {
}

BloomFilter::BloomFilter(std::string importFilePath) {
    size = 160000000;
    bloomVector = std::vector<bool>(size);
    configureHashFunctions();
}

void BloomFilter::configureHashFunctions() {
    hashFunctions.push_back(djb2);
    hashFunctions.push_back(sdbm);
}

void BloomFilter::add(std::string element) {
    for (std::vector<HashFunction>::iterator iter = hashFunctions.begin(); iter != hashFunctions.end(); iter++) {
        int hash = (*iter)(element);
        int index = hash % size;
        bloomVector[index] = true;
    }
}
    
bool BloomFilter::contains(std::string element) {
    for (auto iter = hashFunctions.begin(); iter != hashFunctions.end(); iter++) {
        if (bloomVector[(*iter)(element) % size] == false) {
            return false;
        }
    }
    return true;
}

void BloomFilter::exportToFile(std::string exportFilePath) {
    std::ofstream file(exportFilePath);
    for(std::vector<bool>::const_iterator i = bloomVector.begin(); i != bloomVector.end(); ++i) {
        file << *i;
    }
}
