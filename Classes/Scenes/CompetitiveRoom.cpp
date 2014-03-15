//
//  CompetitiveRoom.cpp
//  CrossWordClient
//
//  Created by cy on 14-3-7.
//
//

#include "CompetitiveRoom.h"
#include "../CommonUI/CGCCBReader.h"
#include "../CommonUI/CGToast.h"
#include "../Events/EventManager.h"
#include "../Events/GameEvents.h"
#include "../Common/Localize.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

CompetitiveRoom::CompetitiveRoom()
{
    //    m_myCard = NULL;
    
}

CompetitiveRoom::~ CompetitiveRoom()
{
    //    CC_SAFE_RELEASE_NULL(m_myCard);
    
    
}

CCScene* CompetitiveRoom::scene()
{
    CCScene* scene = CCScene::create();
    CompetitiveRoom* layer = CompetitiveRoom::create();
    scene->addChild(layer);
    
    return scene;
}

CompetitiveRoom* CompetitiveRoom::create()
{
    CompetitiveRoom *layer = new CompetitiveRoom();
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

bool CompetitiveRoom::init()
{
    if (!CCLayer::init())
    {
        return false;
    }
    
    CCNodeLoaderLibrary *ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
    CGCCBReader reader(ccNodeLoaderLibrary);
    CCNode* node = reader.readCCBFile("competitive_room.ccbi", this);
    addChild(node);
    
    this->setTouchEnabled(true);
    this->setKeypadEnabled(true);
    
    CGToast *toast = CGToast::create();
    toast->setText(Localize::sharedLocalize()->getString("toast_txt3"));
    toast->playAction();
    this->addChild(toast);
    
    return true;
}

void CompetitiveRoom::keyBackClicked()
{
	CCLog("CompetitiveRoom::keyBackClicked()~~~");
}

void CompetitiveRoom::keyMenuClicked()
{
    CCLog("CompetitiveRoom::keyMenuClicked()~~~");
}

void CompetitiveRoom::onEnter()
{
    CCLayer::onEnter();
}

void CompetitiveRoom::onExit()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCLayer::onExit();
}

SEL_MenuHandler CompetitiveRoom::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName)
{
    return NULL;
}

SEL_CCControlHandler CompetitiveRoom::onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onBack", CompetitiveRoom::onBack);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onEasy", CompetitiveRoom::onEasy);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onNormal", CompetitiveRoom::onNormal);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onHard", CompetitiveRoom::onHard);
    return NULL;
}

bool CompetitiveRoom::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode)
{
    //    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "myCard", CCSprite*, m_myCard);
    return false;
}

void CompetitiveRoom::registerWithTouchDispatcher(void)
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority, true);
}

bool CompetitiveRoom::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    return true;
}

void CompetitiveRoom::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    
}

void CompetitiveRoom::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    
    
}

void CompetitiveRoom::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
}


void CompetitiveRoom::onBack(CCObject* pObject, CCControlEvent event)
{
    Event *e = new Event(EventTypeEnterHall);
    EventManager::sharedEventManager()->addEvent(e);
}

void CompetitiveRoom::onEasy(CCObject* pObject, CCControlEvent event)
{
    SitDownEvent *sde = new SitDownEvent(0, 0);
    EventManager::sharedEventManager()->addEvent(sde);    
}

void CompetitiveRoom::onNormal(CCObject* pObject, CCControlEvent event)
{
    SitDownEvent *sde = new SitDownEvent(0, 1);
    EventManager::sharedEventManager()->addEvent(sde);
}

void CompetitiveRoom::onHard(CCObject* pObject, CCControlEvent event)
{
    SitDownEvent *sde = new SitDownEvent(0, 2);
    EventManager::sharedEventManager()->addEvent(sde);
}