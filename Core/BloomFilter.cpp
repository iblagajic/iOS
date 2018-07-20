#include <iostream>
#include <string>
#include <vector>
#include <iterator>
#include <stdio.h>
#include <cstdio>
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

std::vector<bool> BloomFilter::importFromBase64File(std::string path) {
    std::ifstream inFile(path);
    std::vector<bool> output((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
    while(char c = inFile.get() && !inFile.eof()) {
        output.push_back(c);
    }
    return output;
}

typedef char BlockType;
typedef std::basic_istream<BlockType> BinaryInputStream;
typedef std::basic_ostream<BlockType> BinaryOutputStream;

template< typename T>
T pack(const std::vector<bool>& filter, const size_t block, const size_t bits)
{
    const size_t sizeOfTInBits = sizeof(T) * 8;
    assert( bits <= sizeOfTInBits );
    T buffer = 0;
    for ( int j = 0; j < bits; ++j )
    {
        const size_t offset = (block * sizeOfTInBits) + j;
        const T bit = filter[offset] << j;
        buffer |= bit;
    }
    return buffer;
}

void write(std::vector<bool>& bloomVector, BinaryOutputStream& out) {
    
    const size_t elements = bloomVector.size();
    const int count0 = ((elements & 0x000000ff) >> 0);
    const int count1 = ((elements & 0x0000ff00) >> 8);
    const int count2 = ((elements & 0x00ff0000) >> 16);
    const int count3 = ((elements & 0xff000000) >> 24);
    out.put( count0 );
    out.put( count1 );
    out.put( count2 );
    out.put( count3 );
    
    const size_t bitsPerBlock = sizeof(BlockType) * 8;
    for ( size_t i = 0; i < elements / bitsPerBlock; i++ )
    {
        const BlockType buffer = pack<BlockType>(bloomVector, i, bitsPerBlock);
        out.put( buffer );
    }
    
    const size_t bitsInLastBlock = elements % bitsPerBlock;
    if ( bitsInLastBlock > 0 )
    {
        const size_t lastBlock = elements / bitsPerBlock;
        const BlockType buffer = pack<BlockType>(bloomVector, lastBlock, bitsInLastBlock);
        out.put( buffer );
    }
}

void BloomFilter::exportToFile(std::string path) {
    std::basic_ofstream<BlockType> out(path.c_str(), std::ofstream::binary);
    write(bloomVector, out);
}

void unpackIntoVector(
    std::vector<bool>& bloomVector,
    const size_t offset,
    const size_t bitsInThisBlock,
    BinaryInputStream& in)
{
    const BlockType block = in.get();
    
    for ( int j = 0; j < bitsInThisBlock; j++ )
    {
        const BlockType mask = 1 << j;
        bloomVector[offset + j] = (block & mask) != 0;
    }
}

std::vector<bool> read(BinaryInputStream& in)
{
    const size_t component1 = in.get() << 0;
    const size_t component2 = in.get() << 8;
    const size_t component3 = in.get() << 16;
    const size_t component4 = in.get() << 24;
    const size_t elementCount = component1 + component2 + component3 + component4;
    
    std::vector<bool> bloomVector(elementCount);
    
    const size_t bitsPerBlock = sizeof(BlockType) * 8;
    const size_t fullBlocks = elementCount / bitsPerBlock;
    for ( int i = 0; i < fullBlocks; ++i )
    {
        const size_t offset = i * bitsPerBlock;
        unpackIntoVector(bloomVector, offset, bitsPerBlock, in);
    }
    
    const size_t bitsInLastBlock = elementCount % bitsPerBlock;
    if ( bitsInLastBlock > 0 )
    {
        const size_t offset = bitsPerBlock * fullBlocks;
        unpackIntoVector(bloomVector, offset, bitsInLastBlock, in);
    }
    
    return bloomVector;
}


std::vector<bool> BloomFilter::importFromFile(std::string path) {
    std::basic_ifstream<BlockType> inFile(path, std::ifstream::binary);
    return read(inFile);
}
