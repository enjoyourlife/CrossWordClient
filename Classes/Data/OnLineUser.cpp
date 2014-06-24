//
//  OnLineUser.cpp
//  CrossWordClient
//
//  Created by cy on 14-6-7.
//
//

#include "OnLineUser.h"

OnLineUser::OnLineUser()
{
    m_uid = -1;
    m_username = "";
    m_sex = 1;
    m_exp = 0;
    m_lv = 0;
    m_name = "";
    m_achievement = "";
    m_gold = 0;
    m_isLevelUp = false;
    
    m_rewardGold = 0;//没有存到数据库中
    m_rewardExp = 0;
}

OnLineUser::~ OnLineUser()
{
}