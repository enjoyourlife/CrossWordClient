//
//  NetServer.h
//  CrossWordClient
//
//  Created by cy on 14-2-20.
//
//

#ifndef __CrossWordClient__NetServer__
#define __CrossWordClient__NetServer__

#include <iostream>
#include "cocos2d.h"
#include "pomelo.h"

class NetServer
{
public:
	NetServer();
    ~NetServer();
    //创建一个NetServer单例
    static NetServer* sharedNetServer();
    
public:
    /**
     发送msg给server
     */
    void sendMsg(const char *route, json_t *msg);
    
private:
    /**
     下面三个方法连接默认的服务器
     */
    void requestGate();
    static void requestGateCallback(pc_request_t *req, int status, json_t *resp);
    static void requestConnectorCallback(pc_request_t *req, int status, json_t *resp);
    /**
     sendMsg的回调
     */
    static void sendMsgCallback(pc_request_t *req, int status, json_t *resp);
    
    /*
     对应于服务器的onChat方法的回调
     */
    static void onChatCallback(pc_client_t *client, const char *event, void *data);
};

#endif /* defined(__CrossWordClient__NetServer__) */
