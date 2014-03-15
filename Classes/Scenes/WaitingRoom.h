//
//  WaitingRoom.h
//  CrossWordClient
//
//  Created by cy on 14-3-13.
//
//

#ifndef __CrossWordClient__WaitingRoom__
#define __CrossWordClient__WaitingRoom__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"


class WaitingRoom : public cocos2d::CCLayer, public cocos2d::extension::CCBSelectorResolver, public cocos2d::extension::CCBMemberVariableAssigner
{
public:
    WaitingRoom();
    ~ WaitingRoom();
    
    static cocos2d::CCScene* scene();
    
    /*!
     * @brief	创建一个WaitingRoom实例。
     */
    static WaitingRoom* create();
    
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
    
    /*!
     * @brief		覆盖CCLayer类的update方法。
     * @see         CCLayer::update
     */
    virtual void update(float dt);
    
private:
    
    void onBack(CCObject* pObject, cocos2d::extension::CCControlEvent event);
    //是否已经坐下
    bool m_sitDowned;
    float m_time;



};

#endif /* defined(__CrossWordClient__WaitingRoom__) */
