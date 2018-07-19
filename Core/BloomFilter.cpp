#include <iostream>
#include <string>
#include <vector>
#include <iterator>
#include <stdio.h>
#include <fstream>
#include <math.h>
#include "BloomFilter.hpp"


BloomFilter::BloomFilter(unsigned int items, double targetProbability) {
    unsigned int size = ceil((items * log(targetProbability)) / log(1.0 / (pow(2.0, log(2.0)))));
    bloomVector = std::vector<bool>(size);
    hashRounds = calculateHashRounds(size, items);
}

BloomFilter::BloomFilter(std::string importFilePath) {
    unsigned int items = 2900000; // TODO load number from file
    bloomVector = importFromFile(importFilePath);
    hashRounds = calculateHashRounds((unsigned int) bloomVector.size(), items);
}

unsigned int BloomFilter::calculateHashRounds(unsigned int size, unsigned int items) {
    return round(log(2.0) * size / items);
}

void BloomFilter::add(std::string element) {
    
    unsigned int hash1 = BloomFilter::djb2Hash(element);
    unsigned int hash2 = BloomFilter::sdbmHash(element);
    
    for (int i = 0; i < hashRounds; i++)
    {
        unsigned int hash = BloomFilter::doubleHash(hash1, hash2, i);
        unsigned int index = hash % bloomVector.size();
        bloomVector[index] = true;
    }
}

bool BloomFilter::contains(std::string element) {
    
    unsigned int hash1 = BloomFilter::djb2Hash(element);
    unsigned int hash2 =  BloomFilter::sdbmHash(element);
    
    for (int i = 0; i < hashRounds; i++) {
        unsigned int hash = BloomFilter::doubleHash(hash1, hash2, i);
        unsigned int index = hash % bloomVector.size();
        if (bloomVector[index] == false) {
            return false;
        }
    }
    
    return true;
}

unsigned int BloomFilter::djb2Hash(std::string text) {
    unsigned int hash = 5381;
    for (auto iterator = text.begin(); iterator != text.end(); iterator++) {
        hash = ((hash << 5) + hash) + *iterator;
    }
    return hash;
}

unsigned int BloomFilter::sdbmHash(std::string text) {
    unsigned int hash = 0;
    for (auto iterator = text.begin(); iterator != text.end(); iterator++) {
        hash = *iterator + ((hash << 6) + (hash << 16) - hash);
    }
    return hash;
}

unsigned int BloomFilter::doubleHash(unsigned int hash1, unsigned int hash2, unsigned int round) {
    switch(round) {
        case 0:
            return hash1;
        case 1:
            return hash2;
        default:
            return (hash1 + (round * hash2) + (round^2));
    }
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
