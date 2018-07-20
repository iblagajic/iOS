//
//  BloomFilterWrapper.h
//  DuckDuckGo
//
//  Created by duckduckgo on 11/07/2018.
//  Copyright © 2018 DuckDuckGo. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface BloomFilterWrapper : NSObject
- (BOOL)checkEntry:(NSString*) entry;
- (void)runValidationTests;
@end
