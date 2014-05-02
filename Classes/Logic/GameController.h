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
    
    void handleChooseAnswerEvent(Event *event);
    //检查答案是否为正确答案 每选一个字就检查一次
    bool checkAnswerFix(int phraseIndex, int phrase2Index);
    //检查游戏是否胜利
    bool checkGameWin();
    
    //自动寻找下一次点击的Grid -1为没有下一次点击Grid
    /*
     选择完答案后，接着自动选择下一个字的逻辑
     1.如果玩家点的是交叉字  如果横方向尚未显示正确字 则自动选择横方向的下一个答案字   否则选择纵方向的下一个答案字
     2.如果是先选择纵行第一个字  依次自动选择的时候  遇到交叉点  还是以纵行为主  自动选择纵行的下一个字
     3.自动选择下一个答案字  象一个环形一样选择  比如玩家选择了第二个字  下一个是第三个 然后绕一圈回到第一个字
     4.在自动选择下一个答案字的时候  如果遇到已经有正确的字的时候  要自动跳到下一个
     */
    int autoFindNextTouchGrid(int touchGridIndex, int phraseIndex, int phrase2Index);

};

#endif /* defined(__SnakeClient__GameController__) */
