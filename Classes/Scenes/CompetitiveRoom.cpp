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
#include "../Data/DataManager.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

CompetitiveRoom::CompetitiveRoom()
{
    m_ownInfo = NULL;
    m_roomName = NULL;
}

CompetitiveRoom::~ CompetitiveRoom()
{
    CC_SAFE_RELEASE_NULL(m_ownInfo);
    CC_SAFE_RELEASE_NULL(m_roomName);
    
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
    
    initOwnInfo();
    
    initIntro();
    
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
    EventManager::sharedEventManager()->addObserver(this);
}

void CompetitiveRoom::onExit()
{
    EventManager::sharedEventManager()->removeObserver(this);
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

    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_ownInfo", CCLabelTTF*, m_ownInfo);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_roomName", CCLabelTTF*, m_roomName);
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
    GameType gameType = DataManager::sharedDataManager()->getGameType();
    
    int level = 0;
    DataManager::sharedDataManager()->setLevel(level);
    
    SitDownEvent *sde = new SitDownEvent(gameType, level);//1-竞技 2-合作
    EventManager::sharedEventManager()->addEvent(sde);    
}

void CompetitiveRoom::onNormal(CCObject* pObject, CCControlEvent event)
{
    GameType gameType = DataManager::sharedDataManager()->getGameType();
    
    int level = 1;
    DataManager::sharedDataManager()->setLevel(level);
    
    SitDownEvent *sde = new SitDownEvent(gameType, level);
    EventManager::sharedEventManager()->addEvent(sde);
}

void CompetitiveRoom::onHard(CCObject* pObject, CCControlEvent event)
{
    GameType gameType = DataManager::sharedDataManager()->getGameType();
    
    int level = 2;
    DataManager::sharedDataManager()->setLevel(level);
    
    SitDownEvent *sde = new SitDownEvent(gameType, level);
    EventManager::sharedEventManager()->addEvent(sde);
}

void CompetitiveRoom::onEventSucceeded(Event *event)
{
    int type = event->getType();
    switch (type)
    {
        case EventTypeUpdateInfoEx:
        {
            UpdateInfoEventEx *e = (UpdateInfoEventEx*)event;
            OnLineUser *ownOnLineUser = DataManager::sharedDataManager()->getOwnOnLineUser();
            if (e->getUid() == ownOnLineUser->m_uid)
            {
                char info[100];
                sprintf(info, "info:[username:%s, gold:%d, exp:%d]", ownOnLineUser->m_username.c_str(), ownOnLineUser->m_gold, ownOnLineUser->m_exp);
                m_ownInfo->setString(info);
            }
            break;
        }
            
        default:
            break;
    }
}

void CompetitiveRoom::onEventFailed(Event *event)
{
    int type = event->getType();
    switch (type)
    {
        case EventTypeGetInfo:
        {
            //TOAST获取信息失败
            CCLog("get info fail.");
            
//            m_ownInfo->setVisible(false);
            break;
        }
            
        default:
            break;
    }
}

void CompetitiveRoom::initIntro()
{
    GameType gameType = DataManager::sharedDataManager()->getGameType();
    if (gameType == GameTypeCompetitive)
    {
        m_roomName->setString("This is competitive");
    }
    else if (gameType == GameTypeCooperation)
    {
        m_roomName->setString("This is cooperation");
    }
}

void CompetitiveRoom::initOwnInfo()
{
    OnLineUser *ownOnLineUser = DataManager::sharedDataManager()->getOwnOnLineUser();
    
    char info[100];
    sprintf(info, "info:[username:%s, gold:%d, exp:%d]", DataManager::sharedDataManager()->getUsername().c_str(), ownOnLineUser->m_gold, ownOnLineUser->m_exp);
    m_ownInfo->setString(info);
}