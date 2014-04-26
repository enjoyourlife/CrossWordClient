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
    
    //url编码解码 只对utf-8字符有用
    static unsigned char toHex(const unsigned char &x);
    static unsigned char fromHex(const unsigned char &x);
    static std::string URLEncode(const char *sIn);
    static std::string URLDecode(const char *sIn);
    
    /*
     *	字符串分割 返回的vector不带分割符delimiter
     */
    static std::vector<std::string> splitString(const std::string& str, const std::string& delimiter);
    
    // 对vector中索引为start至(end-1)的元素进行随机排列
    static void random_permute(std::vector<std::string>& array, int start, int end);
    // 对vector中所有元素进行随机排列
    static void random_permute(std::vector<std::string>& array);

};

#endif /* defined(__CrossWordClient__Utilities__) */
