//
//  DataManager.h
//  CrossWordClient
//
//  Created by cy on 14-2-28.
//
//

#ifndef __CrossWordClient__DataManager__
#define __CrossWordClient__DataManager__

#include <iostream>
#include "cocos2d.h"
#include "jansson.h"
#include "../Events/EventManager.h"
#include "../Events/EventObserver.h"
#include "Grid.h"
#include "Words.h"


#define ANSWER_NUM 8 //答案的个数


class DataManager : public cocos2d::CCObject, public EventObserver
{
private:
    DataManager();
    ~DataManager();
    
    void init();
    
public:
    /*!
     * @brief		获取一个DataManager单例。
     */
    static DataManager* sharedDataManager();
    
public:
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
    
public:
    GameType getGameType();
    void setGameType(GameType gameType);
    
    int getLevel();
    void setLevel(int level);
    
    int getSingleSubLevel();
    void setSingleSubLevel(int singleSubLevel);
    
    bool getIsLogin();
    void setIsLogin(bool isLogin);
    
    std::vector<Grid*> getGrids();
    void initGrids(std::vector<Grid*>& gridVector);
    
    std::vector<Words*> getWords();
    void initWords(std::vector<Words*>& words);//把words引用给m_words 即是在NetServerEx中赋值 在此类中删除
    void clearWords();
    
    int getLine();
    int getCol();
    void setLine(int line);
    void setCol(int col);
    
    /*
     根据点击的索引获取被点击的Grid
     */
    Grid* getClickGrid(int index);
    
    //初始化候选答案
    void initAnswers(std::vector<std::string>& answers);
    std::vector<std::string> getAnswers();
    
private:
    void clearGrids();
    
    
private:
    GameType m_gameType;
    //竞技或合作的难度 取值0 1 2
    int m_level;
    //单机子关 0-easy 1-normal 2-hard
    int m_singleSubLevel;
    //是否已经登录
    bool m_isLogin;
    //所有方框数据
    std::vector<Grid*> m_grids;
    //文字和解析数据
    std::vector<Words*> m_words;
    
    //行列
    int m_line;
    int m_col;


    //候选答案
    std::vector<std::string> m_answers;
};

#endif /* defined(__CrossWordClient__DataManager__) */
