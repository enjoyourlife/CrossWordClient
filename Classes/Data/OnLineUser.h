//
//  OnLineUser.h
//  CrossWordClient
//
//  Created by cy on 14-6-7.
//
//  联网用户数据
//

#ifndef __CrossWordClient__OnLineUser__
#define __CrossWordClient__OnLineUser__

#include <iostream>

class OnLineUser
{
public:
    OnLineUser();
    ~OnLineUser();
    
public:
    //uid
    int m_uid;
    //名字
    std::string m_username;
    //昵称 需要显示在界面上
    std::string m_nick;
    //性别 0-女 1-男
    int m_sex;
    //经验
    int m_exp;
    //等级
    int m_lv;
    //称号
    std::string m_name;
    //成就
    std::string m_achievement;
    //金币
    int m_gold;
    //是否升级了
    bool m_isLevelUp;
    
    //联网的时候 存放暂时奖励的经验和金币
    int m_rewardGold;
    int m_rewardExp;
};

#endif /* defined(__CrossWordClient__OnLineUser__) */
