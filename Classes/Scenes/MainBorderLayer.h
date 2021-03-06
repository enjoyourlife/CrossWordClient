//
//  MainBorderLayer.h
//  CrossWordClient
//
//  Created by cy on 14-4-12.
//
//

#ifndef __CrossWordClient__MainBorderLayer__
#define __CrossWordClient__MainBorderLayer__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"


class MainBorderLayer : public cocos2d::CCLayer, public cocos2d::extension::CCBSelectorResolver, public cocos2d::extension::CCBMemberVariableAssigner
{
public:
    MainBorderLayer();
    ~ MainBorderLayer();
    
    static cocos2d::CCScene* scene();
    
    /*!
     * @brief	创建一个MainBorderLayer实例。
     */
    static MainBorderLayer* create();
    
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
    
    //显示百分比
    void showPer(const char* per, const char* otherInfo, const char* ownPer, const char* ownInfo);
    
    //显示单机玩家信息
    void updateLocalUserData();
    
    //是否显示重置按钮
    void showSingleResetBtn();
    
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
    //cgdialog的回调
    void onOk(CCObject* obj);
    void onBack(CCObject* pObject, cocos2d::extension::CCControlEvent event);
    
    //单机重置事件  单机重置功能放到关卡选择页面 当过关的时候 点击提示框 问是否需要重置
    void onSingleReset(CCObject* pObject, cocos2d::extension::CCControlEvent event);
    //重置确认 发送重置事件
    void onResetOk(CCObject* obj);
    
    cocos2d::CCLayerColor *m_leftBorder;
    cocos2d::CCLayerColor *m_rightBorder;
    //整体或对手的百分比
    cocos2d::CCLabelTTF *m_per;
    cocos2d::CCLabelTTF *m_otherInfo0;
    //自己的百分比
    cocos2d::CCLabelTTF *m_ownPer;
    cocos2d::CCLabelTTF *m_ownInfo;
    
    
    
    //左侧单机相关变量
    cocos2d::CCLayer *m_localUserBg;
    cocos2d::CCLabelTTF *m_localUserSilver;
    cocos2d::CCLabelTTF *m_localUserLevel;
    cocos2d::CCLabelTTF *m_localUserExp;
    
    //右侧单机相关变量
    cocos2d::extension::CCControlButton *m_singleResetBtn;
};

#endif /* defined(__CrossWordClient__MainBorderLayer__) */
