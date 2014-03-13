//
//  DataManager.h
//  CrossWordClient
//
//  Created by cy on 14-2-28.
//
//

#ifndef __CrossWordClient__DataManager__
#define __CrossWordClient__DataManager__

#include <iostream>
#include "cocos2d.h"
#include "../Events/EventManager.h"
#include "../Events/EventObserver.h"

class DataManager : public cocos2d::CCObject, public EventObserver
{
private:
    DataManager();
    ~DataManager();
    
    void init();
    
public:
    /*!
     * @brief		获取一个DataManager单例。
     */
    static DataManager* sharedDataManager();
    
public:
    /*!
     * @brief		队列事件成功时的回调。
     * @param       event   事件
     */
    virtual void onEventSucceeded(Event* event);
    
    /*!
     * @brief		队列事件失败时的回调。
     * @param       event   事件
     */
    virtual void onEventFailed(Event* event);
    
public:
    GameType getGameType();
    void setGameType(GameType gameType);
    
    int getSingleSubLevel();
    void setSingleSubLevel(int singleSubLevel);
    
    bool getIsLogin();
    void setIsLogin(bool isLogin);
private:
    GameType m_gameType;
    //单机子关 0-easy 1-normal 2-hard
    int m_singleSubLevel;
    //是否已经登录
    bool m_isLogin;
};

#endif /* defined(__CrossWordClient__DataManager__) */
