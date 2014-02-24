//
//  SingleGameController.h
//  SnakeClient
//
//  Created by cy on 13-12-11.
//
//

#ifndef __SnakeClient__SingleGameController__
#define __SnakeClient__SingleGameController__

#include <iostream>
#include "GameController.h"

class SingleGameController : public GameController
{
public:
    SingleGameController();
    ~ SingleGameController();
    
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
    
private:
    //掷骰子逻辑
    int rollLogic();
    
};

#endif /* defined(__SnakeClient__SingleGameController__) */
