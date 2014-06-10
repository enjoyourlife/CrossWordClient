//
//  Hall.cpp
//  CrossWordClient
//
//  Created by cy on 14-2-28.
//
//

#include "Hall.h"
#include "../CommonUI/CGCCBReader.h"
#include "../Events/EventManager.h"
#include "../Events/GameEvents.h"
#include "../Data/DataManager.h"
#include "../CommonUI/CGDialog.h"
#include "SceneManager.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

Hall::Hall()
{
//    m_myCard = NULL;
    
}

Hall::~ Hall()
{
//    CC_SAFE_RELEASE_NULL(m_myCard);
    
    
}

CCScene* Hall::scene()
{
    CCScene* scene = CCScene::create();
    Hall* layer = Hall::create();
    scene->addChild(layer);
    
    return scene;
}

Hall* Hall::create()
{
    Hall *layer = new Hall();
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

bool Hall::init()
{
    if (!CCLayer::init())
    {
        return false;
    }
    
    CCNodeLoaderLibrary *ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
    CGCCBReader reader(ccNodeLoaderLibrary);
    CCNode* node = reader.readCCBFile("hall.ccbi", this);
    addChild(node);
    
    this->setTouchEnabled(true);
    this->setKeypadEnabled(true);
    
    return true;
}

void Hall::keyBackClicked()
{
	CGDialog::show(GameOKCancelButtonType, "exit_game", this, menu_selector(Hall::exitGame), NULL);
}

void Hall::keyMenuClicked()
{
    
}

void Hall::onEnter()
{
    CCLayer::onEnter();
}

void Hall::onExit()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCLayer::onExit();
}

SEL_MenuHandler Hall::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName)
{
    return NULL;
}

SEL_CCControlHandler Hall::onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onSingleGame", Hall::onSingleGame);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onCompetitiveGame", Hall::onCompetitiveGame);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onCooperationGame", Hall::onCooperationGame);
    
    return NULL;
}

bool Hall::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode)
{
//    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "myCard", CCSprite*, m_myCard);
    return false;
}

void Hall::registerWithTouchDispatcher(void)
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority, true);
}

bool Hall::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    return true;
}

void Hall::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    
}

void Hall::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    
    
}

void Hall::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    
}

void Hall::onSingleGame(CCObject* pObject, CCControlEvent event)
{
    DataManager::sharedDataManager()->setGameType(GameTypeSingle);
    
    Event *e = new Event(EventTypeEnterSingleGame);
    EventManager::sharedEventManager()->addEvent(e);
}

void Hall::onCompetitiveGame(CCObject* pObject, CCControlEvent event)
{
    DataManager::sharedDataManager()->setGameType(GameTypeCompetitive);
    
    Event *e = new Event(EventTypeEnterCompetitiveGame);
    EventManager::sharedEventManager()->addEvent(e);
}

void Hall::onCooperationGame(CCObject* pObject, CCControlEvent event)
{
    DataManager::sharedDataManager()->setGameType(GameTypeCooperation);
    
    Event *e = new Event(EventTypeEnterCooperationGame);
    EventManager::sharedEventManager()->addEvent(e);
    
}

void Hall::exitGame(CCObject* obj)
{
    CCDirector::sharedDirector()->end();
}