//
//  NetServerEx.cpp
//  CrossWordClient
//
//  Created by cy on 14-3-6.
//
//

#include "NetServerEx.h"
#include "../Events/EventManager.h"
#include "../Events/GameEvents.h"
#include "../Data/DataManager.h"


USING_NS_CC;
using namespace std;

#define GATE_HOST "192.168.0.2"
//#define GATE_HOST "127.0.0.1"
#define GATE_PORT 3014

static NetServerEx* s_netServerEx = NULL;
static pc_client_t* pomelo_client = NULL;
static string s_username = "";
static string s_password = "";
//是否已经超时 登录失败
static bool s_loginFailed = false;

NetServerEx* NetServerEx::sharedNetServerEx()
{
	if (s_netServerEx == NULL)
	{
		s_netServerEx = new NetServerEx;
	}
	return s_netServerEx;
}

NetServerEx::NetServerEx()
{
    
}

NetServerEx::~NetServerEx()
{
	
}

void NetServerEx::login(const string username, const string password)
{
//    if (m_gate_client != NULL)
//    {
//        pc_client_destroy(m_gate_client);
//        m_gate_client = NULL;
//    }
//
//    if (pomelo_client != NULL)
//    {
//        pc_client_destroy(pomelo_client);
//        pomelo_client = NULL;
//    }
    
    s_username = username;
    s_password = password;
    m_time = 0.0f;
    s_loginFailed = false;
    
    requestGate();
}

void NetServerEx::requestGate()
{
    CCLog("NetServerEx::requestGate~~~");
    
    const char *ip = GATE_HOST;
    int port = GATE_PORT;
    
//    pc_client_t *client = pc_client_new();//创建一个新的pomelo client实例
    m_gate_client = pc_client_new();

    
    struct sockaddr_in address;
    
    memset(&address, 0, sizeof(struct sockaddr_in));
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = inet_addr(ip);
    
    //连接到服务器，在连接过程中会创建子线程用于处理网络I/O
    if(pc_client_connect(m_gate_client, &address))
    {
        CCLog("fail to connect gate server.\n");
        pc_client_destroy(m_gate_client);//销毁客户端的连接
        
        s_loginFailed = true;
        Event* e = EventManager::sharedEventManager()->getEvent(EventTypeLogin);
        if (e != NULL)
        {
            EventManager::sharedEventManager()->notifyEventFailed(e);
        }
        
        return;
    }
    
    //添加一个事件监听 比如断开重连
    //	pc_add_listener(client, PC_EVENT_DISCONNECT, on_close);
    
    const char *route = "gate.gateHandler.queryEntry";
    json_t *msg = json_object();
    json_t *str = json_string("hello world");
    json_object_set(msg, "uid", str);
    // decref for json object
    json_decref(str);
    
    //创建一个request请求实例
    pc_request_t *request = pc_request_new();//1
    void (*on_request_gate_cb)(pc_request_t *req, int status, json_t *resp) = &NetServerEx::requestGateCallback;
    pc_request(m_gate_client, request, route, msg, on_request_gate_cb);
    
    //main thread has nothing to do and wait until child thread return.
    //主线程中调用等待子线程的退出
    pc_client_join(m_gate_client);
    
    // release the client
    //此client在连接上Gate 然后回调requestGateCallback被调用后销毁
    pc_client_destroy(m_gate_client);
}

/*
 在回调函数中可以做一些ui操作 比如说隐藏等待框
 */
