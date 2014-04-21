//
//  CGDialog.h
//  CrossWordClient
//
//  Created by cy on 14-4-6.
//
//

#ifndef __CrossWordClient__CGDialog__
#define __CrossWordClient__CGDialog__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"

typedef enum
{
	GameOKButtonType,
	GameOKCancelButtonType
} GameDialogType;

class  CGDialog : public cocos2d::CCLayer, public cocos2d::extension::CCBSelectorResolver, public cocos2d::extension::CCBMemberVariableAssigner
{
public:
    virtual ~CGDialog(void);
    CGDialog(void);
    
    static CGDialog* create();
    
    static void show(GameDialogType type = GameOKButtonType, const char *textId = "dialog_txt", CCObject* target = NULL, cocos2d::SEL_MenuHandler okAction = NULL, cocos2d::SEL_MenuHandler cancelAction = NULL);
    
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
	cocos2d::SEL_MenuHandler m_okAction;
    cocos2d::SEL_MenuHandler m_cancelAction;
	CCObject* m_target;
    
    
    cocos2d::CCLabelTTF *m_text;
    cocos2d::CCLayer *m_ok_cancel_bg;
    cocos2d::CCLayer *m_ok_bg;
    
    
    void onOk(CCObject* pObject, cocos2d::extension::CCControlEvent event);
    void onCancel(CCObject* pObject, cocos2d::extension::CCControlEvent event);
    
    //设置dialog显示的话
    void setText(const char* text);
	void addTarget(CCObject* target = NULL, cocos2d::SEL_MenuHandler okAction = NULL, cocos2d::SEL_MenuHandler cancelAction = NULL);
    void setCGDialogByType(GameDialogType type = GameOKButtonType);
};

#endif /* defined(__CrossWordClient__CGDialog__) */
