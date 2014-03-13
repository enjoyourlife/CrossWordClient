//
//  CGWaiting.cpp
//  CrossWordClient
//
//  Created by cy on 14-3-4.
//
//

#include "CGWaiting.h"
#include "../CommonUI/CGCCBReader.h"
#include "../Common/Localize.h"
#include "../Common/Utilities.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

CGWaiting::CGWaiting()
{
    m_rollSprite = NULL;
    m_text = NULL;
}

CGWaiting::~ CGWaiting()
{
    CC_SAFE_RELEASE_NULL(m_rollSprite);
    
    
}

CGWaiting* CGWaiting::create()
{
    CGWaiting *layer = new CGWaiting();
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

bool CGWaiting::init()
{
    if (!CCLayer::init())
    {
        return false;
    }
    
    CCNodeLoaderLibrary *ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
    CGCCBReader reader(ccNodeLoaderLibrary);
    CCNode* node = reader.readCCBFile("waiting.ccbi", this);
    addChild(node);
    
    this->setTouchEnabled(true);
    this->setKeypadEnabled(true);
    
    m_text->setString(Localize::sharedLocalize()->getString("waiting_txt"));
    
    CCAnimate* animate = Utilities::createAnimate("wait", 14, 0.1f);
    m_rollSprite->runAction(CCRepeatForever::create(animate));
    return true;
}

void CGWaiting::keyBackClicked()
{
	CCLog("CGWaiting::keyBackClicked()~~~");
}

void CGWaiting::keyMenuClicked()
{
    CCLog("CGWaiting::keyMenuClicked()~~~");
}

void CGWaiting::onEnter()
{
    CCLayer::onEnter();
    
}

void CGWaiting::onExit()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCLayer::onExit();
}

SEL_MenuHandler CGWaiting::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName)
{
    return NULL;
}

SEL_CCControlHandler CGWaiting::onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName)
{
//    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onStart", CGWaiting::onStart);
    
    return NULL;
}


bool CGWaiting::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "rollSprite", CCSprite*, m_rollSprite);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "text", CCLabelTTF*, m_text);
    return false;
}

void CGWaiting::registerWithTouchDispatcher(void)
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority, true);
}

bool CGWaiting::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    return true;
}

void CGWaiting::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    
}

void CGWaiting::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    
    
}

void CGWaiting::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
}

