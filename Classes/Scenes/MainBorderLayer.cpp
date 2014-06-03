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
    
    m_per = NULL;
    m_ownPer = NULL;
    
    m_localUserBg = NULL;
    m_localUserSilver = NULL;
    m_localUserLevel = NULL;
    m_localUserExp = NULL;
    
    m_singleResetBtn = NULL;
}

MainBorderLayer::~ MainBorderLayer()
{
    CC_SAFE_RELEASE_NULL(m_leftBorder);
    CC_SAFE_RELEASE_NULL(m_rightBorder);
    
    CC_SAFE_RELEASE_NULL(m_per);
    CC_SAFE_RELEASE_NULL(m_ownPer);
    
    CC_SAFE_RELEASE_NULL(m_localUserBg);
    CC_SAFE_RELEASE_NULL(m_localUserSilver);
    CC_SAFE_RELEASE_NULL(m_localUserLevel);
    CC_SAFE_RELEASE_NULL(m_localUserExp);
    
    CC_SAFE_RELEASE_NULL(m_singleResetBtn);
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
    
    
    this->updateLocalUserData();
    this->showSingleResetBtn();
    
    
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
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onSingleReset", MainBorderLayer::onSingleReset);
    return NULL;
}

bool MainBorderLayer::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_leftBorder", CCLayerColor*, m_leftBorder);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_rightBorder", CCLayerColor*, m_rightBorder);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_per", CCLabelTTF*, m_per);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_ownPer", CCLabelTTF*, m_ownPer);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_localUserBg", CCLayer*, m_localUserBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_localUserSilver", CCLabelTTF*, m_localUserSilver);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_localUserLevel", CCLabelTTF*, m_localUserLevel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_localUserExp", CCLabelTTF*, m_localUserExp);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_singleResetBtn", CCControlButton*, m_singleResetBtn);
    
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
        
        //单机游戏玩家退出时 需要保存玩家所选的答案
        DataManager::sharedDataManager()->saveSelectAnswerVec();
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

void MainBorderLayer::showPer(const char* per, const char* ownPer)
{
    m_per->setString(per);
    m_ownPer->setString(ownPer);
}

void MainBorderLayer::updateLocalUserData()
{
    if (DataManager::sharedDataManager()->getGameType() == GameTypeSingle)
    {
        m_localUserBg->setVisible(true);
        LocalUser *lc = DataManager::sharedDataManager()->getLocalUser();
        int silver = lc->m_silver;
        int lv = lc->m_lv;
        int exp = lc->m_exp;
        
        char data[15];
        
        sprintf(data, "%d", silver);
        m_localUserSilver->setString(data);
        
        sprintf(data, "%d", lv);
        m_localUserLevel->setString(data);
        
        sprintf(data, "%d", exp);
        m_localUserExp->setString(data);
    }
    else
    {
        m_localUserBg->setVisible(false);
    }
}

void MainBorderLayer::showSingleResetBtn()
{
    if (DataManager::sharedDataManager()->getGameType() == GameTypeSingle && DataManager::sharedDataManager()->isWin())
    {
        m_singleResetBtn->setVisible(true);
    }
    else
    {
        m_singleResetBtn->setVisible(false);
    }
}

void MainBorderLayer::onSingleReset(CCObject* pObject, CCControlEvent event)
{
    CGDialog::show(GameOKCancelButtonType, "dialog_reset_txt", this, menu_selector(MainBorderLayer::onResetOk), NULL);
}

void MainBorderLayer::onResetOk(CCObject* obj)
{
    if (DataManager::sharedDataManager()->getGameType() == GameTypeSingle && DataManager::sharedDataManager()->isWin())
    {
        Event *event = new Event(EventTypeSingleReset);
        EventManager::sharedEventManager()->addEvent(event);
    }
}
