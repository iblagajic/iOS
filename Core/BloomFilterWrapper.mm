//
//  BloomFilterWrapper.m
//  Core
//
//  Created by duckduckgo on 11/07/2018.
//  Copyright © 2018 DuckDuckGo. All rights reserved.
//

#import "BloomFilterWrapper.h"
#import "BloomFilterData.h"
#import "BloomFilter.hpp"


@interface BloomFilterWrapper() {
    BloomFilter *filter;
}
@end

@implementation BloomFilterWrapper

-(instancetype)init
{
    self = [super init];
    NSArray* data = [BloomFilterData httpsTestData];
    filter = new BloomFilter((unsigned int) data.count, 0.0001);
    [self addData: data];
    [self exportData];
    [self importData];
    [self runTests];
    return self;
}

-(void)importData
{
    NSString *documentdir = [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) lastObject];
    NSString *path = [documentdir stringByAppendingPathComponent:@"bloomFilter.bin"];
    NSLog(@"Bloom: Importing data from %@", path);
    filter = new BloomFilter([path cStringUsingEncoding:NSUTF8StringEncoding]);
}

-(void)exportData
{
    NSString *documentdir = [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) lastObject];
    NSString *path = [documentdir stringByAppendingPathComponent:@"bloomFilter.bin"];
    NSLog(@"Bloom: Exporting data to %@", path);
    filter->exportToFile([path cStringUsingEncoding:NSUTF8StringEncoding]);
}

- (void)addData:(NSArray*)data
{
    for (int i=0; i<data.count; i++) {
        filter->add([data[i] UTF8String]);
    }
}

- (BOOL)checkEntry:(NSString*)entry
{
    if (entry == nil) {
        return false;
    }
    double startTime = [NSDate new].timeIntervalSince1970;
    BOOL result = filter->contains([entry UTF8String]);
    double endTime = [NSDate new].timeIntervalSince1970;
    double totalTime = endTime - startTime;
    NSLog(@"Bloom: Site %@ %s be updated. Lookup took %f", entry, (result ? "can" : "cannot"), totalTime);
    return result;
}

-(void)runTests
{
    int falsePositives = 0;
    int truePositives = 0;
    int falseNegatives = 0;
    int trueNegatives = 0;

    NSArray* bloomData = [BloomFilterData httpsTestData];
    NSArray* testData = [BloomFilterData topSitesTestData];
    
    for(int i=0; i<testData.count; i++) {
    
        NSString* site = testData[i];
        BOOL result = filter->contains([site UTF8String]);
        if (result == false && [bloomData containsObject:site]) {
            falseNegatives++;
        }
        else if (result == false && ![bloomData containsObject:site]) {
            trueNegatives++;
        }
        else if (result == true && ![bloomData containsObject:site]) {
            falsePositives++;
            NSLog(@"Bloom %d: Site %@ was a false positive", i, site);
        }
        else if (result == true && [bloomData containsObject:site]) {
            truePositives++;
        }
    }

    NSLog(@"False positive %d", falsePositives);
    NSLog(@"True positive %d", truePositives);
    NSLog(@"False negative %d", falseNegatives);
    NSLog(@"True negative %d", trueNegatives);
    NSLog(@"False positive rate %lu", falsePositives / testData.count);
}

@end
