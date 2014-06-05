//
//  GPomeloGame.h
//  CrossWordClient
//
//  Created by cy on 14-5-6.
//
//

#ifndef __CrossWordClient__GPomeloGame__
#define __CrossWordClient__GPomeloGame__

#include <iostream>
#include "PomeloServer.h"

class GPomeloGame : public PomeloServer
{
public:
	GPomeloGame();
    ~GPomeloGame();
    
public:
    void initPomeloGame();
    
    //坐下
    void userEnter(int type, int level);
    //坐起
    void userExit();
    //发送已经正确的索引给服务器
    void sendRightWordsIndexToServer();
    
    void resetPomeloGame();
    
private:
    //对应于服务器方法的回调
    static void onUserEnter(pc_client_t *client, const char *event, void *data);
    static void onUserExit(pc_client_t *client, const char *event, void *data);
    static void onGameStart(pc_client_t *client, const char *event, void *data);
    static void onGameStop(pc_client_t *client, const char *event, void *data);
    static void onGameReady(pc_client_t *client, const char *event, void *data);
    static void onGameProc(pc_client_t *client, const char *event, void *data);
    static void onGameTime(pc_client_t *client, const char *event, void *data);
    static void onUserChat(pc_client_t *client, const char *event, void *data);
    static void onDisconnect(pc_client_t *client, const char *event, void *data);
    
    //集中添加服务器方法的回调
    void addServerListener();
    
    //本地请求方法的回调
    static void userEnterCallback(pc_request_t *req, int status, json_t *resp);
    static void userExitCallback(pc_request_t *req, int status, json_t *resp);
    static void sendCallback(pc_request_t *req, int status, json_t *resp);
    
    
    
};

#endif /* defined(__CrossWordClient__GPomeloGame__) */
