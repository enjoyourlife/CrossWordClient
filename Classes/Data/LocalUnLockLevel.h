//
//  LocalUnLockLevel.h
//  CrossWordClient
//
//  Created by cy on 14-5-31.
//  
//  单机解锁关卡数据

#ifndef __CrossWordClient__LocalUnLockLevel__
#define __CrossWordClient__LocalUnLockLevel__

#include <iostream>

#include <iostream>
#include "jansson.h"

class LocalUnLockLevel
{
public:
    LocalUnLockLevel();
    ~LocalUnLockLevel();
    
private:
    void setLocalUnLockLevelJson(json_t* localUnLockLevelJson);
    
public:
    int m_id;
    
    //依次为0 1 2大关解锁的小关卡 -1代表该大关未解锁
    int m_unLockSubLevel0;
    int m_unLockSubLevel1;
    int m_unLockSubLevel2;
    
    //对应的版本
    int m_version;
    
    //每个模式下的关卡一共有多少子关卡 在从服务器更新新包时 需要动态更新
    int m_level0Num;
    int m_level1Num;
    int m_level2Num;
    
    //与本地解锁关卡对应的json
    json_t* m_localUnLockLevelJson;
};

#endif /* defined(__CrossWordClient__LocalUnLockLevel__) */
