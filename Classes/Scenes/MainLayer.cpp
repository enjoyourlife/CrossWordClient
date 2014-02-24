//
//  MainLayer.cpp
//  CrossWordClient
//
//  Created by cy on 14-2-22.
//
//

#include "MainLayer.h"
#include "../CommonUI/CGCCBReader.h"
#include "../Network/NetServer.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

MainLayer::MainLayer()
{
    m_myCard = NULL;
    
}

MainLayer::~ MainLayer()
{
    CC_SAFE_RELEASE_NULL(m_myCard);
    
    
}

MainLayer* MainLayer::create()
{
    MainLayer *layer = new MainLayer();
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

bool MainLayer::init()
{
    if (!CCLayer::init())
    {
        return false;
    }
    
    CCNodeLoaderLibrary *ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
    CGCCBReader reader(ccNodeLoaderLibrary);
    CCNode* node = reader.readCCBFile("main.ccbi", this);
    addChild(node);
    
    this->setTouchEnabled(true);
    this->setKeypadEnabled(true);
    
    return true;
}

void MainLayer::keyBackClicked()
{
	CCLog("MainLayer::keyBackClicked()~~~");
}

void MainLayer::keyMenuClicked()
{
    CCLog("MainLayer::keyMenuClicked()~~~");
}

void MainLayer::onEnter()
{
    CCLayer::onEnter();
    
}

void MainLayer::onExit()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCLayer::onExit();
}

SEL_MenuHandler MainLayer::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName)
{
    return NULL;
}

SEL_CCControlHandler MainLayer::onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName)
{
   CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onStart", MainLayer::onStart);
    
    return NULL;
}


bool MainLayer::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "myCard", CCSprite*, m_myCard);
    return false;
}

void MainLayer::registerWithTouchDispatcher(void)
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority, true);
}

bool MainLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    return true;
}

void MainLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    
}

void MainLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
        
    
}

void MainLayer::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
}

void MainLayer::onStart(CCObject* pObject, CCControlEvent event)
{
    if (m_myCard->isVisible())
    {
        m_myCard->setVisible(false);
    }
    else
    {
        m_myCard->setVisible(true);
    }
    
    
    /*
    const char *route = "chat.chatHandler.send";
    json_t *msg = json_object();
    json_t *content = json_string("Good!");
    json_t *channelName = json_string("channelname");
    json_t *userName = json_string("username");
    json_t *target = json_string("*");
    json_object_set(msg, "content", content);
    json_object_set(msg, "rid", channelName);
    json_object_set(msg, "from", userName);
    json_object_set(msg, "target", target);
    // 使用的时候记得删除不用的变量
    json_decref(content);
    json_decref(channelName);
    json_decref(userName);
    json_decref(target);
    NetServer::sharedNetServer()->sendMsg(route, msg);
     */
}

