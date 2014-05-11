//
//  PomeloServer.h
//  CrossWordClient
//
//  Created by cy on 14-5-4.
//
//

#ifndef __CrossWordClient__PomeloServer__
#define __CrossWordClient__PomeloServer__

#include <iostream>
#include "cocos2d.h"
#include "pomelo.h"

#define HOST "http://192.168.0.2:1337/getip"
//#define HOST "http://gamepans.eicp.net:1337/getip"
#define PORT 3010

class PomeloServer : cocos2d::CCObject
{
public:
	PomeloServer();
    ~PomeloServer();
    
public:
    //通过域名获取ip
    void getIpByHost(const char* host);
    //初始化m_client
    bool init(const char* host, u_short port);
    /*
     发起请求
     cb 回调函数
     */
    void request(const char* route, json_t *msg, pc_request_cb cb);
    
    void reset();
    
protected:
    //通过域名获取到的ip
    std::string m_ip;
    //与服务器通信用的client
    pc_client_t* m_client;
    //init函数是否初始化成功
    bool m_isInit;
};

#endif /* defined(__CrossWordClient__PomeloServer__) */
