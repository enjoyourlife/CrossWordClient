//
//  PomeloServer.cpp
//  CrossWordClient
//
//  Created by cy on 14-5-4.
//
//

#include "PomeloServer.h"
#include <curl/curl.h>

USING_NS_CC;
using namespace std;


PomeloServer::PomeloServer()
{
    m_ip = "";
    m_isInit = false;
}

PomeloServer::~PomeloServer()
{
	
}

size_t process_data(void *buffer, size_t size, size_t nmemb, void *user_p)
{
    string *ip = (string*)user_p;
    ip->append((char*)buffer, size * nmemb);
    
    return (size * nmemb);
}

void PomeloServer::getIpByHost(const char* host)
{
    CURL* curl;
	CURLcode res;
	curl=curl_easy_init();
	if(curl)
	{
		// 注意加随机数或处理刷新的问题
		curl_easy_setopt(curl,CURLOPT_URL,host);
		curl_easy_setopt(curl,CURLOPT_TIMEOUT,5);
		curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION, process_data);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &m_ip);
		res=curl_easy_perform(curl);
		curl_easy_cleanup(curl);
		if(res!=CURLE_OK)
		{
            m_ip = "";
			CCLog("http request error: %i",res);//http request error: 7错误
			return;
		}
        
        CCLog("m_ip is : %s \n",m_ip.c_str());
	}
	else
	{
        m_ip = "";
		return;
	}
}

bool PomeloServer::init(const char* host, u_short port)
{
    this->getIpByHost(host);//只要调用一次即可
//    m_ip = "192.168.0.2";
    if (m_ip.compare("") != 0)
    {
        m_client = pc_client_new();
        
        struct sockaddr_in address;
        
        memset(&address, 0, sizeof(struct sockaddr_in));
        address.sin_family = AF_INET;
        address.sin_port = htons(port);
        address.sin_addr.s_addr = inet_addr(m_ip.c_str());
        
        // try to connect to server.
        if(pc_client_connect(m_client, &address)) {
            CCLog("fail to connect server.\n");
            pc_client_destroy(m_client);
            m_client = NULL;
            return false;
        }
        
//        pc_add_listener(m_client, PC_EVENT_DISCONNECT, PomeloServer::onDisconnect);
        CCLog("success to connect server: %s \n",m_ip.c_str());
        return true;
    }
    else
    {
        return false;
    }
}

void PomeloServer::request(const char* route, json_t *msg, pc_request_cb cb)
{
    if (m_client != NULL)
    {
        pc_request_t *request = pc_request_new();
        pc_request(m_client, request, route, msg, cb);
    }
}

void PomeloServer::reset()
{
    /* crash
     pc_client_destroy(m_client);
     */
    m_client = NULL;
    
    m_isInit = false;
    m_ip = "";//需要 否则m_ip is : 192.168.0.2192.168.0.2192.168.0.2192.168.0.2192.168.0.2 就重叠了

}
