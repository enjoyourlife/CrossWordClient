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
    
    initEditBox();
//    m_username->setString("user2");
    
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
//    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "username", CCLabelTTF*, m_username);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "password", CCLabelTTF*, m_password);
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
    const char* username = m_username->getText();
    const char* password = m_password->getText();
    
    LoginEvent *loginEvent = new LoginEvent(m_loginType, username, password);
    EventManager::sharedEventManager()->addEvent(loginEvent);
    CCLog("onLogin~~~~~~~ %s and %s", username, password);
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

void Login::initEditBox()
{
    CCSize size = this->getContentSize();
    float bgWidth = size.width;
    float bgHeight = size.height;
    
    CCSpriteFrame *spriteFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("yellow_btn_select.png");
    CCScale9Sprite *sprite = CCScale9Sprite::createWithSpriteFrame(spriteFrame);
    m_username = CCEditBox::create(CCSizeMake(164, 52), sprite);//(164, 52)是yellow_btn_select.png的宽高
    m_username->setAnchorPoint(ccp(0.5f, 0.5f));
    m_username->setPosition(ccp(bgWidth * 0.25f, bgHeight * 0.8f));
    m_username->setText("");
    m_username->setMaxLength(20); //这个在PC上没有作用的。
    m_username->setOpacity(200);
    m_username->setFontSize(10);
    m_username->setInputMode(kEditBoxInputModeAny);
    this->addChild(m_username);
    
    CCScale9Sprite *spritePassword = CCScale9Sprite::createWithSpriteFrame(spriteFrame);
    m_password = CCEditBox::create(CCSizeMake(164, 52), spritePassword);//(164, 52)是yellow_btn_select.png的宽高
    m_password->setAnchorPoint(ccp(0.5f, 0.5f));
    m_password->setPosition(ccp(bgWidth * 0.75f, bgHeight * 0.8f));
    m_password->setText("");
    m_password->setMaxLength(20); //这个在PC上没有作用的。
    m_password->setOpacity(200);
    m_password->setFontSize(10);
    m_password->setInputFlag(kEditBoxInputFlagPassword);
    this->addChild(m_password);
    
}
