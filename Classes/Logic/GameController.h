//
//  GameController.h
//  SnakeClient
//
//  Created by cy on 13-12-11.
//
//

#ifndef __SnakeClient__GameController__
#define __SnakeClient__GameController__

#include <iostream>
#include "cocos2d.h"
#include "../Data/Grid.h"



class Event;

class GameController : public cocos2d::CCObject
{
protected:
    GameController();
    virtual ~ GameController();

public:
	/*!
     * @brief		开始执行某事件。
     * @param       type	事件类型
	 * @param		data	数据
     */
    virtual void startEvent(int type, void* data) = 0;
	
    /*!
     * @brief		开始执行某事件。
     * @param       event   事件
     */
    virtual void startEvent(Event* event) = 0;
	
	/*!
     * @brief		取消正在执行的事件。
     * @param       event   事件
     */
	virtual void cancelEvent(Event* event) = 0;

protected:
    //设置候选答案
    void initAnswers(Grid *grid);
    //处理TouchGridEvent事件
    void handleTouchGridEvent(Event* event);
    

};

#endif /* defined(__SnakeClient__GameController__) */
