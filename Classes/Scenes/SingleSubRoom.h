//
//  SingleSubRoom.h
//  CrossWordClient
//
//  Created by cy on 14-2-26.
//
//

#ifndef __CrossWordClient__SingleSubRoom__
#define __CrossWordClient__SingleSubRoom__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"

#define LEVELNUMINPAGE 6 //每页有多少关卡

class SingleSubRoom : public cocos2d::CCLayer, public cocos2d::extension::CCBSelectorResolver, public cocos2d::extension::CCBMemberVariableAssigner
{
public:
    SingleSubRoom();
    ~ SingleSubRoom();
    
    /*!
     * @brief	创建仅包含一个SingleSubRoom的场景。
     */
    static cocos2d::CCScene* scene();
    
    /*!
     * @brief	创建一个SingleSubRoom实例。
     */
    static SingleSubRoom* create();
    
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
    
private:
    
    void onBack(CCObject* pObject, cocos2d::extension::CCControlEvent event);
    void initScrollView();
    //创建ContainerLayer
    cocos2d::CCLayerColor* getContainerLayer();
    //调整CCScrollView使其自动回位
    void adjustScrollView(float offset);
    //关卡按钮的回调
    void onBtnClicked(CCObject* obj);
    //根据m_pageCount初始化m_ballLayer
    void initBallLayer();
    //设置显示对应的页面指示球
    void setCurPageBall(int curPage);
    
    //scrollview的占位符
    cocos2d::CCLayerColor *m_scrollViewBg;
    //scrollview和container(即包含内容能被拖动的层)
    cocos2d::extension::CCScrollView *m_scrollView;
    //总共的子关卡数
    int m_subLevelNum;
    //解锁的子关卡
    int m_unLockSubLevel;
    //页数 通过计算得到
    int m_pageCount;
    //当前页数
    int m_curPage;
    //触摸起始点
    cocos2d::CCPoint m_touchPoint;
    //scrollview的偏移量
    cocos2d::CCPoint m_offset;
    //m_scrollViewBg的Y轴范围
    float m_bgBeginY;
    float m_bgEndY;
    //放页面指示的CCLayer
    cocos2d::CCLayer *m_ballLayer;
    //指示球
    std::vector<cocos2d::CCSprite*> m_ballVector;
    
    float m_ccbScale;
};

#endif /* defined(__CrossWordClient__SingleSubRoom__) */
