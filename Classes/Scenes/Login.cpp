//
//  Login.cpp
//  CrossWordClient
//
//  Created by cy on 14-3-4.
//
//

#include "Login.h"
#include "../CommonUI/CGCCBReader.h"
#include "../Events/GameEvents.h"
#include "../Events/EventManager.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

Login::Login()
{
    m_username = NULL;
    m_password = NULL;
}

Login::~ Login()
{
    CC_SAFE_RELEASE_NULL(m_username);
    CC_SAFE_RELEASE_NULL(m_password);
}

Login* Login::create()
{
    Login *layer = new Login();
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

bool Login::init()
{
    if (!CCLayer::init())
    {
        return false;
    }
    
    CCNodeLoaderLibrary *ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
    CGCCBReader reader(ccNodeLoaderLibrary);
    CCNode* node = reader.readCCBFile("login.ccbi", this);
    addChild(node);
    
    this->setTouchEnabled(true);
    this->setKeypadEnabled(true);
    
    m_username->setString("user2");
    
    return true;
}

void Login::keyBackClicked()
{
	CCLog("Login::keyBackClicked()~~~");
}

void Login::keyMenuClicked()
{
    CCLog("Login::keyMenuClicked()~~~");
}

void Login::onEnter()
{
    CCLayer::onEnter();
}

void Login::onExit()
{
    
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCLayer::onExit();
}

SEL_MenuHandler Login::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName)
{
    return NULL;
}

SEL_CCControlHandler Login::onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onLogin", Login::onLogin);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onRegister", Login::onRegister);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClose", Login::onClose);
    
    return NULL;
}


bool Login::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "username", CCLabelTTF*, m_username);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "password", CCLabelTTF*, m_password);
    return false;
}

void Login::registerWithTouchDispatcher(void)
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority, true);
}

bool Login::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    return true;
}

void Login::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    
}

void Login::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    
    
}

void Login::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    
}

void Login::onLogin(CCObject* pObject, CCControlEvent event)
{
    const char* username = m_username->getString();
    const char* password = m_password->getString();
    
    LoginEvent *loginEvent = new LoginEvent(m_loginType, username, password);
    EventManager::sharedEventManager()->addEvent(loginEvent);
    CCLog("onLogin~~~~~~~");
}

void Login::onRegister(CCObject* pObject, CCControlEvent event)
{
    CCLog("onRegister~~~");
}

void Login::onClose(CCObject* pObject, CCControlEvent event)
{
    this->removeFromParentAndCleanup(true);
}

void Login::setLoginType(int loginType)
{
    m_loginType = loginType;
}
