//
//  System.h
//  CrossWordClient
//
//  Created by cy on 14-3-4.
//
//

#ifndef __CrossWordClient__System__
#define __CrossWordClient__System__

#include <iostream>

class System
{
public:
    /*!
     * @brief		检测网络是否可用。
     */
    static bool isNetAvailable();
    
    /*!
     * @brief		检测WIFI是否可用。
     */
    static bool isWIFIEnabled();
    
    /*!
     * @brief		检测3G网络是否可用。
     */
    static bool is3GEnabled();
};

#endif /* defined(__CrossWordClient__System__) */
