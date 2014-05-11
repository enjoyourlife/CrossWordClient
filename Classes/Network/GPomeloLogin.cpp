//
//  GPomeloLogin.cpp
//  CrossWordClient
//
//  Created by cy on 14-5-6.
//
//

#include "GPomeloLogin.h"
#include "../Events/EventManager.h"
#include "../Events/GameEvents.h"
#include "../Data/DataManager.h"

USING_NS_CC;
using namespace std;


GPomeloLogin::GPomeloLogin()
{
    m_isInit = init(HOST, PORT);
    if (m_isInit)
    {
        addServerListener();
    }
    
}

GPomeloLogin::~GPomeloLogin()
{
	
}


void GPomeloLogin::onDisconnect(pc_client_t *client, const char *event, void *data)
{
    CCLog("GPomeloLogin onDisconnect~~~");
    //发送断线消息 执行重登录
}

void GPomeloLogin::login(const string username, const string password)
{
    if (m_isInit)//此时还不一定登录成功 需要在loginCallback里判断
    {
        const char *route = "connector.entryHandler.login";
        json_t *msg = json_object();
        json_t *username_str = json_string(username.c_str());
        json_t *password_str = json_string(password.c_str());
        json_object_set(msg, "usr", username_str);
        json_object_set(msg, "pwd", password_str);
        // decref for json object
        json_decref(username_str);
        json_decref(password_str);
        
        this->request(route, msg, GPomeloLogin::loginCallback);
    }
    else
    {
        //登录失败
    }
    
    
}

void GPomeloLogin::loginCallback(pc_request_t *req, int status, json_t *resp)
{
    if(status == -1)
    {
        Event* e = EventManager::sharedEventManager()->getEvent(EventTypeLogin);
        if (e != NULL)
        {
            EventManager::sharedEventManager()->notifyEventFailed(e);
        }
        
    }
    else if(status == 0)
    {
        //resp应该是服务器端通过next方法传过来的json串
        int code = json_integer_value(json_object_get(resp, "code"));
        if (code == 200)
        {
            const char *uid = json_string_value(json_object_get(resp, "uid"));
            DataManager::sharedDataManager()->setUserUid(uid);
            
            Event* e = EventManager::sharedEventManager()->getEvent(EventTypeLogin);
            if (e != NULL)
            {
                EventManager::sharedEventManager()->notifyEventSucceeded(e);
            }
        }
        else
        {
            Event* e = EventManager::sharedEventManager()->getEvent(EventTypeLogin);
            if (e != NULL)
            {
                EventManager::sharedEventManager()->notifyEventFailed(e);
            }
        }
    }
}

void GPomeloLogin::addServerListener()
{
    pc_add_listener(m_client, PC_EVENT_DISCONNECT, GPomeloLogin::onDisconnect);
}
