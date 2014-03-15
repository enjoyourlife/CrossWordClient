//
//  WaitingRoom.cpp
//  CrossWordClient
//
//  Created by cy on 14-3-13.
//
//

#include "WaitingRoom.h"
#include "../CommonUI/CGCCBReader.h"
#include "../Events/EventManager.h"
#include "../Events/Events.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

WaitingRoom::WaitingRoom()
{
    m_sitDowned = false;
    m_time = 0.0f;
    //    m_myCard = NULL;
    
}

WaitingRoom::~ WaitingRoom()
{
    //    CC_SAFE_RELEASE_NULL(m_myCard);
    
    
}

CCScene* WaitingRoom::scene()
{
    CCScene* scene = CCScene::create();
    WaitingRoom* layer = WaitingRoom::create();
    scene->addChild(layer);
    
    return scene;
}

WaitingRoom* WaitingRoom::create()
{
    WaitingRoom *layer = new WaitingRoom();
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

bool WaitingRoom::init()
{
    if (!CCLayer::init())
    {
        return false;
    }
    
    CCNodeLoaderLibrary *ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
    CGCCBReader reader(ccNodeLoaderLibrary);
    CCNode* node = reader.readCCBFile("waiting_room.ccbi", this);
    addChild(node);
    
    this->setTouchEnabled(true);
    this->setKeypadEnabled(true);
    
    CCDirector::sharedDirector()->getScheduler()->scheduleUpdateForTarget(this, 0, false);
    
    return true;
}

void WaitingRoom::keyBackClicked()
{
	CCLog("WaitingRoom::keyBackClicked()~~~");
}

void WaitingRoom::keyMenuClicked()
{
    CCLog("WaitingRoom::keyMenuClicked()~~~");
}

void WaitingRoom::onEnter()
{
    CCLayer::onEnter();
   
}

void WaitingRoom::onExit()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCLayer::onExit();
}

SEL_MenuHandler WaitingRoom::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName)
{
    return NULL;
}

SEL_CCControlHandler WaitingRoom::onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onBack", WaitingRoom::onBack);
    return NULL;
}

bool WaitingRoom::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode)
{
    //    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "myCard", CCSprite*, m_myCard);
    return false;
}

void WaitingRoom::registerWithTouchDispatcher(void)
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority, true);
}

bool WaitingRoom::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    return true;
}

void WaitingRoom::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    
}

void WaitingRoom::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    
    
}

void WaitingRoom::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
}

void WaitingRoom::update(float dt)
{
    m_time += dt;
    if (m_time >= 5.0f && !m_sitDowned)
    {
        Event* e = EventManager::sharedEventManager()->getEvent(EventTypeSitDown);
        if (e != NULL)
        {
            EventManager::sharedEventManager()->notifyEventSucceeded(e);
        }
        m_sitDowned = true;
    }
}

void WaitingRoom::onBack(CCObject* pObject, CCControlEvent event)
{
    if (!m_sitDowned)
    {
        Event *e = new Event(EventTypeEnterHall);
        EventManager::sharedEventManager()->addEvent(e);
    }
    else
    {
        //sit up
    }
    
}