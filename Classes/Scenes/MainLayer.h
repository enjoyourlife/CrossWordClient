//
//  MainLayer.h
//  CrossWordClient
//
//  Created by cy on 14-2-22.
//
//

#ifndef __CrossWordClient__MainLayer__
#define __CrossWordClient__MainLayer__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "../CommonUI/CGControlButton.h"
#include "../Events/EventManager.h"
#include "../Events/EventObserver.h"
#include "MainBorderLayer.h"
#include "../Data/Words.h"
#include "../CommonUI/CGClipLayer.h"


class MainLayer : public cocos2d::CCLayer, public cocos2d::extension::CCBSelectorResolver, public cocos2d::extension::CCBMemberVariableAssigner, public EventObserver
{
public:
    MainLayer();
    ~ MainLayer();
    
    static cocos2d::CCScene* scene();
    
    /*!
     * @brief	创建一个MainLayer实例。
     */
    static MainLayer* create();
    
    /*!
     * @brief		覆盖CCLayer类的init方法。
     * @see         CCLayer::init
     */
    virtual bool init();
    
    /*!
     * @brief		覆盖CCLayer类的onEnter方法。
     * @see         cocos2d::CCLayer::onEnter
     */
    virtual void onEnter();
    
    /*!
     * @brief		覆盖CCLayer类的onExit方法。
     * @see         cocos2d::CCLayer::onExit
     */
    virtual void onExit();
    
	/*!
     * @brief		覆盖CCLayer类的keyBackClicked方法。
     * @see         cocos2d::CCLayer::keyBackClicked
     */
	virtual void keyBackClicked();
    
	/*!
     * @brief		覆盖CCLayer类的keyMenuClicked方法。
     * @see         cocos2d::CCLayer::keyMenuClicked
     */
	virtual void keyMenuClicked();
    
private:
    
    virtual void registerWithTouchDispatcher(void);
    
    /*!
     * @brief		覆盖CCLayer类的ccTouchBegan方法。
     * @see         cocos2d::CCLayer::ccTouchBegan
     */
    virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    /*!
     * @brief		覆盖CCLayer类的ccTouchMoved方法。
     * @see         cocos2d::CCLayer::ccTouchMoved
     */
    virtual void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    /*!
     * @brief		覆盖CCLayer类的ccTouchEnded方法。
     * @see         cocos2d::CCLayer::ccTouchEnded
     */
    virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    /*!
     * @brief		覆盖CCLayer类的ccTouchCancelled方法。
     * @see         cocos2d::CCLayer::ccTouchCancelled
     */
    virtual void ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    /*!
     * @brief	处理CCBI文件中的CCMenuItem按钮点击事件。
     * @see     cocos2d::extension::CCBSelectorResolver
     */
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName);

    /*!
     * @brief	处理CCBI文件中的CCControl按钮点击事件。
     * @see     cocos2d::extension::CCBSelectorResolver
     */
    virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName);
    
    /*!
     * @brief	通过变量名称绑定CCBI文件中的变量。
     * @see     cocos2d::extension::CCBMemberVariableAssigner
     */
    virtual bool onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);
    
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
    
