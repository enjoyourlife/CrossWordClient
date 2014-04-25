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
    m_line = 0;
    m_col = 0;
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

int DataManager::getLevel()
{
    return m_level;
}

void DataManager::setLevel(int level)
{
    m_level = level;
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

vector<Grid*> DataManager::getGrids()
{
    return m_grids;
}


void DataManager::initGrids(vector<Grid*>& gridVector)
{
    clearGrids();
    m_grids = gridVector;
    
    
    
    for (vector<Grid*>::iterator it = m_grids.begin(); it != m_grids.end(); ++it)
    {
        Grid *grid = *it;
        CCLog("id %d %d  %d %d %d %d %d", grid->getIndex(), grid->getType(), grid->getType2(), grid->getPhraseIndex(), grid->getWordIndex(), grid->getPhrase2Index(), grid->getWord2Index());
    }
    
}

void DataManager::clearGrids()
{
    for (vector<Grid*>::iterator it = m_grids.begin(); it != m_grids.end(); ++it)
    {
        delete *it;
    }
    m_grids.clear();
}


vector<Words*> DataManager::getWords()
{
    return m_words;
}

void DataManager::initWords(vector<Words*>& words)
{
    clearWords();
    m_words = words;
}

void DataManager::clearWords()
{
    for (vector<Words*>::iterator it = m_words.begin(); it != m_words.end(); ++it)
    {
        delete *it;
    }
    m_words.clear();
}

int DataManager::getLine()
{
    return m_line;
}

int DataManager::getCol()
{
    return m_col;
}

void DataManager::setLine(int line)
{
    m_line = line;
}

void DataManager::setCol(int col)
{
    m_col = col;
}

Grid* DataManager::getClickGrid(int index)
{
    int size = m_grids.size();
    if (index >= 0 && index < size)
    {
        return m_grids.at(index);
    }
    return NULL;
}
