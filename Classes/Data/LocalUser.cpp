//
//  LocalUser.cpp
//  CrossWordClient
//
//  Created by cy on 14-5-24.
//
//

#include "LocalUser.h"

using namespace std;

LocalUser::LocalUser()
{
    m_username = "user";
    m_sex = 1;
    m_isLevelUp = false;
    m_version = 0;
    m_nextLvExp = 1;
}

LocalUser::~ LocalUser()
{
}

void LocalUser::setLocalUserJson(json_t* localUserJson)
{
    m_localUserJson = localUserJson;
}

