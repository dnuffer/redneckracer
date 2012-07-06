#import <Foundation/Foundation.h>

#include "EngineConfig.hpp"
#include "Resources.hpp"
#include "platform/CCFileUtils.h"


namespace engine
{
	
	namespace Resources
	{
		using namespace cocos2d;
		
		StringArray listResources()
		{
			StringArray results;
			const char* cPath = CCFileUtils::fullPathFromRelativePath("assets/");
			NSString* path = [NSString stringWithUTF8String: cPath];
			
			NSError* error = nil;
			NSArray* array = [[NSFileManager defaultManager] subpathsOfDirectoryAtPath:path error:&error];
			if (array != nil)
			{
				for (int i = 0; i < [array count]; ++i) {
					results.push_back([[array objectAtIndex:i] UTF8String]);
				}
			}
			return results;
		}
		
	}
}
