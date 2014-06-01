//
//  DBManager.h
//  CrossWordClient
//
//  Created by cy on 14-5-24.
//
//

#ifndef __CrossWordClient__DBManager__
#define __CrossWordClient__DBManager__

#include <iostream>
#include "cocos2d.h"
#include "../easySQLite/SqlDatabase.h"
#include "../easySQLite/SqlField.h"
#include "../easySQLite/SqlTable.h"
#include "LocalUser.h"
#include "LocalUnLockLevel.h"


class DBManager : public cocos2d::CCObject
{
private:
    DBManager();
    ~DBManager();
    
    void init();
    
public:
    /*!
     * @brief		获取一个DBManager单例。
     */
    static DBManager* sharedDBManager();
    
    //根据username查询LocalUser
    LocalUser* getLocalUserByUsername(const std::string& username);
    void updateLocalUserByUsername(const std::string& username, int sex, int exp, int lv, const std::string& name, int silver, int version);
    //只更新单机玩家的银币
    void updateLocalUserSilver(const std::string& username, int silver);

    //根据version查询LocalUnLockLevel
    LocalUnLockLevel* getLocalUnLockLevelByVersion(int version);
    //根据version更新LocalUnLockLevel
    void updateLocalUnLockLevelByVersion(int unLockSubLevel0, int unLockSubLevel1, int unLockSubLevel2, int version);
    //当有新版本的时候 需要插入一条新的LocalUnLockLevel
    void addLocalUnLockLevel(int unLockSubLevel0, int unLockSubLevel1, int unLockSubLevel2, int level0Num, int level1Num, int level2Num, int version);
    
private:
    void openDataBase(const std::string& dataBaseName);
    void createDefaultTables();
    
private:
    sql::Database m_db;
    sql::Table* m_tbLocalUser;
    sql::Table* m_tbLocalUnLockLevel;
};

#endif /* defined(__CrossWordClient__DBManager__) */
