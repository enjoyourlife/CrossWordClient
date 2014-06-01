//
//  DBManager.cpp
//  CrossWordClient
//
//  Created by cy on 14-5-24.
//
//

#include "DBManager.h"

USING_NS_CC;
using namespace std;
using namespace sql;


static DBManager* s_DBManager = NULL;

Field tbLocalUser[9] =
{
    Field(FIELD_KEY),
    Field("m_username", type_text),
    Field("m_sex", type_int),
    Field("m_exp", type_int),
    Field("m_lv", type_int),
    Field("m_name", type_text),
    Field("m_silver", type_int),
    Field("m_version", type_int),
    Field(DEFINITION_END)
};

Field tbLocalUnLockLevel[9] =
{
    Field(FIELD_KEY),
    Field("m_unLockSubLevel0", type_int),
    Field("m_unLockSubLevel1", type_int),
    Field("m_unLockSubLevel2", type_int),
    Field("m_version", type_int),
    Field("m_level0Num", type_int),
    Field("m_level1Num", type_int),
    Field("m_level2Num", type_int),
    Field(DEFINITION_END)
};


DBManager::DBManager()
{
    
}

DBManager::~DBManager()
{
    
}

void DBManager::init()
{
    openDataBase("crossworddb");
}

DBManager* DBManager::sharedDBManager()
{
    if (s_DBManager == NULL)
    {
        s_DBManager = new DBManager;
        s_DBManager->init();
    }
    return s_DBManager;
}

void DBManager::openDataBase(const string& dataBaseName) {
    try {
        string sqlPath = CCFileUtils::sharedFileUtils()->getWritablePath() + dataBaseName;
        m_db.open(sqlPath);
        
        createDefaultTables();
        
    } catch (Exception ex) {
        CCLOG("%s", ex.msg().c_str());
    }
}

void DBManager::createDefaultTables()
{
    m_tbLocalUser = new Table(m_db.getHandle(), "tb_localuser", tbLocalUser);
    if (!m_tbLocalUser->exists())
    {
        m_tbLocalUser->create();
        
        //初始化基础数值
        Record record(m_tbLocalUser->fields());
        
        record.setString("m_username", "user");
        record.setInteger("m_sex", 1);
        record.setInteger("m_exp", 0);
        record.setInteger("m_lv", 1);
        record.setString("m_name", "帝王");
        record.setInteger("m_silver", 1000);
        record.setInteger("m_version", 0);
        
        m_tbLocalUser->addRecord(&record);
    }
    
    
    m_tbLocalUnLockLevel = new Table(m_db.getHandle(), "tb_localunlocklevel", tbLocalUnLockLevel);
    if (!m_tbLocalUnLockLevel->exists())
    {
        m_tbLocalUnLockLevel->create();
        
        //初始化基础数值
        Record record(m_tbLocalUnLockLevel->fields());
        
        record.setInteger("m_unLockSubLevel0", 2);//初始化解锁前3关
        record.setInteger("m_unLockSubLevel1", -1);
        record.setInteger("m_unLockSubLevel2", -1);
        record.setInteger("m_version", 0);
        record.setInteger("m_level0Num", 10);
        record.setInteger("m_level1Num", 10);
        record.setInteger("m_level2Num", 10);
        
        m_tbLocalUnLockLevel->addRecord(&record);
    }
}

LocalUser* DBManager::getLocalUserByUsername(const string& username)
{
    ostringstream oss;
    oss << "m_username == '" << username << "'";
    string condition = oss.str();
    
    m_tbLocalUser->open(condition);
    Record* record = m_tbLocalUser->getTopRecord();
    if (record != NULL) {
        LocalUser* localUser = new LocalUser;
        
        localUser->m_id = record->getValue("_ID")->asInteger();
        localUser->m_username = record->getValue("m_username")->asString();
        localUser->m_sex = record->getValue("m_sex")->asInteger();
        localUser->m_exp = record->getValue("m_exp")->asInteger();
        localUser->m_lv = record->getValue("m_lv")->asInteger();
        localUser->m_name = record->getValue("m_name")->asString();
        localUser->m_silver = record->getValue("m_silver")->asInteger();
        localUser->m_version = record->getValue("m_version")->asInteger();
        
        return localUser;
    }
    return NULL;
}

