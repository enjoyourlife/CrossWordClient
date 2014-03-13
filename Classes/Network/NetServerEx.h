//
//  NetServerEx.h
//  CrossWordClient
//
//  Created by cy on 14-3-6.
//
//

#ifndef __CrossWordClient__NetServerEx__
#define __CrossWordClient__NetServerEx__

#include <iostream>

#include "cocos2d.h"
#include "pomelo.h"

class NetServerEx : cocos2d::CCObject
{
public:
	NetServerEx();
    ~NetServerEx();
    //创建一个NetServerEx单例
    static NetServerEx* sharedNetServerEx();
    
public:
    void login(const std::string username, const std::string password);
    
    /**
     发送msg给server
     */
    void sendMsg(const char *route, json_t *msg);
    
private:
    /**
     下面三个方法为连接服务器的方法
     register注册方法把这三个方法重新写一遍?
     */
    void requestGate();
    static void requestGateCallback(pc_request_t *req, int status, json_t *resp);
    static void requestConnectorCallback(pc_request_t *req, int status, json_t *resp);
    
private:
    /**
     sendMsg的回调
     */
    static void sendMsgCallback(pc_request_t *req, int status, json_t *resp);
    
    /*
     对应于服务器的onChat方法的回调
     */
    static void onChatCallback(pc_client_t *client, const char *event, void *data);
    
private:
    //作为成员变量 便于超时的时候销毁
    pc_client_t* m_gate_client;
    //超时时间
    float m_time;
    
};

#endif /* defined(__CrossWordClient__NetServerEx__) */