private:
    
    void initGridButtons();
    //初始化gridLayer的x y移动范围限制
    void initXYRange();
    //初始化可以移动的方向
    void initMoveDirection();
    //点是否在可移动区域内
    bool isInMoveArea(cocos2d::CCPoint point);
    //gridLayer匀速移动
    void layerXMoveInUniformSpeed(float moveV);
    void layerYMoveInUniformSpeed(float moveV);
    //把grid的rect转换为this坐标下的rect
    cocos2d::CCRect changeGridToThisCoordRect(int index);
    /*
     是否点击到一个grid 没有的话返回-1
     beginTouch和endTouch在同一个grid的rect内算是点击了该grid
     */
    int touchGrid(cocos2d::CCPoint beginTouch, cocos2d::CCPoint endTouch);
    
    
    //初始化TipsBg
    void initTipsBg();
    //显示对应的解析
    void showTips(bool isShow, int phraseIndex = -1, int phrase2Index = -1);
    //显示词语前面的特殊奖励精灵 isH true-横 false-竖
    void showSpecialBonus(Words *words, bool isH);
    
    //显示触摸动画
    void showTouchAction(Event *event);
    //隐藏触摸动画
    void hideTouchAction();
    
    //是否显示答案
    void setAnswers(bool isShow);
    //显示候选答案
    void showAnswers();
    
    //点击下方答案响应事件
    void onChooseAnswer(CCObject* pObject, cocos2d::extension::CCControlEvent event);
    
    //在Grid中显示玩家选择的答案
    void showChooseAnswer(Event *event);
    
    //设置正确答案字的Grid
    void setFixGridLabel(Event *event);
    
    //检查点击的grid是否已经是fix
    bool checkGridIndexIsFix(int gridIndex);
    
    //初始化本地玩家经验进度条
    void initLocalUserProTimer();
    //更新本地玩家信息  这个操作每次都解析json数据 稍慢  后期改进!
    void updateLocalUserMsg();
    //显示本地玩家的奖励动画 或者 弹出本地玩家的结算框
    void showLocalUserRewardOrWinLayer(Event *event);
    void blinkDone(CCNode* parent);
    
    //cgdialog的回调 暂时
    void onOk(CCObject* obj);
    
    //返回按钮及自身回调
    void onBackOk(CCObject* obj);
    void onBack(CCObject* pObject, cocos2d::extension::CCControlEvent event);

    //加载单机游戏上一局的信息
    void loadSingleSelectAnswer();
    //重置清空已选的答案
    void clearSingleSelectAnswer();
    
    //点击道具响应事件
    void onChooseTool(CCObject* pObject, cocos2d::extension::CCControlEvent event);
    
    
private:
    //-- 联网相关的方法
    void updateMainLayer();
    //以很绚的动画显示合作玩家完成的答案
    void showPartnerFixAnswer(int fixIndex);
    
    
    //初始化竞技模式完成度进度条
    void initCompCompleteProTimer();
    //初始化竞技模式和合作模式玩家信息  
    void initCompCoopUserMsg();
    
    
    //初始化合作模式时间进度条
    void initCoopTimeProTimer();
    //合作模式倒计时
    void coopCountTime(float dt);
    
    //联网处理游戏停止消息
    void handleGameStop(Event *event);
    //暂时的游戏结束回调 后期放到结算框里
    void onCompOk(CCObject* obj);
    void onCompCancel(CCObject* obj);
    void onCoopOk(CCObject* obj);
    void onCoopCancel(CCObject* obj);
    
private:
    //this的CCSize
    cocos2d::CCSize m_size;
    //缩放比例
    float m_ccbScale;
    //行列
    int m_line;
    int m_col;
    
    cocos2d::CCLayerColor *m_topLayer;
    cocos2d::CCLayer *m_bottomLayer;
    CGClipLayer *m_letterGroove;
    cocos2d::CCLayerColor *m_letterLayer;
    CGClipLayer *m_numberGroove;
    cocos2d::CCLayerColor *m_numberLayer;
    CGClipLayer *m_gridGroove;
    cocos2d::CCLayer *m_gridLayer;
    
    //格子精灵