void DBManager::updateLocalUserByUsername(const string& username, int sex, int exp, int lv, const string& name, int silver, int version)
{
    ostringstream oss;
    oss << "m_username == '" << username << "'";
    string condition = oss.str();
    
    m_tbLocalUser->open(condition);
    Record* record = m_tbLocalUser->getTopRecord();
    if (record != NULL)
    {
        record->setInteger("m_sex", sex);
        record->setInteger("m_exp", exp);
        record->setInteger("m_lv", lv);
        record->setString("m_name", name);
        record->setInteger("m_silver", silver);
        record->setInteger("m_version", version);
        m_tbLocalUser->updateRecord(record);
    }
}

void DBManager::updateLocalUserSilver(const string& username, int silver)
{
    ostringstream oss;
    oss << "m_username == '" << username << "'";
    string condition = oss.str();
    
    m_tbLocalUser->open(condition);
    Record* record = m_tbLocalUser->getTopRecord();
    if (record != NULL)
    {
        record->setInteger("m_silver", silver);
        m_tbLocalUser->updateRecord(record);
    }
}

LocalUnLockLevel* DBManager::getLocalUnLockLevelByVersion(int version)
{
    ostringstream oss;
    oss << "m_version == " << version;
    string condition = oss.str();
    
    m_tbLocalUnLockLevel->open(condition);
    Record* record = m_tbLocalUnLockLevel->getTopRecord();
    if (record != NULL) {
        LocalUnLockLevel* localUnLockLevel = new LocalUnLockLevel;
        
        localUnLockLevel->m_id = record->getValue("_ID")->asInteger();
        localUnLockLevel->m_unLockSubLevel0 = record->getValue("m_unLockSubLevel0")->asInteger();
        localUnLockLevel->m_unLockSubLevel1 = record->getValue("m_unLockSubLevel1")->asInteger();
        localUnLockLevel->m_unLockSubLevel2 = record->getValue("m_unLockSubLevel2")->asInteger();
        localUnLockLevel->m_version = record->getValue("m_version")->asInteger();
        localUnLockLevel->m_level0Num = record->getValue("m_level0Num")->asInteger();
        localUnLockLevel->m_level1Num = record->getValue("m_level1Num")->asInteger();
        localUnLockLevel->m_level2Num = record->getValue("m_level2Num")->asInteger();
        
        return localUnLockLevel;
    }
    return NULL;
}

void DBManager::updateLocalUnLockLevelByVersion(int unLockSubLevel0, int unLockSubLevel1, int unLockSubLevel2, int version)
{
    ostringstream oss;
    oss << "m_version == " << version;
    string condition = oss.str();
    
    m_tbLocalUnLockLevel->open(condition);
    Record* record = m_tbLocalUnLockLevel->getTopRecord();
    if (record != NULL)
    {
        record->setInteger("m_unLockSubLevel0", unLockSubLevel0);
        record->setInteger("m_unLockSubLevel1", unLockSubLevel1);
        record->setInteger("m_unLockSubLevel2", unLockSubLevel2);
        m_tbLocalUnLockLevel->updateRecord(record);
        
        //m_level0Num一般不会更新
    }
}

void DBManager::addLocalUnLockLevel(int unLockSubLevel0, int unLockSubLevel1, int unLockSubLevel2, int level0Num, int level1Num, int level2Num, int version)
{
    Record record(m_tbLocalUnLockLevel->fields());
    
    record.setInteger("m_unLockSubLevel0", unLockSubLevel0);
    record.setInteger("m_unLockSubLevel1", unLockSubLevel1);
    record.setInteger("m_unLockSubLevel2", unLockSubLevel2);
    record.setInteger("m_version", version);
    record.setInteger("m_level0Num", level0Num);
    record.setInteger("m_level1Num", level1Num);
    record.setInteger("m_level2Num", level2Num);
    
    m_tbLocalUnLockLevel->addRecord(&record);
}