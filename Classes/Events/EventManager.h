//
//  EventManager.h
//  SnakeClient
//
//  Created by cy on 13-12-11.
//
//

#ifndef __SnakeClient__EventManager__
#define __SnakeClient__EventManager__

#include <iostream>
#include <list>
#include "cocos2d.h"
#include "Events.h"
#include "EventObserver.h"
#include "../Logic/GameController.h"

/*!
 * @brief		游戏类型：单机，联网，比赛。
 */
typedef enum
{
    GameTypeSingle = 0,       // 单机
    GameTypeCompetitive = 1,             // 竞技
    GameTypeCooperation = 2,              // 合作
}
GameType;

typedef enum
{
    EventTypeLogin,         // 登录
    EventTypeLoginEx,
    EventTypeEnterHall, //进入大厅 
    EventTypeEnterSingleGame, //进入单机房间
    EventTypeEnterCompetitiveGame,
    EventTypeEnterCooperationGame,
    EventTypeEnterSingleSubGame, //进入单机子关
    EventTypeSitDown,//坐下
    EventTypeSitUp,//站起
    EventTypeGameStart,//游戏开始
    EventTypeGameStartEx,
    
    
    EventTypeEnterGame,
    EventTypeRoll,//掷骰子
    EventTypeWin,//检测是否有赢家
}
EventType;

typedef enum
{
    ErrorCodeLogin,
}
ErrorCodeType;

/*!
 * @brief	事件管理器。
 * @ingroup Events
 *
 * 事件分为立即事件和队列事件，立即事件马上得到执行，而队列事件需序列化执行
 */
class EventManager : public cocos2d::CCObject
{
protected:
    EventManager();
    virtual ~ EventManager();
    
public:
    /*!
     * @brief		获取一个EventManager单例。
     */
    static EventManager* sharedEventManager();
    
private:
	/*!
     * @brief		初始化。
     */
    virtual bool init();
    
	/*!
     * @brief		在此方法中不断检查并处理事件队列中的事件。
     */
    virtual void update(float t);
    
public:
    /*!
     * @brief		将事件加入到队列中。
     * @param       event   事件对象
     */
    virtual void addEvent(Event* event);
    
    /*!
     * @brief		取消某队列事件。
     * @param       event   事件对象
     */
    virtual void cancelEvent(Event* event);
    
    /*!
     * @brief		通知所有观察者某队列事件成功。此后可能还需要其它处理（例如播放动画）才能彻底完成此事件。
     * @param       event   事件对象
     */
    virtual void notifyEventSucceeded(Event* event);
    
    /*!
     * @brief		通知所有观察者某事件失败。
     * @param       event   事件对象
     */
    virtual void notifyEventFailed(Event* event);
    
    /*!
     * @brief		根据事件类型获取队列中对应的事件。
     * @param       type    事件类型
     * @param       tag     事件标志
     */
    Event* getEvent(int type, int tag = -1);
	
	/*!
     * @brief		获取队列中正在执行的事件。
     */
    Event* getActiveEvent() const;
	
	/*!
     * @brief		立即执行某事件（不加入到队列）
     * @param       type    事件类型
     * @param       data    数据参数
     */
	virtual void doEvent(int type, void* data = NULL);
	
	/*!
     * @brief		通知所有观察者某立即事件成功。
     * @param       event   事件ID
     * @param       data    传入的参数
     */
    virtual void notifyEventSucceeded(int event, void* data = NULL);
    
    /*!
     * @brief		通知所有观察者某立即事件失败。
     * @param       event   事件ID
     * @param       data    传入的参数
     */
    virtual void notifyEventFailed(int event, void* data = NULL);
    
    /*!
     * @brief		删除队列中所有的事件
     */
    void removeAllEvents();
    
    /*!
     * @brief		添加观察者。
     * @param       observer   观察者
     */
    void addObserver(EventObserver* observer);
    
    /*!
     * @brief		删除观察者。
     * @param       observer   观察者
     */
    void removeObserver(EventObserver* observer);
    
    /*!
     * @brief		观察者对此事件表示严重关注。
     * @param       observer    表示关注的观察者
     * @param       event       事件
     */
    virtual void addFocusOnEvent(EventObserver* observer, Event* event);
    
    /*!
     * @brief		取消对此事件的关注。
     * @param       observer    曾经表示关注的观察者
     * @param       event       事件
     */
    virtual void removeFocusOnEvent(EventObserver* observer, Event* event);
    
    /*!
     * @brief		取消对此事件的关注。
     * @param       observer    曾经表示关注的观察者
     * @param       type        事件类型
     * @param       tag         事件标记
     */
    virtual void removeFocusOnEvent(EventObserver* observer, int type, int tag = -1);
    
    /*!
     * @brief		获取当前队列中的事件数量。
     */
    int getEventNumber();
    
private:
    /*!
     * @brief		根据当前的游戏类型获取游戏控制器（单机，联网或比赛）
     */
    GameController* getGameController();
    
    /*!
     * @brief		处理当前事件
     */
    void process();
    
    /*!
     * @brief		根据事件类型获取事件名称（便于调试）。
     * @param       type    事件类型
     * @return      事件名称
     */
    const char* getEventName(int type) const;
    
public:
    /*!
     * @brief		打印队列中的所有事件
     */
    void printEvents();
    
private:
    /*!
     * @brief		事件列表。
     */
    std::list<Event*> m_events;
    
    /*!
     * @brief		观察者列表。
     */
    std::list<EventObserver*> m_observers;
    
    /*!
     * @brief		联网游戏事件控制器。
     */
    GameController* m_onlineController;
    
    /*!
     * @brief		单机游戏事件控制器。
     */
    GameController* m_singleController;
};

#endif /* defined(__SnakeClient__EventManager__) */
