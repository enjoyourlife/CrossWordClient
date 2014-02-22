//
//  NetServer.cpp
//  CrossWordClient
//
//  Created by cy on 14-2-20.
//
//

#include "NetServer.h"


USING_NS_CC;


#define GATE_HOST "192.168.0.2"
#define GATE_PORT 3014

static NetServer* s_netServer = NULL;
static pc_client_t* pomelo_client = NULL;

NetServer* NetServer::sharedNetServer()
{
	if (s_netServer == NULL)
	{
		s_netServer = new NetServer;
	}
	return s_netServer;
}

NetServer::NetServer()
{
    requestGate();
}

NetServer::~NetServer()
{
	
}


void NetServer::requestGate()
{
    CCLog("NetServer::requestGate~~~");
    
    const char *ip = GATE_HOST;
    int port = GATE_PORT;
    
    pc_client_t *client = pc_client_new();//创建一个新的pomelo client实例
    
    struct sockaddr_in address;
    
    memset(&address, 0, sizeof(struct sockaddr_in));
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = inet_addr(ip);
    
    //连接到服务器，在连接过程中会创建子线程用于处理网络I/O
    if(pc_client_connect(client, &address))
    {
        CCLog("fail to connect gate server.\n");
        pc_client_destroy(client);//销毁客户端的连接
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
    void (*on_request_gate_cb)(pc_request_t *req, int status, json_t *resp) = &NetServer::requestGateCallback;
    pc_request(client, request, route, msg, on_request_gate_cb);
    
    //main thread has nothing to do and wait until child thread return.
    //主线程中调用等待子线程的退出
    pc_client_join(client);
    
    // release the client
    //此client在连接上Gate 然后回调requestGateCallback被调用后销毁
    pc_client_destroy(client);
}

void NetServer::requestGateCallback(pc_request_t *req, int status, json_t *resp)
{
    CCLog("NetServer::requestGateCallback~~~");
    if(status == -1)
    {
        CCLog("Fail to send request to server.\n");
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
            return ;
        }
        
        // add pomelo events listener
        /* 
         暂时不增加事件监听
         注意 连接上connector的client就可以增加业务事件监听了
        void (*on_disconnect)(pc_client_t *client, const char *event, void *data) = &NetServer::onDisconnectCallback;
        void (*on_chat)(pc_client_t *client, const char *event, void *data) = &NetServer::onChatCallback;
        void (*on_add)(pc_client_t *client, const char *event, void *data) = &NetServer::onAddCallback;
        void (*on_leave)(pc_client_t *client, const char *event, void *data) = &NetServer::onLeaveCallback;
        
        pc_add_listener(client, "disconnect", on_disconnect);
        pc_add_listener(client, "onChat", on_chat);
        pc_add_listener(client, "onAdd", on_add);
        pc_add_listener(client, "onLeave", on_leave);
        */
        void (*on_chat)(pc_client_t *client, const char *event, void *data) = &NetServer::onChatCallback;
        pc_add_listener(client, "onChat", on_chat);
        
        const char *route = "connector.entryHandler.enter";
        json_t *msg = json_object();
        json_t *str = json_string("username");
        json_t *channel_str = json_string("channelname");
        json_object_set(msg, "username", str);
        json_object_set(msg, "rid", channel_str);
        // decref for json object
        json_decref(str);
        json_decref(channel_str);
        
        pc_request_t *request = pc_request_new();
        void (*connect_cb)(pc_request_t *req, int status, json_t *resp )= &NetServer::requestConnectorCallback;
        
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

void NetServer::requestConnectorCallback(pc_request_t *req, int status, json_t *resp)
{
    if(status == -1)
    {
        CCLog("Fail to send request to server.\n");
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
        
        /*
         应该是 连接上connector的pomelo client实例
         获取后就可以用来与服务器端正常通信了
         */
        pomelo_client = req->client;
        
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

void NetServer::sendMsg(const char *route, json_t *msg)
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
void NetServer::sendMsgCallback(pc_request_t *req, int status, json_t *resp)
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

void NetServer::onChatCallback(pc_client_t *client, const char *event, void *data)
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
