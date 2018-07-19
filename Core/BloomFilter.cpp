#include <iostream>
#include <string>
#include <vector>
#include <iterator>
#include <stdio.h>
#include <fstream>
#include "BloomFilter.hpp"


unsigned int djb2(std::string text) {
    unsigned int hash = 5381;
    for (auto iterator = text.begin(); iterator != text.end(); iterator++) {
        hash = ((hash << 5) + hash) + *iterator;
    }
    return hash;
}

unsigned int sdbm(std::string text) {
    unsigned int hash = 0;
    
    for (auto iterator = text.begin(); iterator != text.end(); iterator++) {
        hash = *iterator + ((hash << 6) + (hash << 16) - hash);
    }
    return hash;
}

BloomFilter::BloomFilter() {
    bloomVector = std::vector<bool>(160000000);
    configureHashFunctions();
}

BloomFilter::BloomFilter(std::string importFilePath) {
    bloomVector = importFromFile(importFilePath);
    configureHashFunctions();
}

void BloomFilter::configureHashFunctions() {
    hashFunctions.push_back(djb2);
    hashFunctions.push_back(sdbm);
}

void BloomFilter::add(std::string element) {
    for (std::vector<HashFunction>::iterator iter = hashFunctions.begin(); iter != hashFunctions.end(); iter++) {
        int hash = (*iter)(element);
        int index = hash % bloomVector.size();
        bloomVector[index] = true;
    }
}
    
bool BloomFilter::contains(std::string element) {
    for (auto iter = hashFunctions.begin(); iter != hashFunctions.end(); iter++) {
        if (bloomVector[(*iter)(element) % bloomVector.size()] == false) {
            return false;
        }
    }
    return true;
}

std::vector<bool> BloomFilter::importFromFile(std::string path) {
    std::ifstream inFile(path);
    std::vector<bool> output((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
    while(char c = inFile.get() && !inFile.eof()) {
        output.push_back(c);
    }
    return output;
}

void BloomFilter::exportToFile(std::string path) {
    std::ofstream outFile(path);
    std::copy(bloomVector.begin(), bloomVector.end(), std::ostreambuf_iterator<char>(outFile));
}
