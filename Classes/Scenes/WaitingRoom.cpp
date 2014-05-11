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
#include "../Events/GameEvents.h"
#include "../Data/DataManager.h"

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
    if (m_time >= 3.0f && !m_sitDowned)
    {
        SitDownEvent* e = (SitDownEvent*)EventManager::sharedEventManager()->getEvent(EventTypeSitDown);
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
        //先清掉SitDownEvent
        SitDownEvent* sde = (SitDownEvent*)EventManager::sharedEventManager()->getEvent(EventTypeSitDown);
        if (sde != NULL)
        {
            EventManager::sharedEventManager()->notifyEventFailed(sde);
        }
        
        Event *e = new Event(EventTypeEnterHall);
        EventManager::sharedEventManager()->addEvent(e);
    }
    else
    {
        //其实SitDownEvent SitUpEvent中的type level直接取DataManager中的值就可以了 这里这两个值多余赋值了
        GameType gameType = DataManager::sharedDataManager()->getGameType();
        int level = DataManager::sharedDataManager()->getLevel();
        CCLog("gameType is %d level is %d", gameType, level);
        
        //sit up
        SitUpEvent *sue = new SitUpEvent(gameType, level);//1-竞技 2-合作
        EventManager::sharedEventManager()->addEvent(sue);
        
        //先站起然后回到选难度界面
        Event *e = new Event(EventTypeEnterCompetitiveGame);
        EventManager::sharedEventManager()->addEvent(e);
    }
    
}