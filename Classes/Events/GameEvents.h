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
    SitDownEvent(int gameType, int level) : Event(EventTypeSitDown)
    {
        m_gameType = gameType;
        m_level = level;
    }
    
    virtual ~ SitDownEvent()
    {
        
    }
    
    int getGameType() const
    {
        return m_gameType;
    }
    
    int getLevel() const
    {
        return m_level;
    }
    
private:
    int m_gameType;
    int m_level;
};

/**
 站起事件
 type 1-竞技 2-合作  暂时无用
 level 级别
 */
class SitUpEvent : public Event
{
public:
    SitUpEvent(int gameType, int level) : Event(EventTypeSitUp)
    {
        m_gameType = gameType;
        m_level = level;
    }
    
    virtual ~ SitUpEvent()
    {
        
    }
    
    int getGameType() const
    {
        return m_gameType;
    }
    
    int getLevel() const
    {
        return m_level;
    }
    
private:
    int m_gameType;
    int m_level;
};


/*
 点击格子事件
 */
class TouchGridEvent : public Event
{
public:
    TouchGridEvent() : Event(EventTypeTouchGrid)
    {
        m_index = -1;
    }
    
    virtual ~ TouchGridEvent()
    {
        
    }
    
    int getIndex() const
    {
        return m_index;
    }
    
    void setIndex(int index)
    {
        m_index = index;
    }
    
    void setWordsIndexVector(const std::vector<int>& wordsIndexVector)
    {
        m_wordsIndexVector = wordsIndexVector;
    }
    
    const std::vector<int>& getWordsIndexVector()
    {
        return m_wordsIndexVector;
    }
    
private:
    int m_index;
    //横竖词语的索引vector
    std::vector<int> m_wordsIndexVector;
    
};

class ChooseAnswerEvent : public Event
{
public:
    ChooseAnswerEvent() : Event(EventTypeChooseAnswer)
    {
        m_index = -1;
    }
    
    virtual ~ ChooseAnswerEvent()
    {
        
    }
    
    int getIndex() const
    {
        return m_index;
    }
    
    void setIndex(int index)
    {
        m_index = index;
    }
    
private:
    
    int m_index;
    
};

class FixAnswerEvent : public Event
{
public:
    FixAnswerEvent() : Event(EventTypeFixAnswer)
    {
        m_phraseIndex = -1;
        m_phrase2Index = -1;
    }
    
    virtual ~ FixAnswerEvent()
    {
        
    }
    
    int getPhraseIndex() const
    {
        return m_phraseIndex;
    }
    
    void setPhraseIndex(int phraseIndex)
    {
        m_phraseIndex = phraseIndex;
    }
    
    int getPhrase2Index() const
    {
        return m_phrase2Index;
    }
    
    void setPhrase2Index(int phrase2Index)
    {
        m_phrase2Index = phrase2Index;
    }
    
private:
    int m_phraseIndex;//匹配的词语的索引
    int m_phrase2Index;
    
};

/*
 有以下几种奖励的方式 m_bonusType 为
 1 倒计时奖励的银币 或 经验 1 或 2
 2 完成一个成语奖励银币和经验 3
 3 关卡过关奖励的银币和经验 4
 */
class RewardEvent : public Event
{
public:
    RewardEvent(int bonusType) : Event(EventTypeReward)
    {
        m_bonusType = bonusType;
        
        m_isWin = false;
        m_bonusSilver = 0;
        m_bonusExp = 0;
    }
    
    virtual ~ RewardEvent()
    {
        
    }
    
    bool getIsWin() const
    {
        return m_isWin;
    }
    
    int getBonusType() const
    {
        return m_bonusType;
    }
    
    int getBonusSilver() const
    {
        return m_bonusSilver;
    }
    
    int getBonusExp() const
    {
        return m_bonusExp;
    }
    
    void setIsWin(bool isWin)
    {
        m_isWin = isWin;
    }
    
    void setBonusSilver(int bonusSilver)
    {
        m_bonusSilver = bonusSilver;
    }
    
    void setBonusExp(int bonusExp)
    {
        m_bonusExp = bonusExp;
    }

private:
    bool m_isWin;
    int m_bonusType;
    int m_bonusSilver;
    int m_bonusExp;
    
};

