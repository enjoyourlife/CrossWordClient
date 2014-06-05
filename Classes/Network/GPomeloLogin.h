//
//  GPomeloLogin.h
//  CrossWordClient
//
//  Created by cy on 14-5-6.
//
//

#ifndef __CrossWordClient__GPomeloLogin__
#define __CrossWordClient__GPomeloLogin__

#include <iostream>
#include "PomeloServer.h"

class GPomeloLogin : public PomeloServer
{
public:
	GPomeloLogin();
    ~GPomeloLogin();
    
public:
    //把构造函数里的初始化方法放出来 避免一初始化就调用
    void initPomeloLogin();
    
    void login(const std::string username, const std::string password);
    
    //重置事件
    void resetLogin();
    
private:
    //对应于服务器方法的回调
    static void onDisconnect(pc_client_t *client, const char *event, void *data);
    
    //集中添加服务器方法的回调
    void addServerListener();
    
    //本地请求方法的回调
    static void loginCallback(pc_request_t *req, int status, json_t *resp);
    
    
    
};


#endif /* defined(__CrossWordClient__GPomeloLogin__) */
