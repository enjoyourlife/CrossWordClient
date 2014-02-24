//
//  EventObserver.h
//  SnakeClient
//
//  Created by cy on 13-12-11.
//
//

#ifndef __SnakeClient__EventObserver__
#define __SnakeClient__EventObserver__

#include <iostream>

class Event;

/*!
 * @brief	事件观察者。
 * @ingroup	Events
 *
 * 观察者负责监听事件，收到事件通知后更新自身状态。
 * 与传统observer模式不同，观察者不单是偷窥事件，而是可对所监听的队列事件表示观注，
 * 从而影响事件的处理流程和生命周期。
 */
class EventObserver
{
public:
    /*!
     * @brief		立即事件成功时的回调。
     * @param       event   事件ID
     * @param       data    事件传入的数据
     */
    virtual void onEventSucceeded(int event, void* data);
    
    /*!
     * @brief		立即事件失败时的回调。
     * @param       event   事件ID
     * @param       data    事件传入的数据
     */
    virtual void onEventFailed(int event, void* data);
    
    /*!
     * @brief		队列事件开始时的回调。
     * @param       event   事件
     */
    virtual void onEventStarted(Event* event);
    
    /*!
     * @brief		取消队列事件时的回调。
     * @param       event   事件
     */
    virtual void onEventCancelled(Event* event);
    
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
};


#endif /* defined(__SnakeClient__EventObserver__) */
