//
//  SaveDataManager.cpp
//  CrossWordClient
//
//  Created by cy on 14-3-4.
//
//

#include "SaveDataManager.h"
using namespace std;
using namespace cocos2d;


static SaveDataManager* s_saveDataManager = NULL;

SaveDataManager::SaveDataManager()
{
    
}

SaveDataManager::~SaveDataManager()
{
    
}

void SaveDataManager::init()
{
    if (!CCUserDefault::sharedUserDefault()->getBoolForKey("isGameInitialized"))
    {
        CCUserDefault::sharedUserDefault()->setStringForKey("username", "");
        CCUserDefault::sharedUserDefault()->setStringForKey("password", "");
        
        CCUserDefault::sharedUserDefault()->setBoolForKey("isGameInitialized", true);
    }
    CCUserDefault::sharedUserDefault()->flush();
}

SaveDataManager* SaveDataManager::sharedSaveDataManager()
{
    if (s_saveDataManager == NULL)
    {
        s_saveDataManager = new SaveDataManager;
        s_saveDataManager->init();
    }
    return s_saveDataManager;
}

void SaveDataManager::setUsername(const string& username)
{
    CCUserDefault::sharedUserDefault()->setStringForKey("username", username);
    CCUserDefault::sharedUserDefault()->flush();
}

const string SaveDataManager::getUsername()
{
    return CCUserDefault::sharedUserDefault()->getStringForKey("username");
}

void SaveDataManager::setPassword(const string& password)
{
    CCUserDefault::sharedUserDefault()->setStringForKey("password", password);
    CCUserDefault::sharedUserDefault()->flush();
}

const string SaveDataManager::getPassword()
{
    return CCUserDefault::sharedUserDefault()->getStringForKey("password");
}
