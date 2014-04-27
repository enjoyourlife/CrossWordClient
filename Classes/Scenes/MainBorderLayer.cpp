//
//  MainBorderLayer.cpp
//  CrossWordClient
//
//  Created by cy on 14-4-12.
//
//

#include "MainBorderLayer.h"
#include "../CommonUI/CGCCBReader.h"
#include "../Events/EventManager.h"
#include "../Events/GameEvents.h"
#include "../Data/DataManager.h"
#include "../CommonUI/CGDialog.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

MainBorderLayer::MainBorderLayer()
{
    m_leftBorder = NULL;
    m_rightBorder = NULL;
}

MainBorderLayer::~ MainBorderLayer()
{
    CC_SAFE_RELEASE_NULL(m_leftBorder);
    CC_SAFE_RELEASE_NULL(m_rightBorder);
    
}

CCScene* MainBorderLayer::scene()
{
    CCScene* scene = CCScene::create();
    MainBorderLayer* layer = MainBorderLayer::create();
    scene->addChild(layer);
    
    return scene;
}

MainBorderLayer* MainBorderLayer::create()
{
    MainBorderLayer *layer = new MainBorderLayer();
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

bool MainBorderLayer::init()
{
    if (!CCLayer::init())
    {
        return false;
    }
    
    CCNodeLoaderLibrary *ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
    CGCCBReader reader(ccNodeLoaderLibrary);
    CCNode* node = reader.readCCBFile("main_border.ccbi", this);
    addChild(node);
    
//    this->setTouchEnabled(true);
//    this->setKeypadEnabled(true);
    
    return true;
}

void MainBorderLayer::keyBackClicked()
{
	CCLog("MainBorderLayer::keyBackClicked()~~~");
}

void MainBorderLayer::keyMenuClicked()
{
    CCLog("MainBorderLayer::keyMenuClicked()~~~");
}

void MainBorderLayer::onEnter()
{
    CCLayer::onEnter();
}

void MainBorderLayer::onExit()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCLayer::onExit();
}

SEL_MenuHandler MainBorderLayer::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName)
{
    return NULL;
}

SEL_CCControlHandler MainBorderLayer::onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onBack", MainBorderLayer::onBack);
    return NULL;
}

bool MainBorderLayer::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_leftBorder", CCLayerColor*, m_leftBorder);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_rightBorder", CCLayerColor*, m_rightBorder);
    return false;
}

void MainBorderLayer::registerWithTouchDispatcher(void)
{
//    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority, true);
}

bool MainBorderLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    return true;
}

void MainBorderLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    
}

void MainBorderLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    
    
}

void MainBorderLayer::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
}


void MainBorderLayer::onBack(CCObject* pObject, CCControlEvent event)
{
    
    CGDialog::show(GameOKCancelButtonType, "dialog_txt", this, menu_selector(MainBorderLayer::onOk), NULL);
}

void MainBorderLayer::onOk(CCObject* obj)
{
    GameType gameType = DataManager::sharedDataManager()->getGameType();
    int level = DataManager::sharedDataManager()->getLevel();
    
    //先站起然后回到对应的界面
    if (gameType == GameTypeSingle)
    {
        Event *e = new Event(EventTypeEnterSingleSubGame);
        EventManager::sharedEventManager()->addEvent(e);
    }
    else if (gameType == GameTypeCompetitive)
    {
        //sit up
        SitUpEvent *sue = new SitUpEvent(gameType, level);//1-竞技 2-合作
        EventManager::sharedEventManager()->addEvent(sue);
        
        Event *e = new Event(EventTypeEnterCompetitiveGame);
        EventManager::sharedEventManager()->addEvent(e);
    }
    else if (gameType == GameTypeCooperation)
    {
        
    }
    
}
