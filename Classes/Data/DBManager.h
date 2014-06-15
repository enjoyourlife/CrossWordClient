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
#include "Answer.h"


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
    void updateLocalUserByUsername(const std::string& username, int sex, int exp, int nextLvExp, int lv, const std::string& name, int silver, int version);
    //只更新单机玩家的银币
    void updateLocalUserSilver(const std::string& username, int silver);

    //根据version查询LocalUnLockLevel
    LocalUnLockLevel* getLocalUnLockLevelByVersion(int version);
    //根据version更新LocalUnLockLevel
    void updateLocalUnLockLevelByVersion(int unLockSubLevel0, int unLockSubLevel1, int unLockSubLevel2, int version);
    //当有新版本的时候 需要插入一条新的LocalUnLockLevel
    void addLocalUnLockLevel(int unLockSubLevel0, int unLockSubLevel1, int unLockSubLevel2, int level0Num, int level1Num, int level2Num, int version);
    
    //增加玩家已经选择的答案 在每局游戏退出前调用
    void addSelectAnswer(int level, int subLevel, int version, int index, int phraseIndex, int wordIndex, int phrase2Index, int word2Index, const std::string& oriAnswerWord, const std::string& answerWord, bool isSame, bool isFix);
    //获取SelectAnswer
    std::vector<Answer*> listSelectAnswer(int level, int subLevel, int version);
    //删除SelectAnswer
    bool deleteSelectAnswer(int level, int subLevel, int version);
    
private:
    void openDataBase(const std::string& dataBaseName);
    void createDefaultTables();
    
private:
    sql::Database m_db;
    sql::Table* m_tbLocalUser;
    sql::Table* m_tbLocalUnLockLevel;
    sql::Table* m_tbSelectAnswer;
};

#endif /* defined(__CrossWordClient__DBManager__) */