void NetServerEx::requestGateCallback(pc_request_t *req, int status, json_t *resp)
{
    CCLog("NetServerEx::requestGateCallback~~~");
    if(status == -1)
    {
        CCLog("Fail to send request to server.\n");
        
        s_loginFailed = true;
        Event* e = EventManager::sharedEventManager()->getEvent(EventTypeLogin);
        if (e != NULL)
        {
            EventManager::sharedEventManager()->notifyEventFailed(e);
        }
    }
    else if(status == 0)
    {
        const char *connectorHost = json_string_value(json_object_get(resp, "host"));//固定返回字段host port
        int connectorPort = json_number_value(json_object_get(resp, "port"));
        
        CCLog("Host is %s and port is %d", connectorHost, connectorPort);
        
        pc_client_t *client = pc_client_new();
        
        struct sockaddr_in address;
        
        memset(&address, 0, sizeof(struct sockaddr_in));
        address.sin_family = AF_INET;
        address.sin_port = htons(connectorPort);
        address.sin_addr.s_addr = inet_addr(connectorHost);
        
        // try to connect to server.
        if(pc_client_connect(client, &address)) {
            CCLog("fail to connect connector server.\n");
            pc_client_destroy(client);
            
            s_loginFailed = true;
            Event* e = EventManager::sharedEventManager()->getEvent(EventTypeLogin);
            if (e != NULL)
            {
                EventManager::sharedEventManager()->notifyEventFailed(e);
            }
            
            return ;
        }
        
        // add pomelo events listener
        /*
         暂时不增加事件监听
         注意 连接上connector的client就可以增加业务事件监听了
         void (*on_disconnect)(pc_client_t *client, const char *event, void *data) = &NetServerEx::onDisconnectCallback;
         void (*on_chat)(pc_client_t *client, const char *event, void *data) = &NetServerEx::onChatCallback;
         void (*on_add)(pc_client_t *client, const char *event, void *data) = &NetServerEx::onAddCallback;
         void (*on_leave)(pc_client_t *client, const char *event, void *data) = &NetServerEx::onLeaveCallback;
         
         pc_add_listener(client, "disconnect", on_disconnect);
         pc_add_listener(client, "onChat", on_chat);
         pc_add_listener(client, "onAdd", on_add);
         pc_add_listener(client, "onLeave", on_leave);
         
        void (*on_chat)(pc_client_t *client, const char *event, void *data) = &NetServerEx::onChatCallback;
        pc_add_listener(client, "onChat", on_chat);
         */
        
        const char *route = "connector.entryHandler.login";
        json_t *msg = json_object();
        json_t *username_str = json_string(s_username.c_str());
        json_t *password_str = json_string(s_password.c_str());
        json_object_set(msg, "username", username_str);
        json_object_set(msg, "rid", password_str);
        // decref for json object
        json_decref(username_str);
        json_decref(password_str);
        
        pc_request_t *request = pc_request_new();
        void (*connect_cb)(pc_request_t *req, int status, json_t *resp )= &NetServerEx::requestConnectorCallback;
        
        /**
         发起一个request请求
         int pc_request(pc_client_t *client, pc_request_t *req, const char *route, json_t *msg, pc_request_cb cb);
         */
        pc_request(client, request, route, msg, connect_cb);//没有销毁此client
    }
    
    // release relative resource with pc_request_t
    json_t *pc_msg = req->msg;
    json_decref(pc_msg);
    
    /*
     这个request请求实例里的pomelo client实例 是否为连接Gate的那个
     应该不是 感觉那个已经在上个方法requestGate中销毁了 还是先stop后再被销毁的?
     */
    pc_client_t *pc_client = req->client;
    /*
     停止客户端的连接
     该接口适合于在libuv子线程中调用，然后在主线程中，通过 pc_client_join来wait子线程退出(requestGate方法中)
     */
    pc_client_stop(pc_client);
    
    /*
     销毁一个request请求实例
     这个是否为requestGate中的pc_request_t 在1处
     */
    pc_request_destroy(req);
    
}

void NetServerEx::requestConnectorCallback(pc_request_t *req, int status, json_t *resp)
{
    if(status == -1)
    {
        CCLog("Fail to send request to server.\n");
        
        s_loginFailed = true;
        Event* e = EventManager::sharedEventManager()->getEvent(EventTypeLogin);
        if (e != NULL)
        {
            EventManager::sharedEventManager()->notifyEventFailed(e);
        }
        
    }
    else if(status == 0)
    {
        //resp应该是服务器端通过next方法传过来的json串
        /*
         char *json_str = json_dumps(resp, 0);
         CCLOG("server response: %s \n", json_str);
         json_t* users = json_object_get(resp,"users");
         if(json_object_get(resp, "error") != NULL) {
         CCLog("connect error %s", json_str);
         free(json_str);//回收char*的方法
         return;
         }*/
        
        int result = json_number_value(json_object_get(resp, "result"));
        if (result == 0)
        {
            /*
             应该是 连接上connector的pomelo client实例
             获取后就可以用来与服务器端正常通信了
             */
            pomelo_client = req->client;
            
            s_loginFailed = false;

            Event* e = EventManager::sharedEventManager()->getEvent(EventTypeLogin);
            if (e != NULL)
            {
                EventManager::sharedEventManager()->notifyEventSucceeded(e);
            }
        }
        else
        {
            pomelo_client = NULL;
            
            s_loginFailed = true;
            Event* e = EventManager::sharedEventManager()->getEvent(EventTypeLogin);
            if (e != NULL)
            {
                EventManager::sharedEventManager()->notifyEventFailed(e);
            }
        }
        
        
        
        
        
    }
    
    // release relative resource with pc_request_t
    json_t *msg = req->msg;
    json_decref(msg);
    /*
     由于是 连接上connector的pomelo client实例 所以这里没有pc_client_stop
     由此看来 requestGateCallback方法中的req->client 确实是连上Gate的pomelo client实例
     */
    pc_client_t *client = req->client;
    pc_request_destroy(req);
}

void NetServerEx::sendMsg(const char *route, json_t *msg)
{
    if (pomelo_client != NULL)
    {
        pc_request_t *request = pc_request_new();//request和msg没有销毁
        void (*on_send_cb)(pc_request_t * req, int status, json_t * resp) = sendMsgCallback;
        pc_request(pomelo_client, request, route, msg, on_send_cb);
    }
}

/*
 这个req确实是sendMsg函数中连接成功的request 所以应该销毁掉
 */
void NetServerEx::sendMsgCallback(pc_request_t *req, int status, json_t *resp)
{
    
    /*
     打印内容
     sendMsgCallback: {"rid": "channelname", "content": "Good Morning.", "from": "username", "target": "*"}
     */
    /*
     char *json_msg = json_dumps(req->msg, 0);
     CCLog("sendMsgCallback: %s \n", json_msg);
     */
    
    //是否应该销毁掉req和req->msg
    json_t *msg = req->msg;
    json_decref(msg);
    pc_request_destroy(req);
    
}

void NetServerEx::onChatCallback(pc_client_t *client, const char *event, void *data)
{
    json_t* json = (json_t* )data;
    const char* msg = json_dumps(json, 0);
    /*
     打印内容
     Cocos2d: onChat {"msg": "look", "from": "pet", "target": "username"}
     */
    CCLog("%s %s", event, msg);
    return;
}