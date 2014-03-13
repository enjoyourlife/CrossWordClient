//
//  System.cpp
//  CrossWordClient
//
//  Created by cy on 14-3-4.
//
//

#include "System.h"

#import "Reachability.h"

//网络是否可用
bool System::isNetAvailable()
{
    return   (isWIFIEnabled() ||  is3GEnabled());
}

// 检测WIFI是否可用
bool System::isWIFIEnabled()
{
    return ([[Reachability reachabilityForLocalWiFi] currentReachabilityStatus] != NotReachable);
}

// 检测3G网络是否可用
bool System::is3GEnabled()
{
    return [[Reachability reachabilityForInternetConnection] isReachableViaWWAN];
}