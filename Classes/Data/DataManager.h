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
#include "Answer.h"
#include "LocalUser.h"


#define ANSWER_NUM 8 //答案的个数
#define MAX_SILVER 999999 //最大银币值


class DataManager : public cocos2d::CCObject
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
    GameType getGameType();
    void setGameType(GameType gameType);
    
    int getLevel();
    void setLevel(int level);
    
    int getSingleSubLevel();
    void setSingleSubLevel(int singleSubLevel);
    
    bool getIsLogin();
    void setIsLogin(bool isLogin);
    
    //解析游戏数据 联网和单机共用
    void parseJson(json_t* gameDataJson);
    //随机设置单机成语的奖励
    void randomInitLocalWordBonus();
    
    std::vector<Grid*>& getGrids();
    void initGrids(std::vector<Grid*>& gridVector);
    
    std::vector<Words*>& getWords();
    void initWords(std::vector<Words*>& words);//把words引用给m_words 即是在NetServerEx中赋值 在此类中删除
    void clearWords();
    
    int getLine();
    int getCol();
    void setLine(int line);
    void setCol(int col);
    
    /*
     根据点击的索引获取被点击的Grid
     */
    Grid* getTouchGrid(int index);
    
    Grid* getTouchGridDirect();
    void setTouchGridDirect(Grid *grid);
    
    void setPhraseWordsIndexVec(std::vector<int>& phraseWordsIndexVec);
    std::vector<int>& getPhraseWordsIndexVec();
    
    void setPhrase2WordsIndexVec(std::vector<int>& phrase2WordsIndexVec);
    std::vector<int>& getPhrase2WordsIndexVec();
    
    void setIsVertical(bool isVertical);
    bool getIsVertical();
    
    //初始化候选答案
    void initAnswers(std::vector<std::string>& answers);
    std::vector<std::string>& getAnswers();
    
    void initAnswerDic();
    std::vector<std::string>& getAnswerDic();
    
    //初始化玩家选择的答案
    void initSelectAnswerVec(std::vector<Answer*>& selectAnswerVec);
    //使用了& 记得看是否出错
    std::vector<Answer*>& getSelectAnswerVec();
    
    
    //把正确的成语放进去
    void setRightWordsIndexVec(int index);
    //使用了& 记得看是否出错
    std::vector<int>& getRightWordsIndexVec();
    //每局游戏开始的时候需要调用
    void clearRightWordsIndexVec();
    
    bool isWin();
    
public:
    // -- 和网络有关
    void setUserUid(const std::string& userUid);
    std::string& getUserUid();
    
    void setUsername(const std::string& username);
    std::string& getUsername();
    
    //解析游戏实时数据
    void parseGameProcJson(json_t* gameProcJson);
    
    std::vector<int>& getChessVec();
    std::vector<int>& getOwnChessVec();
    
    
public:
    // -- 和策划数值有关的方法 会随着玩家的游戏而发生动态变化  比如本地玩家信息 联网玩家信息 成就等等
    
    //设置本地玩家信息
    void initLocalUser();
    LocalUser* getLocalUser();
    //更新本地玩家 不更新银币
    void updateLocalUser(int exp);
    //只更新本地玩家银币
    void updateLocalUserSilver(int silver);
    //获取本地奖励
    std::vector<int>& getLocalPassBonus();
    std::vector<int>& getLocalEveryBonus();
    
private:
    void clearGrids();
    void clearSelectAnswerVec();
    
private:
    GameType m_gameType;
    //单机 竞技或合作的难度 取值0 1 2
    int m_level;
    //单机关卡里面的 子关 0-...
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
    //候选答案字典 从中选择ANSWER_NUM-1个字
    std::vector<std::string> m_answerDic;
    
    //玩家选择的答案
    std::vector<Answer*> m_selectAnswerVec;
    
    //已经正确的成语索引
    std::vector<int> m_rightWordsIndexVec;
    
    //触摸点的index 和 TouchGridEvent里的变量有重复
    Grid *m_touchGrid;
    std::vector<int> m_phraseWordsIndexVec;//成语1所有字grid的索引
    std::vector<int> m_phrase2WordsIndexVec;//成语2所有字grid的索引
    //是否先点击了竖行 在手动点击的时候置为false
    bool m_isVertical;
    
    
private:
    // -- 和网络有关的变量
    
    //用户登录成功后赋于的uid
    std::string m_userUid;
    //用户名
    std::string m_username;
    
    //对手或整个棋盘数据
    std::vector<int> m_chessVec;
    /*
     自己的棋盘数据  形式如下
     "chess": [0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
     1 代表该成语已经完成
     */
    std::vector<int> m_ownChessVec;
    
    
private:
    // -- 和策划数值有关的变量 会随着玩家的游戏而发生动态变化  比如本地玩家信息 联网玩家信息 成就等等
    
    //本地玩家信息
    LocalUser* m_localUser;
    //本地游戏奖励
    json_t* m_localBonusJson;
    //本地  过关奖励 和 每个词语的奖励 解析m_localBonusJson获得 后期看能否优化 优化到只需要解析m_localBonusJson一次
    std::vector<int> m_localPassBonus;//先银币后经验
    std::vector<int> m_localEveryBonus;
};

#endif /* defined(__CrossWordClient__DataManager__) */
