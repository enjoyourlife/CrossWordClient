//
//  CGDialog.cpp
//  CrossWordClient
//
//  Created by cy on 14-4-6.
//
//

#include "CGDialog.h"
#include "../CommonUI/CGCCBReader.h"
#include "../Common/Localize.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

CGDialog::CGDialog()
{
    m_target = NULL;
    m_okAction = NULL;
    m_cancelAction = NULL;
    
    m_ok_bg = NULL;
    m_ok_cancel_bg = NULL;
    m_text = NULL;
}

CGDialog::~ CGDialog()
{
    
    CC_SAFE_RELEASE_NULL(m_ok_bg);
    CC_SAFE_RELEASE_NULL(m_ok_cancel_bg);
    CC_SAFE_RELEASE_NULL(m_text);
    
}

CGDialog* CGDialog::create()
{
    CGDialog *layer = new CGDialog();
    if (layer && layer->init())
    {
        layer->autorelease();
    }
    else
    {
        delete layer;
        layer = NULL;
    }
    return layer;
}

void CGDialog::show(GameDialogType type, const char *textId, CCObject* target, SEL_MenuHandler okAction, SEL_MenuHandler cancelAction)
{
    CGDialog *dialog = CGDialog::create();
    dialog->setCGDialogByType(type);
    dialog->setText(Localize::sharedLocalize()->getString(textId));
    dialog->addTarget(target, okAction, cancelAction);
    
    CCDirector::sharedDirector()->getRunningScene()->addChild(dialog);
}

bool CGDialog::init()
{
    if (!CCLayer::init())
    {
        return false;
    }
    
    CCNodeLoaderLibrary *ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
    CGCCBReader reader(ccNodeLoaderLibrary);
    CCNode* node = reader.readCCBFile("common_dialog.ccbi", this);
    addChild(node);
    
    this->setTouchEnabled(true);
    this->setKeypadEnabled(true);
    
    m_text->setString("");
    //    m_text->setString(Localize::sharedLocalize()->getString("toast_txt0"));
    
    return true;
}

void CGDialog::keyBackClicked()
{
	CCLog("CGDialog::keyBackClicked()~~~");
}

void CGDialog::keyMenuClicked()
{
    CCLog("CGDialog::keyMenuClicked()~~~");
}

bool CGDialog::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    return true;
}

void CGDialog::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    
}

void CGDialog::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    
    
}

void CGDialog::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
}

void CGDialog::onEnter()
{
    CCLayer::onEnter();
    
}

void CGDialog::onExit()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCLayer::onExit();
}

void CGDialog::registerWithTouchDispatcher(void)
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority, true);
}

void CGDialog::setText(const char* text)
{
    m_text->setString(text);
}

void CGDialog::addTarget(CCObject* target, SEL_MenuHandler okAction, SEL_MenuHandler cancelAction)
{
	m_target = target;
	m_okAction = okAction;
    m_cancelAction = cancelAction;
}

SEL_MenuHandler CGDialog::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName)
{
    return NULL;
}

SEL_CCControlHandler CGDialog::onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onOk", CGDialog::onOk);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onCancel", CGDialog::onCancel);
    return NULL;
}

bool CGDialog::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_ok_bg", CCLayer*, m_ok_bg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_ok_cancel_bg", CCLayer*, m_ok_cancel_bg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_text", CCLabelTTF*, m_text);
    return false;
}

void CGDialog::onOk(CCObject* pObject, CCControlEvent event)
{
    if (m_target && m_okAction)
    {
        (m_target->*m_okAction)(this);
    }
    
    this->removeFromParentAndCleanup(true);
}

void CGDialog::onCancel(CCObject* pObject, CCControlEvent event)
{
    if (m_target && m_cancelAction)
    {
        (m_target->*m_cancelAction)(this);
    }
    
    this->removeFromParentAndCleanup(true);
}

void CGDialog::setCGDialogByType(GameDialogType type)
{
    switch (type)
    {
        case GameOKCancelButtonType:
        {
			m_ok_cancel_bg->setVisible(true);
			m_ok_bg->setVisible(false);
            break;
        }
        case GameOKButtonType:
        {
            m_ok_bg->setVisible(true);
            m_ok_cancel_bg->setVisible(false);
            break;
        }
            
        default:
            break;
    }
}

