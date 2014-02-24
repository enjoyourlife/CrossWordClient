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
