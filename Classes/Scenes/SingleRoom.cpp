//
//  SingleRoom.cpp
//  CrossWordClient
//
//  Created by cy on 14-2-28.
//
//

#include "SingleRoom.h"
#include "../CommonUI/CGCCBReader.h"
#include "../Events/EventManager.h"
#include "../Events/Events.h"
#include "../Data/DataManager.h"
#include "../CommonUI/CGToast.h"
#include "../Common/Localize.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

SingleRoom::SingleRoom()
{
    m_normalLock = NULL;
    m_hardLock = NULL;
    
}

SingleRoom::~ SingleRoom()
{
    CC_SAFE_RELEASE_NULL(m_normalLock);
    CC_SAFE_RELEASE_NULL(m_hardLock);
    
}

CCScene* SingleRoom::scene()
{
    CCScene* scene = CCScene::create();
    SingleRoom* layer = SingleRoom::create();
    scene->addChild(layer);
    
    return scene;
}

SingleRoom* SingleRoom::create()
{
    SingleRoom *layer = new SingleRoom();
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

bool SingleRoom::init()
{
    if (!CCLayer::init())
    {
        return false;
    }
    
    CCNodeLoaderLibrary *ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
    CGCCBReader reader(ccNodeLoaderLibrary);
    CCNode* node = reader.readCCBFile("single_room.ccbi", this);
    addChild(node);
    
    this->setTouchEnabled(true);
    this->setKeypadEnabled(true);
    
    showLock();
    
    return true;
}

void SingleRoom::keyBackClicked()
{
	CCLog("SingleRoom::keyBackClicked()~~~");
}

void SingleRoom::keyMenuClicked()
{
    CCLog("SingleRoom::keyMenuClicked()~~~");
}

void SingleRoom::onEnter()
{
    CCLayer::onEnter();
    
}

void SingleRoom::onExit()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCLayer::onExit();
}

SEL_MenuHandler SingleRoom::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName)
{
    return NULL;
}

SEL_CCControlHandler SingleRoom::onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onBack", SingleRoom::onBack);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onEasy", SingleRoom::onEasy);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onNormal", SingleRoom::onNormal);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onHard", SingleRoom::onHard);
    return NULL;
}

bool SingleRoom::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_normalLock", CCSprite*, m_normalLock);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_hardLock", CCSprite*, m_hardLock);
    return false;
}

void SingleRoom::registerWithTouchDispatcher(void)
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority, true);
}

bool SingleRoom::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    return true;
}

void SingleRoom::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    
}

void SingleRoom::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    
    
}

void SingleRoom::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
}


void SingleRoom::onBack(CCObject* pObject, CCControlEvent event)
{
    DataManager::sharedDataManager()->setGameType(GameTypeSingle);
    
    Event *e = new Event(EventTypeEnterHall);
    EventManager::sharedEventManager()->addEvent(e);
}

void SingleRoom::onEasy(CCObject* pObject, CCControlEvent event)
{
    DataManager::sharedDataManager()->setLevel(0);
    
    Event *e = new Event(EventTypeEnterSingleSubGame);
    EventManager::sharedEventManager()->addEvent(e);
}

void SingleRoom::onNormal(CCObject* pObject, CCControlEvent event)
{

    if (DataManager::sharedDataManager()->getLocalUnLockLevel()->m_unLockSubLevel1 == -1)
    {
        //目前只弹出toast提示框 后面应该弹出一个提示框 可以让玩家花钱解锁
        const char* text = Localize::sharedLocalize()->getString("toast_txt7");
            
        CGToast *toast = CGToast::create();
        toast->setText(text);
        toast->playAction();
        this->addChild(toast);
    }
    else
    {
        DataManager::sharedDataManager()->setLevel(1);
        
        Event *e = new Event(EventTypeEnterSingleSubGame);
        EventManager::sharedEventManager()->addEvent(e);
    }
}

void SingleRoom::onHard(CCObject* pObject, CCControlEvent event)
{
    if (DataManager::sharedDataManager()->getLocalUnLockLevel()->m_unLockSubLevel2 == -1)
    {
        const char* text = Localize::sharedLocalize()->getString("toast_txt7");
        
        CGToast *toast = CGToast::create();
        toast->setText(text);
        toast->playAction();
        this->addChild(toast);
    }
    else
    {
        DataManager::sharedDataManager()->setLevel(2);
        
        Event *e = new Event(EventTypeEnterSingleSubGame);
        EventManager::sharedEventManager()->addEvent(e);
    }
}

void SingleRoom::showLock()
{
    if (DataManager::sharedDataManager()->getLocalUnLockLevel()->m_unLockSubLevel1 == -1)
    {
        m_normalLock->setVisible(true);
    }
    else
    {
        m_normalLock->setVisible(false);
    }
    
    if (DataManager::sharedDataManager()->getLocalUnLockLevel()->m_unLockSubLevel2 == -1)
    {
        m_hardLock->setVisible(true);
    }
    else
    {
        m_hardLock->setVisible(false);
    }
}