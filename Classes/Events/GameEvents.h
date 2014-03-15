//
//  GameEvents.h
//  SnakeClient
//
//  Created by cy on 13-12-21.
//
//

#ifndef __SnakeClient__GameEvents__
#define __SnakeClient__GameEvents__

#include <iostream>
#include "Events.h"
#include "EventManager.h"

/*!
 * @brief		登录游戏事件 
 m_loginType
 0-外面大厅点登录
 1-点 GameTypeCompetitive 竞技模式进行登录
 2-点 GameTypeCooperation 合作模式进行登录
 */
class LoginEvent : public Event
{
public:
    LoginEvent(int loginType, const std::string username, const std::string password) : Event(EventTypeLogin)
    {
        m_loginType = loginType;
        m_username = username;
        m_password = password;
    }
    
    virtual ~ LoginEvent()
    {
        
    }
    
    int getLoginType() const
    {
        return m_loginType;
    }
    
    std::string getUsername() const
    {
        return m_username;
    }
    
    std::string getPassword() const
    {
        return m_password;
    }
    
    
private:
    int m_loginType;
    std::string m_username;
    std::string m_password;
};


/*
 LoginEvent的拷贝事件 为了重复发一次
 */
class LoginEventEx : public Event
{
public:
    LoginEventEx(int loginType, const std::string username, const std::string password) : Event(EventTypeLoginEx)
    {
        m_loginType = loginType;
        m_username = username;
        m_password = password;
    }
    
    virtual ~ LoginEventEx()
    {
        
    }
    
    int getLoginType() const
    {
        return m_loginType;
    }
    
    std::string getUsername() const
    {
        return m_username;
    }
    
    std::string getPassword() const
    {
        return m_password;
    }
    
    
private:
    int m_loginType;
    std::string m_username;
    std::string m_password;
};

/**
 坐下事件
 type 1-竞技 2-合作  暂时无用
 level 级别
 */
class SitDownEvent : public Event
{
public:
    SitDownEvent(int type, int level) : Event(EventTypeSitDown)
    {
        m_type = type;
        m_level = level;
    }
    
    virtual ~ SitDownEvent()
    {
        
    }
    
    int getType() const
    {
        return m_type;
    }
    
    int getLevel() const
    {
        return m_level;
    }
    
private:
    int m_type;
    int m_level;
};

/*!
 * @brief		进入游戏事件，带有起始玩家的人数。
 */
class EnterGameEvent : public Event
{
public:
    EnterGameEvent(int playerNumber, int npcNumber) : Event(EventTypeEnterGame)
    {
        m_playerNumber = playerNumber;
        m_npcNumber = npcNumber;
    }
    
    virtual ~ EnterGameEvent()
    {
        
    }
    
    int getPlayerNumber() const
    {
        return m_playerNumber;
    }
    
    int getNpcNumber() const
    {
        return m_npcNumber;
    }
    
private:
    int m_playerNumber;
    int m_npcNumber;
};


/*!
 * @brief		掷骰子事件。
 */
class RollEvent : public Event
{
public:
    RollEvent(int rollPlayerPos, int point) : Event(EventTypeRoll)
    {
        m_rollPlayerPos = rollPlayerPos;
        m_point = point;
    }
    
    virtual ~ RollEvent()
    {
        
    }
    
    int getRollPlayerPos() const
    {
        return m_rollPlayerPos;
    }
    
    int getPoint() const
    {
        return m_point;
    }
    
    void setPoint(int point)
    {
        m_point = point;
    }
    
private:
    int m_rollPlayerPos;
    int m_point;
};

#endif /* defined(__SnakeClient__GameEvents__) */
