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
    void updateLocalUserByUsername(const std::string& username, int sex, int exp, int lv, const std::string& name, int silver);
    //只更新单机玩家的银币
    void updateLocalUserSilver(const std::string& username, int silver);

    
private:
    void openDataBase(const std::string& dataBaseName);
    void createDefaultTables();
    
private:
    sql::Database m_db;
    sql::Table* m_tbLocalUser;
};

#endif /* defined(__CrossWordClient__DBManager__) */
