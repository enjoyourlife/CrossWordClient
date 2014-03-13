//
//  DataManager.cpp
//  CrossWordClient
//
//  Created by cy on 14-2-28.
//
//

#include "DataManager.h"

using namespace std;
using namespace cocos2d;


static DataManager* s_dataManager = NULL;

DataManager::DataManager()
{
    EventManager::sharedEventManager()->addObserver(this);
}

DataManager::~DataManager()
{
    EventManager::sharedEventManager()->removeObserver(this);
}

void DataManager::init()
{
    m_singleSubLevel = 0;
    m_isLogin = false;
}

DataManager* DataManager::sharedDataManager()
{
    if (s_dataManager == NULL)
    {
        s_dataManager = new DataManager;
        s_dataManager->init();
    }
    return s_dataManager;
}

void DataManager::onEventSucceeded(Event *event)
{
    int type = event->getType();
    switch (type)
    {
        case EventTypeEnterSingleGame:
        {
            //做一些单机数据设置 界面的转换由对应的controller控制
            break;
        }
            
        default:
            break;
    }
}

void DataManager::onEventFailed(Event *event)
{
    
}

GameType DataManager::getGameType()
{
    return m_gameType;
}

void DataManager::setGameType(GameType gameType)
{
    m_gameType = gameType;
}

int DataManager::getSingleSubLevel()
{
    return m_singleSubLevel;
}

void DataManager::setSingleSubLevel(int singleSubLevel)
{
    m_singleSubLevel = singleSubLevel;
}

bool DataManager::getIsLogin()
{
    return m_isLogin;
}

void DataManager::setIsLogin(bool isLogin)
{
    m_isLogin = isLogin;
}
