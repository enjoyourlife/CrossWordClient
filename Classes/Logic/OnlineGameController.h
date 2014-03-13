//
//  OnlineGameController.h
//  SnakeClient
//
//  Created by cy on 13-12-11.
//
//

#ifndef __SnakeClient__OnlineGameController__
#define __SnakeClient__OnlineGameController__

#include <iostream>
#include "GameController.h"
#include "../Events/EventObserver.h"
#include "../CommonUI/CGWaiting.h"

class OnlineGameController : public GameController, public EventObserver
{
public:
    OnlineGameController();
    ~ OnlineGameController();
    
public:
	/*!
     * @brief		开始执行某事件。
     * @param       type	事件类型
	 * @param		data	数据
     */
    virtual void startEvent(int type, void* data);
	
    /*!
     * @brief		开始执行某事件。
     * @param       event   事件
     */
    virtual void startEvent(Event* event);
	
	/*!
     * @brief		取消正在执行的事件。
     * @param       event   事件
     */
	virtual void cancelEvent(Event* event);
    

    
    /*!
     * controller作为EventObserver 主要执行一些界面切换的操作  而scenes比如hall作为EventObserver 主要执行一些改变自身界面的操作
     * @brief		队列事件成功时的回调。
     * @param       event   事件
     */
    virtual void onEventSucceeded(Event* event);
    
    /*!
     * @brief		队列事件失败时的回调。
     * @param       event   事件
     */
    virtual void onEventFailed(Event *event);
    
private:
    //进入竞技场或者合作场
    void enterCompOrCoopGame(Event* event);
    
    CGWaiting* m_waiting;
};

#endif /* defined(__SnakeClient__OnlineGameController__) */
