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
    
    void setBonusType(int bonusType);
    void setBonusValue(int bonusValue);
    int getBonusType();
    int getBonusValue();
    void setIsGet(bool isGet);
    bool isGet();
private:
    //id 其实没有用到 用的是words在vector中的顺序
    int m_wordsId;
    //成语
    std::string m_name;
    //解析
    std::string m_tips;
    /*
     倒计时奖励的类型 0-无奖励 1-奖励银币或金币 2-奖励经验
     单机目前为每关随机选择一个成语作为经验或银币
     拥有这种奖励的成语 完成时不奖励 每个成语完成的奖励
     目前只有联网有经验奖励
    */
    int m_bonusType;
    //倒计时奖励的数值
    int m_bonusValue;
    //是否已经获得该奖励
    bool m_isGet;
    
};

#endif /* defined(__CrossWordClient__Words__) */
