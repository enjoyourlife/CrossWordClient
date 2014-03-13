//
//  Utilities.h
//  CrossWordClient
//
//  Created by cy on 14-3-2.
//
//  工具类

#ifndef __CrossWordClient__Utilities__
#define __CrossWordClient__Utilities__

#include <iostream>
#include "cocos2d.h"
#include "jansson.h"

class Utilities
{
public:
    /*!
     * @brief	将传进来的json文件解析
     * @param   fileName  文件名
     * @return  json 解析值
     */
    static json_t* getJsonFromFile(const char* fileName);
    
    //创建CCAnimate
    static cocos2d::CCAnimate* createAnimate(const char* fileName, int frameCount, float delay);
};

#endif /* defined(__CrossWordClient__Utilities__) */
