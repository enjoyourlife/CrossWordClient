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

class OnlineGameController : public GameController
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
};

#endif /* defined(__SnakeClient__OnlineGameController__) */