//    std::vector<CGControlButton*> m_gridButtons;
    std::vector<cocos2d::CCSprite*> m_gridButtons;

    //精灵宽高
    float m_btnWidth;
    float m_btnHeight;
    
    //是否可以水平 垂直移动
    bool m_horizontal;
    bool m_vertical;
    
    //gridLayer的x y移动范围限制
    float m_xMinRange;
    float m_xMaxRange;
    float m_yMinRange;
    float m_yMaxRange;
    //letterLayer的x 移动范围限制
    float m_letterXMinRange;
    float m_letterXMaxRange;
    
    //numberLayer的y移动范围限制
    float m_numberYMinRange;
    float m_numberYMaxRange;
    
    /*!
     * @brief	场景层的移动速度
     */
    float m_moveV;
    /*!
     * @brief	触摸屏幕的实时点。
     */
    cocos2d::CCPoint m_location;
    //超过这个距离才能移动 防止误操作
    float m_ignoreDis;
    //点击的起始点 和 抬起点 如果这两个点在同一个格子精灵之间 将视为点击了该格子精灵
    cocos2d::CCPoint m_beginTouch;
    cocos2d::CCPoint m_endTouch;
    
    
    //字母数组
    char m_letters[20];
    
    
    
    //这里是m_topLayer层相关的变量
    cocos2d::CCLayer *m_tipsBg;
    cocos2d::CCLayer *m_tip1Bg;
    cocos2d::CCLabelTTF *m_tip1;
    cocos2d::CCLayer *m_tip2Bg;
    cocos2d::CCLabelTTF *m_tip2;
    
    /*单机相关变量 begin*/
    //这里视m_userMsgLayer层相关的变量  
    cocos2d::CCLabelTTF *m_localUserSilver;
    cocos2d::CCLabelTTF *m_localUserLevel;
    cocos2d::CCLabelTTF *m_localUserExp;
    cocos2d::CCSprite *m_localUserExpProBg;
    cocos2d::CCSprite *m_localUserExpPro;
    cocos2d::CCProgressTimer* m_localUserExpProTimer;
    /*单机相关变量 end*/
    
    //奖励相关变量
    cocos2d::CCSprite *m_bonusSpriteH;
    cocos2d::CCLabelBMFont *m_bonusBMFontH;
    cocos2d::CCSprite *m_bonusSpriteV;
    cocos2d::CCLabelBMFont *m_bonusBMFontV;
    
    
    //存放显示点击精灵的button的index
    std::vector<int> m_touchActionIndexV;
    

    //下方候选答案精灵
    std::vector<cocos2d::extension::CCControlButton*> m_answersV;
    
    //mainBorder
    MainBorderLayer *m_mainBorderLayer;
    
    /* 竞技相关变量 begin*/
    cocos2d::CCLabelTTF *m_ownUserGame;//自己游戏进度的数字显示
    cocos2d::CCSprite *m_ownUserGameProBg;//自己游戏进度条
    cocos2d::CCSprite *m_ownUserGamePro;
    cocos2d::CCProgressTimer* m_ownUserGameProTimer;
    
    cocos2d::CCLabelTTF *m_otherUserGame;
    cocos2d::CCSprite *m_otherUserGameProBg;
    cocos2d::CCSprite *m_otherUserGamePro;
    cocos2d::CCProgressTimer* m_otherUserGameProTimer;
    
    /*合作模式相关变量 begin too 和竞技共用*/
    //个人信息
    cocos2d::CCSprite *m_ownImg;
    cocos2d::CCLabelTTF *m_ownName;
    cocos2d::CCLabelTTF *m_ownLv;
    cocos2d::CCLabelTTF *m_ownUserGold;
    
    cocos2d::CCLabelTTF *m_ownRewardGold;
    cocos2d::CCLabelTTF *m_ownRewardExp;
    
    //对手信息
    cocos2d::CCSprite *m_otherImg;
    cocos2d::CCLabelTTF *m_otherName;
    cocos2d::CCLabelTTF *m_otherLv;
    cocos2d::CCLabelTTF *m_otherUserGold;
    
    cocos2d::CCLabelTTF *m_otherRewardGold;
    cocos2d::CCLabelTTF *m_otherRewardExp;
    /*合作模式相关变量 end too 和竞技共用*/
    /* 竞技相关变量 end*/
    
    
    /*合作模式相关变量 begin*/
    cocos2d::CCLabelTTF *m_ownComNum;
    cocos2d::CCLabelTTF *m_otherComNum;
    cocos2d::CCLabelTTF *m_allComNum;
    
    cocos2d::CCLabelTTF *m_coopTime;//倒计时数字 后期换成CCLabelBMFont
    cocos2d::CCSprite *m_timeProBg;
    cocos2d::CCSprite *m_timePro;
    cocos2d::CCSprite *m_timeProPoint;
    cocos2d::CCProgressTimer* m_timeProTimer;
    /*合作模式相关变量 end*/
};

#endif /* defined(__CrossWordClient__MainLayer__) */
