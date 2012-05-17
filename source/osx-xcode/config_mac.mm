#import <Cocoa/Cocoa.h>
#import "resources.h"

string get_mac_dir()
{
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    NSString    *path = [[NSBundle mainBundle] bundlePath];
    string cpath = [path cStringUsingEncoding:[NSString defaultCStringEncoding]];
    [pool release];
    return cpath;
}
