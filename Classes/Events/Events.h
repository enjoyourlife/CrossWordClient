//
//  Events.h
//  SnakeClient
//
//  Created by cy on 13-12-11.
//
//

#ifndef __SnakeClient__Events__
#define __SnakeClient__Events__

#include <iostream>
#include <list>
#include <string>

//和EventObserver循环引用 所以这样包含
class EventObserver;

/*!
 * @brief	事件状态。
 * @ingroup	Events
 */
typedef enum
{
    EventStateInit,         // 尚未开始
    EventStateStarted,      // 已开始，尚未完成
    EventStateDone,         // 已完成，尚未通知观察者
    EventStateFailed,       // 已失败，尚未通知观察者
    EventStateNotified,     // 已通知观察者
}
EventState;


/*!
 * @brief	事件基类。
 * @ingroup	Events
 * @see     EventObserver
 *
 * 事件分四个阶段：开始，成功/失败，通知，结束
 * 事件开始后，由EventManager进行处理，直到成功或失败，然后将事件通知给观察者。
 * 观察者收到通知后，可以选择是否对事件进行关注。当所有观察者都取消关注后，
 * 此事件便彻底宣告结束。
 */
class Event
{
public:
    Event(int type);
    Event(int type, int tag);
    virtual ~ Event();
    
public:
	/*!
     * @brief		获取事件类型。
     */
    int getType() const;
	
    /*!
     * @brief		获取事件状态。
     */
    int getState() const;
    
    /*!
     * @brief		设置事件状态。
     */
    void setState(int state);
    
    /*!
     * @brief		获取事件标志。
     */
    int getTag() const;
    
    /*!
     * @brief		设置事件标志。
     */
    void setTag(int tag);
    
    /*!
     * @brief		获取事件标志。
     */
    int getErrorCode() const;
    
    /*!
     * @brief		设置事件标志。
     */
    void setErrorCode(int errorCode);
    
    /*!
     * @brief		设置事件信息标志。
     */
    void setTips(const std::string& tips);
    
    /*!
     * @brief		获取事件标志。
     */
    std::string& getTips();
    /*!
     * @brief		添加关注此事件的观察者。
     */
    void addFocus(EventObserver* observer);
    
    /*!
     * @brief		移除曾关注此事件的观察者。
     */
    void removeFocus(EventObserver* observer);
    
    /*!
     * @brief		清空关注者列表。
     */
    void clearFocus();
	
	/*!
     * @brief		判断某观察者是否对此事件表示关注。
	 * @param		observer	观察者
     */
	bool hasFocus(EventObserver* observer) const;
    
    /*!
     * @brief		判断是否有任何一个观察者对此事件表示关注。
     */
    bool hasFocus() const;
    
protected:
    /*!
     * @brief		事件类型。
     */
    int m_type;
    
    /*!
     * @brief		事件标志，以区分相同类型的多个事件。
     */
    int m_tag;
    
    /*!
     * @brief		事件状态。
     */
    int m_state;
    
    /*!
     * @brief		错误码。
     */
    int m_errorCode;
    
    /*!
     * @brief	    事件信息。
     */
    std::string m_tips;
    
    /*!
     * @brief		对此事件表示关注的观察者。
     */
    std::list<EventObserver*> m_observers;
};

#endif /* defined(__SnakeClient__Events__) */
