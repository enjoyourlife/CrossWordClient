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
#include "../Events/EventObserver.h"

class SingleGameController : public GameController, public EventObserver
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
    //解析关卡数据
    void parseJson();
    
    /*
     处理本地玩家奖励 发送RewardEvent事件
     由于判断词语正确与否的逻辑是放在本地的 
     所以 单机奖励在EventTypeFixAnswer事件成功后判断给出
     联网奖励却应该由服务器给出 应该在onGameProc里或其他回调里获取 然后再发送一个RewardEvent事件
     */
    void localUserBonus(int phraseIndex, int phrase2Index);
    void localUserBonusByPhraseIndex(int phraseIndex);
    
    //处理RewardEvent奖励事件
    void handleLocalUserRewardEvent(Event *event);
};

#endif /* defined(__SnakeClient__SingleGameController__) */
