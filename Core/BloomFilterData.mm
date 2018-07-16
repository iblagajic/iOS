#import "BloomFilterData.h"

@implementation BloomFilterData

+(NSArray*)httpsTestData
{
    // Note I have not committed full test data. Test urls were taken from
    // https://duckduckgo.com/contentblocking.js?l=https2
    return [self loadData:@"bloom_https_data"];
}

+(NSArray*)topSitesTestData
{
    // Note I have not committed full test data. Top-sites urls were taken from
    // http://downloads.majesticseo.com/majestic_million.csv
    return [self loadData:@"bloom_top_sites"];
}

+(NSArray*)loadData:(NSString*)fileName
{
    NSBundle *bundle = [NSBundle bundleForClass:[self class]];
    NSString *path = [bundle pathForResource:fileName ofType:@"txt"];
    NSString *contents = [NSString stringWithContentsOfFile:path encoding:NSUTF8StringEncoding error:nil];
    return [contents componentsSeparatedByCharactersInSet:[NSCharacterSet newlineCharacterSet]];
}

@end
