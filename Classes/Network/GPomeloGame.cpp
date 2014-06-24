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
    
    //目前只有两个玩家 在此函数里判读 取得对手玩家的uid然后发送取信息事件 获取对手的信息
    /*
     onUserEnter data is : {"users": [{"idx": 0, "uid": 2}, {"idx": 1, "uid": 1}], "route": "onUserEnter", "uid": 1, "cid": "channel0"} 
     只有一个玩家的时候 其中一个uid为null
     */
    json_t* json = (json_t* )data;
    json_t* usersJson = json_object_get(json, "users");
    size_t usersSize = json_array_size(usersJson);
    
    for (int i = 0; i < usersSize; i++)
    {
        json_t *userJson = json_array_get(usersJson, i);
        
        json_t* uidJson = json_object_get(userJson, "uid");
        if (uidJson != NULL)
        {
            int uid = json_integer_value(uidJson);
            int ownUid = DataManager::sharedDataManager()->getOwnOnLineUser()->m_uid;
            if (ownUid != uid && uid != 0)
            {
                GetInfoEvent *getInfoEvent = new GetInfoEvent(uid);
                EventManager::sharedEventManager()->addEvent(getInfoEvent);
                CCLog("ownUid is %d and uid is %d~~~~~~~~~~~~~", ownUid, uid);
                break;
            }
        }
    }

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
    char *json_str = json_dumps((json_t* )data, 0);
    CCLOG("onGameStop data is : %s \n", json_str);
    /*
     flags 有特殊奖励的词语
     flag GameStop的各种情况
     0 － 一个人或者两个人赢了
     1 － 合作模式时间到
     2 － 游戏中途对手退出 游戏停止
     
     竞技模式 onGameStop data is : {"users": [{"idx": 0, "chess": [0, 0, 0, 0, 0, 0, 0, 0, 0], "rewards": {"gold": 0, "exp": 0}, "uid": 2, "flags": [1, 0, 0, 0, 0, 0, 0, 0, 0]}, {"idx": 1, "chess": [1, 1, 1, 1, 1, 1, 1, 1, 1], "rewards": {"gold": 38, "exp": 43}, "uid": 1, "flags": [1, 0, 0, 0, 0, 0, 0, 0, 0]}], "route": "onGameStop", "flag": 0} 
     */
    json_t* json = (json_t* )data;
    json_t* flagJson = json_object_get(json, "flag");
    int flag = json_integer_value(flagJson);
    switch (flag)
    {
        case 0:
        {
            json_t* usersJson = json_object_get(json, "users");
            size_t usersSize = json_array_size(usersJson);
            
            //只需要考虑自己的情况 对方玩家有自己的结算框
            for (int i = 0; i < usersSize; i++)
            {
                json_t *userJson = json_array_get(usersJson, i);
                
                //这里变成uid后 应该是数字
                json_t *uidJson = json_object_get(userJson, "uid");
                int uidData = json_integer_value(uidJson);
                
                if (uidData == DataManager::sharedDataManager()->getOwnUid())//自己
                {
                    json_t *chessJson = json_object_get(userJson, "chess");
                    size_t chessSize = json_array_size(chessJson);
                    
                    json_t *rewardsJson = json_object_get(userJson, "rewards");
                    json_t *goldJson = json_object_get(rewardsJson, "gold");
                    json_t *expJson = json_object_get(rewardsJson, "exp");
                    int gold = json_integer_value(goldJson);
                    int exp = json_integer_value(expJson);
                    
                    bool isWin = true;
                    for (int j = 0; j < chessSize; j++)
                    {
                        json_t *cJson = json_array_get(chessJson, j);
                        int c = json_integer_value(cJson);
                        if (c == 0)
                        {
                            isWin = false;
                            break;
                        }
                    }
                    
                    //这里是总的奖励 显示就行了
                    DataManager::sharedDataManager()->getOwnOnLineUser()->m_rewardGold = gold;
                    DataManager::sharedDataManager()->getOwnOnLineUser()->m_rewardExp = exp;
                    
                    //发送游戏停止消息
                    GameStopEvent *gameStopEvent = new GameStopEvent(flag);
                    gameStopEvent->setIsWin(isWin);
                    EventManager::sharedEventManager()->addEvent(gameStopEvent);
                }
                
                            
            }
            
            break;
        }
            
        case 1:
        {
            GameStopEvent *gameStopEvent = new GameStopEvent(flag);
            gameStopEvent->setIsWin(false);//两个玩家都输了
            EventManager::sharedEventManager()->addEvent(gameStopEvent);
            break;
        }
            
        case 2:
        {
            break;
        }
            
        default:
            break;
    }

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
    
    char *json_str = json_dumps((json_t* )data, 0);
    CCLOG("onGameTime data is : %s \n", json_str);

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
