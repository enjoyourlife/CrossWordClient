//
//  CGToast.h
//  CrossWordClient
//
//  Created by cy on 14-3-2.
//
//

#ifndef __CrossWordClient__CGToast__
#define __CrossWordClient__CGToast__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"

/**
 类似于android的toast 在出现一段时间后自动消息 
 然后执行回调函数 如果有的话
 */
class  CGToast : public cocos2d::CCLayer, public cocos2d::extension::CCBSelectorResolver, public cocos2d::extension::CCBMemberVariableAssigner

{
public:
    virtual ~CGToast(void);
    CGToast(void);
    static CGToast* create();
	virtual bool init();

	void addTarget(CCObject* target, cocos2d::SEL_MenuHandler action);
    //根据actionType执行对应的动画
    void playAction(int actionType = 0);
    
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
	cocos2d::SEL_MenuHandler m_action;
	CCObject* m_target;
    
    cocos2d::CCSprite* m_toastBg;
    cocos2d::CCLabelTTF *m_text;
    
    void actionDone();
};

#endif /* defined(__CrossWordClient__CGToast__) */
