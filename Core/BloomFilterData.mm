#import "BloomFilterData.h"

@implementation BloomFilterData

+(NSArray*)httpsTestData
{
    // Note: I have not committed full test data, which contains nearly 3 million records
    // To generate the full data run the update-bloom-filter.bash script
    // in https://dub.duckduckgo.com/duckduckgo/ddg/pull/13079/files#diff-11314e25e35e0712f11ce0475f27d0daR9
    // commentout the last line /tmp2/ddg/upgrades.txt so the raw upgrades are not deleted
    return [self loadData:@"bloom_https_data"];
}

+(NSArray*)topSitesTestData
{
    // Note I have not committed full test data. Top 200k urls were taken from
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
