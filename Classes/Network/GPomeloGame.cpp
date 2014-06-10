//
//  GPomeloGame.cpp
//  CrossWordClient
//
//  Created by cy on 14-5-6.
//
//

#include "GPomeloGame.h"
#include "../Events/EventManager.h"
#include "../Events/GameEvents.h"
#include "../Data/DataManager.h"

USING_NS_CC;
using namespace std;


GPomeloGame::GPomeloGame()
{
    
    
}

GPomeloGame::~GPomeloGame()
{
	
}

void GPomeloGame::initPomeloGame()
{
    if (!m_isInit)
    {
        m_isInit = init(HOST, PORT);
        if (m_isInit)
        {
            addServerListener();
        }
    }
}

void GPomeloGame::userEnter(int type, int level)
{
    initPomeloGame();
    
    if (m_isInit)
    {
        const char *route = "connector.entryHandler.enter";
        json_t *msg = json_object();
        
        json_t *gidJson = json_string("crossword");
        json_t *userJson = json_string(DataManager::sharedDataManager()->getUsername().c_str());
        json_t *uuidJson = json_string(DataManager::sharedDataManager()->getUserUuid().c_str());
        
        json_t *cid = json_object();
        json_t *typeJson = json_integer(type);
        json_t *levelJson = json_integer(level);
        json_object_set(cid, "level", levelJson);
        json_object_set(cid, "type", typeJson);
        
        
        
        json_object_set(msg, "gid", gidJson);
        json_object_set(msg, "usr", userJson);
        json_object_set(msg, "uuid", uuidJson);
        json_object_set(msg, "cid", cid);
        
        // 使用的时候记得删除不用的变量
        json_decref(gidJson);
        json_decref(userJson);
        json_decref(uuidJson);
        json_decref(cid);
        
        this->request(route, msg, GPomeloGame::userEnterCallback);

    }
    else
    {
        
    }
}

void GPomeloGame::userEnterCallback(pc_request_t *req, int status, json_t *resp)
{
    char *json_str = json_dumps(resp, 0);
    CCLOG("userEnterCallback resp is : %s \n", json_str);
    
    if(status == -1)
    {
        
        
    }
    else if(status == 0)
    {
        //resp应该是服务器端通过next方法传过来的json串
        int code = json_integer_value(json_object_get(resp, "code"));
        if (code == 200)
        {
            
        }
        else
        {
            
        }
    }
}

void GPomeloGame::userExit()
{
    initPomeloGame();
    
    if (m_isInit)
    {
        const char *route = "connector.entryHandler.exit";
        json_t *msg = json_object();
        this->request(route, msg, GPomeloGame::userExitCallback);
    }
    else
    {
        
    }
}

void GPomeloGame::userExitCallback(pc_request_t *req, int status, json_t *resp)
{
    char *json_str = json_dumps(resp, 0);
    CCLOG("userExit resp is : %s \n", json_str);
    
}

void GPomeloGame::sendRightWordsIndexToServer()
{
    initPomeloGame();
    
    if (m_isInit)
    {
        const char *route = "crossword.gameHandler.send";
        json_t *msg = json_object();
        
        vector<int> rightWordsIndexVec = DataManager::sharedDataManager()->getRightWordsIndexVec();
        vector<int>::iterator it;
        json_t *valJson = json_array();
        for (it = rightWordsIndexVec.begin(); it != rightWordsIndexVec.end(); it++)
        {
            int index = *it;
            json_t *indexJson = json_integer(index);
            json_array_append(valJson, indexJson);
            
            json_decref(indexJson);
        }
        json_object_set(msg, "val", valJson);
        json_decref(valJson);
        
        this->request(route, msg, GPomeloGame::sendCallback);
        
    }
    else
    {
        
    }
}

void GPomeloGame::sendCallback(pc_request_t *req, int status, json_t *resp)
{
    char *json_str = json_dumps(resp, 0);
    CCLOG("sendCallback resp is : %s \n", json_str);
    
}


void GPomeloGame::onUserEnter(pc_client_t *client, const char *event, void *data)
{
    CCLog("onUserEnter~~~~~~~~~~");
    
    char *json_str = json_dumps((json_t* )data, 0);
    CCLOG("onUserEnter data is : %s \n", json_str);
    
}

void GPomeloGame::onUserExit(pc_client_t *client, const char *event, void *data)
{
    CCLog("onUserExit~~~~~~~~~~");

}

void GPomeloGame::onGameStart(pc_client_t *client, const char *event, void *data)
{
    CCLog("onGameStart~~~~~~~~~~");
    
    char *json_str = json_dumps((json_t* )data, 0);
    CCLOG("onGameStart data is : %s \n", json_str);
    
    Event *e = new Event(EventTypeGameStart);
    EventManager::sharedEventManager()->addEvent(e);
    
    json_t* json = (json_t* )data;
    json_t* gameDataJson = json_object_get(json, "chess");
    DataManager::sharedDataManager()->parseJson(gameDataJson);

}

void GPomeloGame::onGameStop(pc_client_t *client, const char *event, void *data)
{
    CCLog("onGameStop~~~~~~~~~~");

}

void GPomeloGame::onGameReady(pc_client_t *client, const char *event, void *data)
{
    CCLog("onGameReady~~~~~~~~~~");
    char *json_str = json_dumps((json_t* )data, 0);
    CCLOG("onGameReady data is : %s \n", json_str);

}

void GPomeloGame::onGameProc(pc_client_t *client, const char *event, void *data)
{
    CCLog("onGameProc~~~~~~~~~~");
    
    char *json_str = json_dumps((json_t* )data, 0);
    CCLOG("onGameProc data is : %s \n", json_str);
    
    
    json_t* json = (json_t* )data;
    DataManager::sharedDataManager()->parseGameProcJson(json);
    
    Event *e = new Event(EventTypeUpdateMain);
    EventManager::sharedEventManager()->addEvent(e);
    
    
    
}

void GPomeloGame::onGameTime(pc_client_t *client, const char *event, void *data)
{
    CCLog("onGameTime~~~~~~~~~~");

}

void GPomeloGame::onUserChat(pc_client_t *client, const char *event, void *data)
{
    CCLog("onUserChat~~~~~~~~~~");

}

void GPomeloGame::onDisconnect(pc_client_t *client, const char *event, void *data)
{
    CCLog("GPomeloGame onDisconnect~~~");
    //发送断线消息 清空m_client等 然后每次进行操作前都会检查  没有初始化就调用initPomeloGame初始化一下
    DisconnectEvent *disconnectEvent = new DisconnectEvent(2);
    EventManager::sharedEventManager()->addEvent(disconnectEvent);
}

void GPomeloGame::addServerListener()
{
    pc_add_listener(m_client, "onUserEnter", GPomeloGame::onUserEnter);
    pc_add_listener(m_client, "onUserExit", GPomeloGame::onUserExit);
    pc_add_listener(m_client, "onGameStart", GPomeloGame::onGameStart);
    pc_add_listener(m_client, "onGameStop", GPomeloGame::onGameStop);
    pc_add_listener(m_client, "onGameReady", GPomeloGame::onGameReady);
    pc_add_listener(m_client, "onGameProc", GPomeloGame::onGameProc);
    pc_add_listener(m_client, "onGameTime", GPomeloGame::onGameTime);
    pc_add_listener(m_client, "onUserChat", GPomeloGame::onUserChat);
    pc_add_listener(m_client, PC_EVENT_DISCONNECT, GPomeloGame::onDisconnect);
}

void GPomeloGame::resetPomeloGame()
{
    PomeloServer::reset();
}
