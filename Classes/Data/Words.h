//
//  Words.h
//  CrossWordClient
//
//  Created by cy on 14-4-20.
//
//  词语和解析文字 类

#ifndef __CrossWordClient__Words__
#define __CrossWordClient__Words__

#include <iostream>

class Words
{
public:
    Words(int wordsId, const std::string& name, const std::string& tips);
    ~ Words();
    
    int getWordsId();
    const std::string& getName();
    const std::string& getTips();
    
private:
    //id 其实没有用到 用的是words在vector中的顺序
    int m_wordsId;
    //成语
    std::string m_name;
    //解析
    std::string m_tips;
};

#endif /* defined(__CrossWordClient__Words__) */