/*
 联网的游戏停止消息  相当于单机的RewardEvent消息m_bonusType为4的情况
 对于联网的游戏停止消息 客户端只需要执行显示逻辑 不需要进行例如加金币经验等逻辑
 flag 
 0 － 一个人或者两个人赢了
 1 － 合作模式时间到
 2 － 游戏中途对手退出 游戏停止
 
 0 1 种情况  弹出结算框   提示谁赢谁输   点击确定进入下一次等待  取消回到对应的竞技合作难度选择界面
 
 2 种情况 （需要知道谁退出）
 对于非退出方  如果是竞技模式 弹出结算框 提示对手逃跑取得胜利  给目前取得的奖励
 如果是合作模式  弹出提示框  是否需要继续 如果继续 胜利则给过关奖励 不过关只给当前奖励    如果不继续  只给当前奖励   （或者是不弹出提示框 ， 只是告知伙伴逃跑 ，然后等流程结束再按正常流程走）
 
 对于退出方  需要根据不同情况进行处理
 1）主动退出 则回到难度选择界面
 2）掉线退出 ？？？
 */
class GameStopEvent : public Event
{
public:
    GameStopEvent(int flag) : Event(EventTypeGameStop)
    {
        m_flag = flag;
        
        m_isWin = false;
    }
    
    virtual ~ GameStopEvent()
    {
        
    }
    
    bool getIsWin() const
    {
        return m_isWin;
    }
    
    int getFlag() const
    {
        return m_flag;
    }
    
    void setIsWin(bool isWin)
    {
        m_isWin = isWin;
    }
    
    void setFlag(int flag)
    {
        m_flag = flag;
    }
    
private:
    bool m_isWin;
    int m_flag;
    
};

class GameStopEventEx : public Event
{
public:
    GameStopEventEx(int flag, bool isWin = false) : Event(EventTypeGameStopEx)
    {
        m_flag = flag;
        m_isWin = isWin;
    }
    
    virtual ~ GameStopEventEx()
    {
        
    }
    
    bool getIsWin() const
    {
        return m_isWin;
    }
    
    int getFlag() const
    {
        return m_flag;
    }
    
    void setIsWin(bool isWin)
    {
        m_isWin = isWin;
    }
    
    void setFlag(int flag)
    {
        m_flag = flag;
    }
    
private:
    bool m_isWin;
    int m_flag;
    
};


class DisconnectEvent : public Event
{
public:
    //m_pomeloType 1-PomeloLogin 2-PomeloGame
    DisconnectEvent(int pomeloType) : Event(EventTypeDisconnect)
    {
        m_pomeloType = pomeloType;
    }
    
    virtual ~ DisconnectEvent()
    {
        
    }
    
    int getPomeloType() const
    {
        return m_pomeloType;
    }
    
private:
    int m_pomeloType;
};

class DisconnectEventEx : public Event
{
public:
    //m_pomeloType 1-PomeloLogin 2-PomeloGame
    DisconnectEventEx(int pomeloType) : Event(EventTypeDisconnectEx)
    {
        m_pomeloType = pomeloType;
    }
    
    virtual ~ DisconnectEventEx()
    {
        
    }
    
    int getPomeloType() const
    {
        return m_pomeloType;
    }
    
private:
    int m_pomeloType;
};


class GetInfoEvent : public Event
{
public:
    
    GetInfoEvent(int uid) : Event(EventTypeGetInfo)
    {
        m_uid = uid;
    }
    
    virtual ~ GetInfoEvent()
    {
        
    }
    
    int getUid() const
    {
        return m_uid;
    }
    
private:
    int m_uid;
};


class UpdateInfoEvent : public Event
{
public:
    
    UpdateInfoEvent(int uid) : Event(EventTypeUpdateInfo)
    {
        m_uid = uid;
    }
    
    virtual ~ UpdateInfoEvent()
    {
        
    }
    
    int getUid() const
    {
        return m_uid;
    }
    
private:
    int m_uid;
};

class UpdateInfoEventEx : public Event
{
public:
    
    UpdateInfoEventEx(int uid) : Event(EventTypeUpdateInfoEx)
    {
        m_uid = uid;
    }
    
    virtual ~ UpdateInfoEventEx()
    {
        
    }
    
    int getUid() const
    {
        return m_uid;
    }
    
private:
    int m_uid;
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
