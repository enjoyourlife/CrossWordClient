//
//  LocalUser.h
//  CrossWordClient
//
//  Created by cy on 14-5-24.
//
//  单机用户模型

#ifndef __CrossWordClient__LocalUser__
#define __CrossWordClient__LocalUser__

#include <iostream>
#include "jansson.h"

class LocalUser
{
public:
    LocalUser();
    ~LocalUser();
    
private:
    void setLocalUserJson(json_t* localUserJson);
    
public:
    int m_id;
    std::string m_username;
    //性别 0-女 1-男
    int m_sex;
    //经验
    int m_exp;
    //下一级的经验
    int m_nextLvExp;
    //等级
    int m_lv;
    //称号
    std::string m_name;
    //银币
    int m_silver;
    //是否升级了
    bool m_isLevelUp;
    //当前本地版本
    int m_version;
    //与本地用户对应的json
    json_t* m_localUserJson;
};

#endif /* defined(__CrossWordClient__LocalUser__) */
