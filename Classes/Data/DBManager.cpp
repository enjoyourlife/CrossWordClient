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

Field tbLocalUser[8] =
{
    Field(FIELD_KEY),
    Field("m_username", type_text),
    Field("m_sex", type_int),
    Field("m_exp", type_int),
    Field("m_lv", type_int),
    Field("m_name", type_text),
    Field("m_silver", type_int),
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
        
        m_tbLocalUser->addRecord(&record);
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
        
        return localUser;
    }
    return NULL;
}

void DBManager::updateLocalUserByUsername(const string& username, int sex, int exp, int lv, const string& name, int silver)
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