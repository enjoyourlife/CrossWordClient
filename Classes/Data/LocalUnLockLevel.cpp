//
//  LocalUnLockLevel.cpp
//  CrossWordClient
//
//  Created by cy on 14-5-31.
//
//

#include "LocalUnLockLevel.h"

LocalUnLockLevel::LocalUnLockLevel()
{
    m_unLockSubLevel0 = 2;
    m_unLockSubLevel1 = -1;
    m_unLockSubLevel2 = -1;
}

LocalUnLockLevel::~ LocalUnLockLevel()
{
}

void LocalUnLockLevel::setLocalUnLockLevelJson(json_t* localUnLockLevelJson)
{
    m_localUnLockLevelJson = localUnLockLevelJson;
}
