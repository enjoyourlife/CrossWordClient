//
//  MainLayer.cpp
//  CrossWordClient
//
//  Created by cy on 14-2-22.
//
//

#include "MainLayer.h"
#include "../CommonUI/CGCCBReader.h"
#include "../Data/Grid.h"
#include "../Data/DataManager.h"
#include "../Common/Utilities.h"
#include "../Events/GameEvents.h"
#include "../Events/EventManager.h"
#include "../CommonUI/CGDialog.h"
#include "../CommonUI/CGToast.h"
#include "../Common/Localize.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

MainLayer::MainLayer()
{
    m_topLayer = NULL;
    m_bottomLayer = NULL;
    m_letterGroove = NULL;
    m_letterLayer = NULL;
    m_numberGroove = NULL;
    m_numberLayer = NULL;
    m_gridGroove = NULL;
    m_gridLayer = NULL;
    
    m_horizontal = false;
    m_vertical = false;
    
    m_size = CCSizeZero;
    
    m_btnWidth = 1.0f;
    m_btnHeight = 1.0f;
    
    m_moveV = 9.0f;
    m_location = CCPointZero;
    m_beginTouch = CCPointZero;
    m_endTouch = CCPointZero;
    
    m_letters[0] = 'A';
    m_letters[1] = 'B';
    m_letters[2] = 'C';
    m_letters[3] = 'D';
    m_letters[4] = 'E';
    m_letters[5] = 'F';
    m_letters[6] = 'G';
    m_letters[7] = 'H';
    m_letters[8] = 'I';
    m_letters[9] = 'J';
    m_letters[10] = 'K';
    m_letters[11] = 'L';
    m_letters[12] = 'M';
    m_letters[13] = 'N';
    m_letters[14] = 'O';
    m_letters[15] = 'P';
    m_letters[16] = 'Q';
    m_letters[17] = 'R';
    m_letters[18] = 'S';
    m_letters[19] = 'T';
    
    
    m_tipsBg = NULL;
    m_tip1Bg = NULL;
    m_tip1 = NULL;
    m_tip2Bg = NULL;
    m_tip2 = NULL;
    
    m_localUserSilver = NULL;
    m_localUserLevel = NULL;
    m_localUserExp = NULL;
    m_localUserExpProBg = NULL;
    m_localUserExpPro = NULL;
    
    m_bonusSpriteH = NULL;
    m_bonusBMFontH = NULL;
    m_bonusSpriteV = NULL;
    m_bonusBMFontV = NULL;
    
    
    for (int i = 0; i < ANSWER_NUM; i++)
    {
        m_answersV.push_back(CCControlButton::create());
    }
    
    
    //竞技模式部分
    m_ownUserGame = NULL;
    m_ownUserGameProBg = NULL;
    m_ownUserGamePro = NULL;
    m_otherUserGame = NULL;
    m_otherUserGameProBg = NULL;
    m_otherUserGamePro = NULL;

    
    m_ownImg = NULL;
    m_ownName = NULL;
    m_ownLv = NULL;
    m_ownUserGold = NULL;
    
    m_ownRewardGold = NULL;
    m_ownRewardExp = NULL;
    
    //对手信息
    m_otherImg = NULL;
    m_otherName = NULL;
    m_otherLv = NULL;
    m_otherUserGold = NULL;
    
    m_otherRewardGold = NULL;
    m_otherRewardExp = NULL;

    
    //合作模式部分
    m_ownComNum = NULL;
    m_otherComNum = NULL;
    m_allComNum = NULL;
    
    m_coopTime = NULL;//倒计时数字 后期换成CCLabelBMFont
    m_timeProBg = NULL;
    m_timePro = NULL;
    m_timeProPoint = NULL;
}

MainLayer::~ MainLayer()
{
    CC_SAFE_RELEASE_NULL(m_topLayer);
    CC_SAFE_RELEASE_NULL(m_bottomLayer);
    CC_SAFE_RELEASE_NULL(m_letterGroove);
    CC_SAFE_RELEASE_NULL(m_letterLayer);
    CC_SAFE_RELEASE_NULL(m_numberGroove);
    CC_SAFE_RELEASE_NULL(m_numberLayer);
    CC_SAFE_RELEASE_NULL(m_gridGroove);
    CC_SAFE_RELEASE_NULL(m_gridLayer);
    
    CC_SAFE_RELEASE_NULL(m_tipsBg);
    CC_SAFE_RELEASE_NULL(m_tip1Bg);
    CC_SAFE_RELEASE_NULL(m_tip1);
    CC_SAFE_RELEASE_NULL(m_tip2Bg);
    CC_SAFE_RELEASE_NULL(m_tip2);
    
    CC_SAFE_RELEASE_NULL(m_localUserSilver);
    CC_SAFE_RELEASE_NULL(m_localUserLevel);
    CC_SAFE_RELEASE_NULL(m_localUserExp);
    CC_SAFE_RELEASE_NULL(m_localUserExpProBg);
    CC_SAFE_RELEASE_NULL(m_localUserExpPro);
    
    CC_SAFE_RELEASE_NULL(m_bonusSpriteH);
    CC_SAFE_RELEASE_NULL(m_bonusBMFontH);
    CC_SAFE_RELEASE_NULL(m_bonusSpriteV);
    CC_SAFE_RELEASE_NULL(m_bonusBMFontV);
    
    vector<CCControlButton*>::iterator it;
    for (it = m_answersV.begin(); it != m_answersV.end(); it++)
    {
        CCControlButton* controlButton = *it;
        CC_SAFE_RELEASE_NULL(controlButton);
    }
    
    //竞技模式部分
    CC_SAFE_RELEASE_NULL(m_ownUserGame);
    CC_SAFE_RELEASE_NULL(m_ownUserGameProBg);
    CC_SAFE_RELEASE_NULL(m_ownUserGamePro);
    CC_SAFE_RELEASE_NULL(m_otherUserGame);
    CC_SAFE_RELEASE_NULL(m_otherUserGameProBg);
    CC_SAFE_RELEASE_NULL(m_otherUserGamePro);
    
    
    CC_SAFE_RELEASE_NULL(m_ownImg);
    CC_SAFE_RELEASE_NULL(m_ownName);
    CC_SAFE_RELEASE_NULL(m_ownLv);
    CC_SAFE_RELEASE_NULL(m_ownUserGold);
    
    CC_SAFE_RELEASE_NULL(m_ownRewardGold);
    CC_SAFE_RELEASE_NULL(m_ownRewardExp);
    
    CC_SAFE_RELEASE_NULL(m_otherImg);
    CC_SAFE_RELEASE_NULL(m_otherName);
    CC_SAFE_RELEASE_NULL(m_otherLv);
    CC_SAFE_RELEASE_NULL(m_otherUserGold);
    
    CC_SAFE_RELEASE_NULL(m_otherRewardGold);
    CC_SAFE_RELEASE_NULL(m_otherRewardExp);
    
    //合作模式部分
    CC_SAFE_RELEASE_NULL(m_ownComNum);
    CC_SAFE_RELEASE_NULL(m_otherComNum);
    CC_SAFE_RELEASE_NULL(m_allComNum);
    
    CC_SAFE_RELEASE_NULL(m_coopTime);//倒计时数字 后期换成CCLabelBMFont
    CC_SAFE_RELEASE_NULL(m_timeProBg);
    CC_SAFE_RELEASE_NULL(m_timePro);
    CC_SAFE_RELEASE_NULL(m_timeProPoint);
}

CCScene* MainLayer::scene()
{
    CCScene* scene = CCScene::create();
    MainLayer* layer = MainLayer::create();
    scene->addChild(layer);
    
    return scene;
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
    ccNodeLoaderLibrary->registerCCNodeLoader("CGClipLayer", CGClipLayerLoader::loader());
    
    CGCCBReader reader(ccNodeLoaderLibrary);
    CCNode* node = NULL;
    if (DataManager::sharedDataManager()->getGameType() == GameTypeSingle)
    {
         node = reader.readCCBFile("main.ccbi", this);
    }
    else if (DataManager::sharedDataManager()->getGameType() == GameTypeCompetitive)
    {
         node = reader.readCCBFile("competitive_main.ccbi", this);
    }
    else if (DataManager::sharedDataManager()->getGameType() == GameTypeCooperation)
    {
        node = reader.readCCBFile("cooperation_main.ccbi", this);
    }
   
    addChild(node);
    
    m_ccbScale = reader.getCCBScale();
    
    this->setTouchEnabled(true);
    this->setKeypadEnabled(true);
    
//    m_mainBorderLayer = MainBorderLayer::create();
//    this->addChild(m_mainBorderLayer);
    
    
    m_size = this->getContentSize();
    m_ignoreDis = CCDirector::sharedDirector()->getWinSize().width * 0.005f;
    
    
    m_line = DataManager::sharedDataManager()->getLine();
    m_col = DataManager::sharedDataManager()->getCol();
    
    
    
    
    initGridButtons();
    initMoveDirection();
    initXYRange();
    
    initTipsBg();

    
    setAnswers(false);
    
    loadSingleSelectAnswer();
    
    initLocalUserProTimer();
    updateLocalUserMsg();
    
    
    initCompCompleteProTimer();
    initCompCoopUserMsg();
    
    
    initCoopTimeProTimer();
    
    
    
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
    EventManager::sharedEventManager()->addObserver(this);
    
    if (DataManager::sharedDataManager()->getGameType() == GameTypeCooperation)
    {
        CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(MainLayer::coopCountTime), this, 1.0f, false);
    }
}

void MainLayer::onExit()
{
    if (DataManager::sharedDataManager()->getGameType() == GameTypeCooperation)
    {
        CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(MainLayer::coopCountTime), this);
    }
    
    EventManager::sharedEventManager()->removeObserver(this);
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCLayer::onExit();
}

SEL_MenuHandler MainLayer::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName)
{
    return NULL;
}

SEL_CCControlHandler MainLayer::onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onBack", MainLayer::onBack);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onChooseAnswer", MainLayer::onChooseAnswer);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onChooseTool", MainLayer::onChooseTool);
    
    
    return NULL;
}


bool MainLayer::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_topLayer", CCLayerColor*, m_topLayer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_bottomLayer", CCLayer*, m_bottomLayer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_letterGroove", CGClipLayer*, m_letterGroove);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_letterLayer", CCLayerColor*, m_letterLayer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_numberGroove", CGClipLayer*, m_numberGroove);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_numberLayer", CCLayerColor*, m_numberLayer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_gridGroove", CGClipLayer*, m_gridGroove);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_gridLayer", CCLayer*, m_gridLayer);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_tipsBg", CCLayer*, m_tipsBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_tip1Bg", CCLayer*, m_tip1Bg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_tip1", CCLabelTTF*, m_tip1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_tip2Bg", CCLayer*, m_tip2Bg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_tip2", CCLabelTTF*, m_tip2);
    
    if (DataManager::sharedDataManager()->getGameType() == GameTypeSingle)
    {
        CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_localUserSilver", CCLabelTTF*, m_localUserSilver);
        CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_localUserLevel", CCLabelTTF*, m_localUserLevel);
        CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_localUserExp", CCLabelTTF*, m_localUserExp);
        CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_localUserExpProBg", CCSprite*, m_localUserExpProBg);
        CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_localUserExpPro", CCSprite*, m_localUserExpPro);
    }
    
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_bonusSpriteH", CCSprite*, m_bonusSpriteH);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_bonusBMFontH", CCLabelBMFont*, m_bonusBMFontH);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_bonusSpriteV", CCSprite*, m_bonusSpriteV);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_bonusBMFontV", CCLabelBMFont*, m_bonusBMFontV);


    char name[15];
    for (int i = 0; i < ANSWER_NUM; i++)
    {
        sprintf(name, "m_answer%d", i);
        CCB_MEMBERVARIABLEASSIGNER_GLUE(this, name, CCControlButton*, m_answersV.at(i));
    }
    
    //竞技模式部分
    if (DataManager::sharedDataManager()->getGameType() == GameTypeCompetitive)
    {
        CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_ownUserGame", CCLabelTTF*, m_ownUserGame);
        CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_ownUserGameProBg", CCSprite*, m_ownUserGameProBg);
        CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_ownUserGamePro", CCSprite*, m_ownUserGamePro);
        CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_otherUserGame", CCLabelTTF*, m_otherUserGame);
        CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_otherUserGameProBg", CCSprite*, m_otherUserGameProBg);
        CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_otherUserGamePro", CCSprite*, m_otherUserGamePro);
    }
    
    //共用部分
    if (DataManager::sharedDataManager()->getGameType() == GameTypeCompetitive || DataManager::sharedDataManager()->getGameType() == GameTypeCooperation)
    {
        CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_ownImg", CCSprite*, m_ownImg);
        CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_ownName", CCLabelTTF*, m_ownName);
        CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_ownLv", CCLabelTTF*, m_ownLv);
        CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_ownUserGold", CCLabelTTF*, m_ownUserGold);
        
        CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_ownRewardGold", CCLabelTTF*, m_ownRewardGold);
        CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_ownRewardExp", CCLabelTTF*, m_ownRewardExp);
        
        CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_otherImg", CCSprite*, m_otherImg);
        CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_otherName", CCLabelTTF*, m_otherName);
        CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_otherLv", CCLabelTTF*, m_otherLv);
        CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_otherUserGold", CCLabelTTF*, m_otherUserGold);
        
        CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_otherRewardGold", CCLabelTTF*, m_otherRewardGold);
        CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_otherRewardExp", CCLabelTTF*, m_otherRewardExp);
    }
    
    //合作模式部分
    if (DataManager::sharedDataManager()->getGameType() == GameTypeCooperation)
    {
        CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_ownComNum", CCLabelTTF*, m_ownComNum);
        CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_otherComNum", CCLabelTTF*, m_otherComNum);
        CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_allComNum", CCLabelTTF*, m_allComNum);
        CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_coopTime", CCLabelTTF*, m_coopTime);

        CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_timeProBg", CCSprite*, m_timeProBg);
        CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_timePro", CCSprite*, m_timePro);
        CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_timeProPoint", CCSprite*, m_timeProPoint);
    }

    
    return false;
}

void MainLayer::registerWithTouchDispatcher(void)
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority, true);
}

bool MainLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    CCPoint point = pTouch->getLocation();
    if (isInMoveArea(point))
    {
        m_location = pTouch->getLocation();
        //放在里面才能防止在外面点到grid 但是不能防止在字母和数字边上点击grid 后面修改一下
        m_beginTouch = pTouch->getLocation();
    }

    return true;
}

void MainLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    CCPoint nowPoint = pTouch->getLocation();
    if (isInMoveArea(nowPoint))
    {
        bool flag = false;
        
        float xDis = nowPoint.x - m_location.x;
        if (fabs(xDis) > m_ignoreDis)
        {
            flag = true;
            if (nowPoint.x >= m_location.x) //向右移动
            {
                layerXMoveInUniformSpeed(m_moveV);
            }
            else
            {
                layerXMoveInUniformSpeed(-m_moveV);
            }
        }
        
        float yDis = nowPoint.y - m_location.y;
        if (fabs(yDis) > m_ignoreDis)
        {
            flag = true;
            if (nowPoint.y >= m_location.y)//向上移动
            {
                layerYMoveInUniformSpeed(m_moveV);
            }
            else
            {
                layerYMoveInUniformSpeed(-m_moveV);
            }
        }
        
        if (flag)
        {
            m_location = nowPoint;
        }
        
    }
    
    
}

void MainLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    m_endTouch = pTouch->getLocation();
    
    CCLog("TOUCH GRID INDEX IS %d", this->touchGrid(m_beginTouch, m_endTouch));
}

void MainLayer::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
}

void MainLayer::onEventSucceeded(Event *event)
{
    int type = event->getType();
    switch (type)
    {
        case EventTypeTouchGrid:
        {
            TouchGridEvent *touchGridEvent = (TouchGridEvent*)event;
            Grid *touchGrid = DataManager::sharedDataManager()->getTouchGridDirect();
            if (touchGrid != NULL)
            {
                showTips(true, touchGrid->getPhraseIndex(), touchGrid->getPhrase2Index());
            }
            
            showTouchAction(touchGridEvent);
            
            //如果不是正确答案 则显示候选答案
            if (!checkGridIndexIsFix(touchGrid->getIndex()))
            {
                showAnswers();
            }
            else
            {
                setAnswers(false);
            }
            break;
        }
            
        case EventTypeChooseAnswer:
        {
            showChooseAnswer(event);
            break;
        }
            
        case EventTypeFixAnswer:
        {
            //效果是播放一个动画 然后把正确的字置黑 再隐藏答案
            setFixGridLabel(event);
            setAnswers(false);
            //暂时隐藏选择动画
            hideTouchAction();
            break;
        }
            
        case EventTypeUpdateMainEx://联网才有
        {
            updateMainLayer();
            break;
        }
            
        case EventTypeReward:
        {
            //暂时
            if (DataManager::sharedDataManager()->getGameType() == GameTypeSingle)
            {
                showLocalUserRewardOrWinLayer(event);
                updateLocalUserMsg();
//                m_mainBorderLayer->updateLocalUserData();
                
                
            }
            
            break;
        }
            
        case EventTypeGameStopEx:
        {
            if (DataManager::sharedDataManager()->getGameType() != GameTypeSingle)
            {
                handleGameStop(event);
            }
            break;
        }
            
        case EventTypeSingleReset:
        {
            clearSingleSelectAnswer();
            //隐藏重置按钮
            m_mainBorderLayer->showSingleResetBtn();
            
            const char* text = Localize::sharedLocalize()->getString("toast_txt10");
            CGToast *toast = CGToast::create();
            toast->setText(text);
            toast->playAction();
            this->addChild(toast);
            
            break;
        }
        
        case EventTypeDisconnectEx:
        {
            if (DataManager::sharedDataManager()->getGameType() != GameTypeSingle)
            {
                //先只弹出提示 后面完善的时候 再退回某个界面 然后自动重连!
                CGDialog::show(GameOKButtonType, "dialog_lost_net", this, NULL, NULL);
            }
            break;
        }
            
        case EventTypeUserExitEx:
        {
            if (DataManager::sharedDataManager()->getGameType() == GameTypeCooperation)
            {
                const char* text = Localize::sharedLocalize()->getString("toast_userexit");
                CGToast *toast = CGToast::create();
                toast->setText(text);
                toast->playAction();
                this->addChild(toast);
            }
            
            break;
        }
            
        default:
            break;
    }
}

void MainLayer::onEventFailed(Event *event)
{
    int type = event->getType();
    switch (type)
    {
        case EventTypeTouchGrid:
        {
            showTips(false);
            
            hideTouchAction();
            
            setAnswers(false);
            
            break;
        }
            
        default:
            break;
    }
}


void MainLayer::initGridButtons()
{
    float beginPosX = 0.0f;
    float beginPosY = m_gridLayer->getPosition().y;
    
    vector<Words*> wordsVec = DataManager::sharedDataManager()->getWords();
    vector<Grid*> grids = DataManager::sharedDataManager()->getGrids();
    for (vector<Grid*>::iterator it = grids.begin(); it != grids.end(); ++it)
    {
        Grid *grid = *it;
    
        int type = grid->getType();
        int type2 = grid->getType2();
        CCSpriteFrame* gridFrame = NULL;
        if (type == -1 && type2 == -1)
        {
            gridFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("main_grid0.png");
        }
        else
        {
            gridFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("main_grid1.png");
        }
        CCSprite *button = CCSprite::createWithSpriteFrame(gridFrame);

        int index = grid->getIndex();
        button->setTag(index);
        
        button->setScale(m_ccbScale);
        button->setAnchorPoint(ccp(0.0f, 1.0f));
        
        m_btnWidth = button->getContentSize().width * m_ccbScale;//不知道是否scale了没
        m_btnHeight = button->getContentSize().height * m_ccbScale;
        
        float btnPosX = beginPosX + (index % m_col) * m_btnWidth;
        float btnPosY = beginPosY - (index / m_col) * m_btnHeight;
        button->setPosition(ccp(btnPosX, btnPosY));
        
        m_gridLayer->addChild(button);
        
        
        char num[3];
        sprintf(num, "%d", index);
        CCLabelTTF *label = CCLabelTTF::create(num, "Cochin", 16);
        label->setPosition(ccp(button->getContentSize().width * 0.9f, button->getContentSize().height * 0.9f));
        label->setColor(ccYELLOW);
        button->addChild(label);
        
        int phraseIndex = grid->getPhraseIndex();//第几个成语
        int phrase2Index = grid->getPhrase2Index();
        if (phraseIndex != -1 || phrase2Index != -1)
        {
            CCLabelTTF *wordLabel = CCLabelTTF::create("", "Cochin", 36);
            wordLabel->setPosition(ccp(button->getContentSize().width * 0.5f, button->getContentSize().height * 0.5f));
            wordLabel->setColor(ccBLUE);
            button->addChild(wordLabel, 0, 99);//tag = 99
        }
        
        
        m_gridButtons.push_back(button);
    }
    
    
    //初始化字母
    m_letterLayer->setContentSize(CCSizeMake(m_btnWidth * m_col, m_letterLayer->getContentSize().height));
    float letterBeginPosX = m_btnWidth * 0.5f;
    float letterPosY = m_letterLayer->getContentSize().height * 0.5f;
    char letter[3];
    for (int i = 0; i < m_col; i++)
    {
        sprintf(letter, "%c", m_letters[i]);
        CCLabelTTF *label = CCLabelTTF::create(letter, "TrebuchetMS-Bold", 20);
        label->setPosition(ccp(letterBeginPosX + i * m_btnWidth, letterPosY));
        label->setColor(ccc3(58, 118, 132));
        m_letterLayer->addChild(label);
    }
    
    //初始化数字
    m_numberLayer->setContentSize(CCSizeMake(m_numberLayer->getContentSize().width, m_btnHeight * m_line));
    float numberPosX = m_numberLayer->getContentSize().width * 0.5f;
    float numberBeginPosY = m_numberLayer->getContentSize().height - m_btnHeight * 0.5f;
    char number[3];
    for (int i = 0; i < m_line; i++)
    {
        sprintf(number, "%d", (i + 1));
        CCLabelTTF *label = CCLabelTTF::create(number, "TrebuchetMS-Bold", 20);
        label->setPosition(ccp(numberPosX, numberBeginPosY - i * m_btnHeight));
        label->setColor(ccc3(58, 118, 132));
        m_numberLayer->addChild(label);
    }
}

void MainLayer::initXYRange()
{
    float grooveWidth = m_gridGroove->getContentSize().width;
    float grooveHeight = m_gridGroove->getContentSize().height;
    
    if (m_horizontal)
    {
        float xDis = m_btnWidth * m_col - grooveWidth;
        
        m_xMinRange = m_gridLayer->getPosition().x - xDis;
        m_xMaxRange = m_gridLayer->getPosition().x;
        
        m_letterXMinRange = m_letterLayer->getPosition().x - xDis;
        m_letterXMaxRange = m_letterLayer->getPosition().x;
    }
    
    if (m_vertical)
    {
        float yDis = m_btnHeight * m_line - grooveHeight;
        
        m_yMinRange = m_gridLayer->getPosition().y;
        m_yMaxRange = m_gridLayer->getPosition().y + yDis;
        
        m_numberYMinRange = m_numberLayer->getPosition().y;
        m_numberYMaxRange = m_numberLayer->getPosition().y + yDis;
    }
}

void MainLayer::initMoveDirection()
{
    if (m_gridButtons.size() > 0)
    {
        float grooveWidth = m_gridGroove->getContentSize().width;
        float grooveHeight = m_gridGroove->getContentSize().height;
        if (m_btnWidth * m_col > grooveWidth)
        {
            m_horizontal = true;
        }
        
        if (m_btnHeight * m_line > grooveHeight)
        {
            m_vertical = true;
        }
    }
}

bool MainLayer::isInMoveArea(CCPoint point)
{
    bool flag = false;
    
    //即m_numberGroove左下角在this中的坐标
//    float beginX = m_size.width * 0.125f;
//    float beginY = m_size.height * 0.15f;
    float beginX = m_size.width * 0.0198f;
    float beginY = m_size.height * 0.0144f;

    CCRect rect = CCRect(beginX, beginY, m_size.width * 0.9604f, m_size.height * 0.7372f);
	if (rect.containsPoint(point))
    {
        flag = true;
    }
    return flag;
}

void MainLayer::layerXMoveInUniformSpeed(float moveV)
{
    if (m_horizontal)//水平
    {
        float posX = m_gridLayer->getPosition().x;
        float posY = m_gridLayer->getPosition().y;
        
        float letterLayerPosX = m_letterLayer->getPosition().x;
        float letterLayerPosY = m_letterLayer->getPosition().y;
        
        float newPosX = posX + moveV;
        float letterNewPosX = letterLayerPosX + moveV;
        
        if (newPosX >= m_xMinRange && newPosX <= m_xMaxRange)
        {
            m_gridLayer->setPosition(ccp(newPosX, posY));
            
            m_letterLayer->setPosition(ccp(letterNewPosX, letterLayerPosY));
        }
        else
        {
            if (newPosX < m_xMinRange)
            {
                m_gridLayer->setPosition(ccp(m_xMinRange, posY));
                m_letterLayer->setPosition(ccp(m_letterXMinRange, letterLayerPosY));
            }
            if (newPosX > m_xMaxRange)
            {
                m_gridLayer->setPosition(ccp(m_xMaxRange, posY));
                m_letterLayer->setPosition(ccp(m_letterXMaxRange, letterLayerPosY));
            }
        }
    }
    
}

void MainLayer::layerYMoveInUniformSpeed(float moveV)
{
    if (m_vertical)
    {
        float posX = m_gridLayer->getPosition().x;
        float posY = m_gridLayer->getPosition().y;
        
        float numberPosX = m_numberLayer->getPosition().x;
        float numberPosY = m_numberLayer->getPosition().y;
        
        float newPosY = posY + moveV;
        float numberNewPosY = numberPosY + moveV;
        
        if (newPosY >= m_yMinRange && newPosY <= m_yMaxRange)
        {
            m_gridLayer->setPosition(ccp(posX, newPosY));
            
            m_numberLayer->setPosition(ccp(numberPosX, numberNewPosY));
        }
        else
        {
            if (newPosY < m_yMinRange)
            {
                m_gridLayer->setPosition(ccp(posX, m_yMinRange));//暂时不设置m_numberLayer
                m_numberLayer->setPosition(ccp(numberPosX, m_numberYMinRange));
            }
            if (newPosY > m_yMaxRange)
            {
                m_gridLayer->setPosition(ccp(posX, m_yMaxRange));
                m_numberLayer->setPosition(ccp(numberPosX, m_numberYMaxRange));
            }
        }
        
    }
}

CCRect MainLayer::changeGridToThisCoordRect(int index)
{
    //m_gridLayer的锚点在m_gridGroove中的位置 把它转换成在this坐标系下的位置 然后以这个位置为定点算grid的rect
    CCPoint gridLayerLoc = m_gridLayer->getPosition();
//    CCPoint gridLayerLocInThis = ccp(gridLayerLoc.x + m_size.width * 0.175f, gridLayerLoc.y + m_size.height * 0.15f);
    CCPoint gridLayerLocInThis = ccp(gridLayerLoc.x + m_size.width * 0.0541f, gridLayerLoc.y + m_size.height * 0.0144f);
    
    int gridLine = index / m_col;
    int gridCol = index % m_col;
    
    //grid左下角坐标在this中的位置
    float gridX = gridLayerLocInThis.x + m_btnWidth * gridCol;
    float gridY = gridLayerLocInThis.y - m_btnHeight * (gridLine + 1);
    
    return CCRectMake(gridX, gridY, m_btnWidth, m_btnHeight);
}

int MainLayer::touchGrid(CCPoint beginTouch, CCPoint endTouch)
{
    int index = -1;
    for (vector<CCSprite*>::iterator it = m_gridButtons.begin(); it != m_gridButtons.end(); ++it)
    {
        CCSprite *button = *it;
        int tag = button->getTag();
        CCRect buttonRect = this->changeGridToThisCoordRect(tag);
        if (buttonRect.containsPoint(beginTouch) && buttonRect.containsPoint(endTouch))
        {
            index = tag;
            break;
        }
    }
    
    if (index != -1)
    {
        DataManager::sharedDataManager()->setIsVertical(false);
        
        TouchGridEvent *touchGridEvent = new TouchGridEvent();
        touchGridEvent->setIndex(index);
        
        EventManager::sharedEventManager()->addEvent(touchGridEvent);
    }
    
    return index;
}


void MainLayer::initTipsBg()
{
    m_tipsBg->setVisible(false);
}

void MainLayer::showTips(bool isShow, int phraseIndex, int phrase2Index)
{
    if (isShow)
    {
        m_tipsBg->setVisible(true);
        if (phraseIndex != -1)
        {
            Words *words = DataManager::sharedDataManager()->getWords().at(phraseIndex);
            string tip = words->getTips();
            m_tip1Bg->setVisible(true);
            m_tip1->setString(tip.c_str());
            
            showSpecialBonus(words, true);
        }
        else
        {
            m_tip1Bg->setVisible(false);
        }
        
        if (phrase2Index != -1)
        {
            Words *words = DataManager::sharedDataManager()->getWords().at(phrase2Index);
            string tip = words->getTips();
            m_tip2Bg->setVisible(true);
            m_tip2->setString(tip.c_str());
         
            showSpecialBonus(words, false);
        }
        else
        {
            m_tip2Bg->setVisible(false);
        }
    }
    else
    {
        m_tipsBg->setVisible(false);
    }
}

void MainLayer::showSpecialBonus(Words *words, bool isH)
{
    int bonusType = words->getBonusType();
    int bonusValue = words->getBonusValue();
    char bonus[10];
    GameType gameType = DataManager::sharedDataManager()->getGameType();
    
    CCSpriteFrame *frame = NULL;
    switch (bonusType)
    {
        case 1://金币或银币
        {
            if (gameType == GameTypeSingle)
            {
                frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("main_silver.png");
            }
            else
            {
                frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("main_gold.png");
            }
            
            break;
        }
            
        case 2://经验
        {
            frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("main_exp.png");
            break;
        }
    }
    
    if (isH)
    {
        if (bonusType > 0 && !words->isGet())
        {
            m_bonusSpriteH->setVisible(true);
            m_bonusSpriteH->setDisplayFrame(frame);
            
            sprintf(bonus, "%d", bonusValue);
            m_bonusBMFontH->setString(bonus);
        }
        else
        {
            m_bonusSpriteH->setVisible(false);
        }

    }
    else
    {
        if (bonusType > 0 && !words->isGet())
        {
            m_bonusSpriteV->setVisible(true);
            m_bonusSpriteV->setDisplayFrame(frame);
 
            sprintf(bonus, "%d", bonusValue);
            m_bonusBMFontV->setString(bonus);
        }
        else
        {
            m_bonusSpriteV->setVisible(false);
        }

    }
}

void MainLayer::showTouchAction(Event *event)
{
    //先清除上一次的点击效果
    hideTouchAction();
    
    TouchGridEvent* touchGridEvent = (TouchGridEvent*)event;
    
    int index = touchGridEvent->getIndex();
    vector<int> wordsIndexVector = touchGridEvent->getWordsIndexVector();
    
    //点击的grid button
    CCSprite *button = m_gridButtons.at(index);
    CCSpriteFrame *touchFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("main_grid3.png");
    button->setDisplayFrame(touchFrame);
    
    
    //显示横竖动画
    CCSpriteFrame* otherFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("main_grid2.png");
    for (vector<int>::iterator it = wordsIndexVector.begin(); it != wordsIndexVector.end(); it++)
    {
        int wordsIndex = *it;
        
        if (wordsIndex != index)//点击grid播放上面的动画
        {
            CCSprite *button = m_gridButtons.at(wordsIndex);
            button->setDisplayFrame(otherFrame);
        }
        //所有的 包括点击button的索引
        m_touchActionIndexV.push_back(wordsIndex);
    }
}

void MainLayer::hideTouchAction()
{
    int size = m_touchActionIndexV.size();
    if (size > 0)
    {
        CCSpriteFrame *frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("main_grid1.png");
        for (vector<int>::iterator it = m_touchActionIndexV.begin(); it != m_touchActionIndexV.end(); it++) {
            int index = *it;
            CCSprite *button = m_gridButtons.at(index);
            button->setDisplayFrame(frame);
        }
        m_touchActionIndexV.clear();
    }
}

void MainLayer::setAnswers(bool isShow)
{
    vector<CCControlButton*>::iterator it;
    for (it = m_answersV.begin(); it != m_answersV.end(); it++)
    {
        CCControlButton* button = *it;
        button->setVisible(isShow);
    }
}

void MainLayer::showAnswers()
{
    vector<string> answers = DataManager::sharedDataManager()->getAnswers();
    int size = answers.size();
    for (int i = 0; i < size; i++)
    {
        CCControlButton *button =  m_answersV.at(i);
        button->setVisible(true);
        button->setTitleForState(CCString::create(answers.at(i)), CCControlStateNormal);
    }
}

void MainLayer::onChooseAnswer(CCObject* pObject, CCControlEvent event)
{
    int index = -1;
    CCControlButton *cb = (CCControlButton*)pObject;
    index = cb->getTag();
    
    if (index != -1)
    {
        ChooseAnswerEvent *cae = new ChooseAnswerEvent();
        cae->setIndex(index);
        EventManager::sharedEventManager()->addEvent(cae);
    }
}

void MainLayer::showChooseAnswer(Event *event)
{
    ChooseAnswerEvent *cae = (ChooseAnswerEvent*)event;
    int answerTag = cae->getIndex();
    string answer = DataManager::sharedDataManager()->getAnswers().at(answerTag);
    
    int gridIndex = DataManager::sharedDataManager()->getTouchGridDirect()->getIndex();
    CCSprite *button = m_gridButtons.at(gridIndex);
    
    CCLabelTTF *wordLabel = (CCLabelTTF*)button->getChildByTag(99);
    if (wordLabel != NULL)
    {
        wordLabel->setString(answer.c_str());
    }
   
}

void MainLayer::setFixGridLabel(Event *event)
{
    FixAnswerEvent *fixAnswerEvent = (FixAnswerEvent*)event;
    int phraseIndex = fixAnswerEvent->getPhraseIndex();
    int phrase2Index = fixAnswerEvent->getPhrase2Index();
    
    vector<int>::iterator it;
    if (phraseIndex != -1)
    {
        vector<int> phraseWordsIndexVec = DataManager::sharedDataManager()->getPhraseWordsIndexVec();
        for (it = phraseWordsIndexVec.begin(); it != phraseWordsIndexVec.end(); it++)
        {
            int gridIndex = *it;
            CCSprite *button = m_gridButtons.at(gridIndex);
            
            CCLabelTTF *wordLabel = (CCLabelTTF*)button->getChildByTag(99);
            if (wordLabel != NULL)
            {
                wordLabel->setColor(ccBLACK);
            }
        }
    }
    
    if (phrase2Index != -1)
    {
        vector<int> phrase2WordsIndexVec = DataManager::sharedDataManager()->getPhrase2WordsIndexVec();
        for (it = phrase2WordsIndexVec.begin(); it != phrase2WordsIndexVec.end(); it++)
        {
            int gridIndex = *it;
            CCSprite *button = m_gridButtons.at(gridIndex);
            
            CCLabelTTF *wordLabel = (CCLabelTTF*)button->getChildByTag(99);
            if (wordLabel != NULL)
            {
                wordLabel->setColor(ccBLACK);
            }
        }
    }
}

bool MainLayer::checkGridIndexIsFix(int gridIndex)
{
    bool flag = false;
    
    if (gridIndex >= 0 && gridIndex < m_gridButtons.size())
    {
        vector<Answer*> selectAnswerVec = DataManager::sharedDataManager()->getSelectAnswerVec();
        vector<Answer*>::iterator it;
        for (it = selectAnswerVec.begin(); it != selectAnswerVec.end(); it++)
        {
            Answer *a = *it;
            if (a->getIndex() == gridIndex)
            {
                if (a->getIsFix())
                {
                    flag = true;
                }
                break;
            }
        }
    }
    
    return flag;
}

void MainLayer::initLocalUserProTimer()
{
    if (DataManager::sharedDataManager()->getGameType() == GameTypeSingle)
    {
        m_localUserExpPro->setVisible(false);
        m_localUserExpProTimer = CCProgressTimer::create(m_localUserExpPro);
        m_localUserExpProTimer->setType(kCCProgressTimerTypeBar);
        m_localUserExpProTimer->setMidpoint(ccp(0, 0));
        m_localUserExpProTimer->setBarChangeRate(ccp(1, 0));
        m_localUserExpProTimer->setPosition(ccp(m_localUserExpProBg->getContentSize().width / 2, m_localUserExpProBg->getContentSize().height/2));
        m_localUserExpProTimer->setPercentage(0.0f);
        m_localUserExpProBg->addChild(m_localUserExpProTimer);
    }
}


void MainLayer::updateLocalUserMsg()
{
    if (DataManager::sharedDataManager()->getGameType() == GameTypeSingle)
    {
        LocalUser *lc = DataManager::sharedDataManager()->getLocalUser();
        int silver = lc->m_silver;
        int lv = lc->m_lv;
        int exp = lc->m_exp;
        
        char data[15];
        
        sprintf(data, "%d", silver);
        m_localUserSilver->setString(data);
        
        sprintf(data, "LV.%d", lv);
        m_localUserLevel->setString(data);
        
        int expBase = DataManager::sharedDataManager()->getLocalUserExpBase(lv);
        int numerator = DataManager::sharedDataManager()->getLocalUserNumerator(exp, lv);
        sprintf(data, "%d/%d", numerator, expBase);
        m_localUserExp->setString(data);
        
        float progress = numerator * 100.0f / expBase;
        m_localUserExpProTimer->setPercentage(progress);
        
    }
    
    
}

/*
 暂时只在中间弹出银币动画 和 经验动画
 不在每个词语的位置弹
 后期需要优化这个方法 太长了
 */
void MainLayer::showLocalUserRewardOrWinLayer(Event *event)
{
    RewardEvent *re = (RewardEvent*)event;
    int bonusType = re->getBonusType();
    switch (bonusType)
    {
        case 1:
        {
            //银币
            CCSpriteFrame *frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("silver_icon.png");
            CCSprite *silverSprite = CCSprite::createWithSpriteFrame(frame);
            
            CCBlink *blink = CCBlink::create(1.5f, 5);
            CCCallFuncN *blinkDone = CCCallFuncN::create(this, callfuncN_selector(MainLayer::blinkDone));
            CCSequence* sequence = CCSequence::createWithTwoActions(blink, blinkDone);
            silverSprite->runAction(sequence);
            
            silverSprite->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width * 0.5f, CCDirector::sharedDirector()->getWinSize().height * 0.5f));
            this->addChild(silverSprite);
            
            break;
        }
        case 2:
        {
            //经验
            CCSpriteFrame *frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("sublevel_star.png");
            CCSprite *expSprite = CCSprite::createWithSpriteFrame(frame);
            
            CCBlink *blink = CCBlink::create(1.5f, 5);
            CCCallFuncN *blinkDone = CCCallFuncN::create(this, callfuncN_selector(MainLayer::blinkDone));
            CCSequence* sequence = CCSequence::createWithTwoActions(blink, blinkDone);
            expSprite->runAction(sequence);
            
            expSprite->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width * 0.5f, CCDirector::sharedDirector()->getWinSize().height * 0.5f));
            this->addChild(expSprite);
            
            break;
        }
        case 3:
        {
            //经验 和 银币
            CCSpriteFrame *frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("silver_icon.png");
            CCSprite *silverSprite = CCSprite::createWithSpriteFrame(frame);
            
            CCBlink *blink = CCBlink::create(1.5f, 5);
            CCCallFuncN *blinkDone = CCCallFuncN::create(this, callfuncN_selector(MainLayer::blinkDone));
            CCSequence* sequence = CCSequence::createWithTwoActions(blink, blinkDone);
            silverSprite->runAction(sequence);
            
            silverSprite->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width * 0.45f, CCDirector::sharedDirector()->getWinSize().height * 0.5f));
            this->addChild(silverSprite);
            
            
            frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("sublevel_star.png");
            CCSprite *expSprite = CCSprite::createWithSpriteFrame(frame);
            
            blink = CCBlink::create(1.5f, 5);
            blinkDone = CCCallFuncN::create(this, callfuncN_selector(MainLayer::blinkDone));
            sequence = CCSequence::createWithTwoActions(blink, blinkDone);
            expSprite->runAction(sequence);
            
            expSprite->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width * 0.55f, CCDirector::sharedDirector()->getWinSize().height * 0.5f));
            this->addChild(expSprite);
            
            break;
        }
        case 4:
        {
            //弹结算框
            CGDialog::show(GameOKCancelButtonType, "reward_txt", this, menu_selector(MainLayer::onOk), NULL);
            break;
        }
            
        default:
            break;
    }
    
    //如果升级 升级动画
    if (DataManager::sharedDataManager()->getLocalUser()->m_isLevelUp)
    {
        CCLog("level up~~~");
    }
}

void MainLayer::blinkDone(CCNode* parent)
{
    parent->removeFromParentAndCleanup(true);
}

void MainLayer::onOk(CCObject* obj)
{
    
    GameType gameType = DataManager::sharedDataManager()->getGameType();
    int level = DataManager::sharedDataManager()->getLevel();
    
    if (gameType == GameTypeSingle)
    {
        int subLevelNum = 1;
        switch (level)
        {
            case 0:
                subLevelNum = DataManager::sharedDataManager()->getLocalUnLockLevel()->m_level0Num;
                break;
            case 1:
                subLevelNum = DataManager::sharedDataManager()->getLocalUnLockLevel()->m_level1Num;
                break;
            case 2:
                subLevelNum = DataManager::sharedDataManager()->getLocalUnLockLevel()->m_level2Num;
                break;
            default:
                subLevelNum = DataManager::sharedDataManager()->getLocalUnLockLevel()->m_level0Num;
                break;
        }
        
        int subLevel = DataManager::sharedDataManager()->getSingleSubLevel();
        //下一关逻辑
        subLevel++;
        if (subLevel >= subLevelNum)
        {
            const char* text = Localize::sharedLocalize()->getString("toast_txt9");
            CGToast *toast = CGToast::create();
            toast->setText(text);
            toast->playAction();
            this->addChild(toast);
        }
        else
        {
            DataManager::sharedDataManager()->setSingleSubLevel(subLevel);
            
            Event *e = new Event(EventTypeGameStart);
            EventManager::sharedEventManager()->addEvent(e);
        }
        
    }
    else if (gameType == GameTypeCompetitive)
    {
        
    }
    else if (gameType == GameTypeCooperation)
    {
        
    }
    
    
}

void MainLayer::loadSingleSelectAnswer()
{
    GameType gameType = DataManager::sharedDataManager()->getGameType();
    if (gameType == GameTypeSingle)
    {
        vector<Answer*> lastSelectAnswerVec = DataManager::sharedDataManager()->getSelectAnswerVec();
        vector<Answer*>::iterator answerIt;
        for (answerIt = lastSelectAnswerVec.begin(); answerIt != lastSelectAnswerVec.end(); answerIt++)
        {
            Answer *a = *answerIt;
            
            CCSprite *button = m_gridButtons.at(a->getIndex());//按钮的索引位置
            CCLabelTTF *wordLabel = (CCLabelTTF*)button->getChildByTag(99);
            
            if (wordLabel != NULL)
            {
                if (a->getPhraseIndex() != -1)
                {
                    if (a->getIsFix())
                    {
                        wordLabel->setColor(ccBLACK);
                        wordLabel->setString(a->getAnswerWord().c_str());
                    }
                    else
                    {
                        wordLabel->setString(a->getAnswerWord().c_str());
                    }
                }
                
                if (a->getPhrase2Index() != -1)
                {
                    if (a->getIsFix())
                    {
                        wordLabel->setColor(ccBLACK);
                        wordLabel->setString(a->getAnswerWord().c_str());
                    }
                    else
                    {
                        wordLabel->setString(a->getAnswerWord().c_str());
                    }
                }
            }
            
            
            
        }
        
    }
}


void MainLayer::clearSingleSelectAnswer()
{
    GameType gameType = DataManager::sharedDataManager()->getGameType();
    if (gameType == GameTypeSingle)
    {
        vector<CCSprite*>::iterator buttonIt;
        for (buttonIt = m_gridButtons.begin(); buttonIt != m_gridButtons.end(); buttonIt++)
        {
            CCSprite *button = *buttonIt;
            CCLabelTTF *wordLabel = (CCLabelTTF*)button->getChildByTag(99);
            if (wordLabel != NULL)
            {
                wordLabel->setString("");
                wordLabel->setColor(ccBLUE);
            }
        }
    }
}

//优化一下
void MainLayer::updateMainLayer()
{
    GameType gameType = DataManager::sharedDataManager()->getGameType();
    
    if (gameType == GameTypeCompetitive)//竞技
    {
        //进度信息
        vector<int> chessVec = DataManager::sharedDataManager()->getChessVec();
//        vector<int> ownChessVec = DataManager::sharedDataManager()->getOwnChessVec();
        
        int chessFixNum = 0;
        vector<int>::iterator it;
        for (it = chessVec.begin(); it != chessVec.end(); it++)
        {
            int c = *it;
            if (c == 1)
            {
                chessFixNum++;
            }
        }

        int size = chessVec.size();
        float otherCompPer = chessFixNum * 100.0f / chessVec.size();
        m_otherUserGameProTimer->setPercentage(otherCompPer);
        
        char otherCompChar[15];
        sprintf(otherCompChar, "%d / %d", chessFixNum, size);
        m_otherUserGame->setString(otherCompChar);
        
        
        int ownChessFixNum = DataManager::sharedDataManager()->getRightWordsIndexVec().size();
        float ownCompPer = ownChessFixNum * 100.0f / size;
        m_ownUserGameProTimer->setPercentage(ownCompPer);
        
        char ownCompChar[15];
        sprintf(ownCompChar, "%d / %d", ownChessFixNum, size);
        m_ownUserGame->setString(ownCompChar);
        
        
        
        //玩家自身信息
        OnLineUser *ownOnLineUser = DataManager::sharedDataManager()->getOwnOnLineUser();
        m_ownName->setString(ownOnLineUser->m_nick.c_str());
        
        char msgChar[15];
        sprintf(msgChar, "LV.%d", ownOnLineUser->m_lv);
        m_ownLv->setString(msgChar);
        
        sprintf(msgChar, "%d", ownOnLineUser->m_gold);//使用道具减少的m_gold需要传过来
        m_ownUserGold->setString(msgChar);
        
        sprintf(msgChar, "%d", ownOnLineUser->m_rewardGold);
        m_ownRewardGold->setString(msgChar);
        
        sprintf(msgChar, "%d", ownOnLineUser->m_rewardExp);
        m_ownRewardExp->setString(msgChar);
        
        
        
        
        //other msg
        OnLineUser *otherOnLineUser = DataManager::sharedDataManager()->getOtherOnLineUserVec().at(0);
        m_otherName->setString(otherOnLineUser->m_nick.c_str());
        
        sprintf(msgChar, "LV.%d", otherOnLineUser->m_lv);
        m_otherLv->setString(msgChar);
        
        sprintf(msgChar, "%d", otherOnLineUser->m_gold);
        m_otherUserGold->setString(msgChar);
        
        sprintf(msgChar, "%d", otherOnLineUser->m_rewardGold);
        m_otherRewardGold->setString(msgChar);
        
        sprintf(msgChar, "%d", otherOnLineUser->m_rewardExp);
        m_otherRewardExp->setString(msgChar);

    }
    else if (gameType == GameTypeCooperation)
    {
        //取对方的应该是最新的
        vector<int> chessVec = DataManager::sharedDataManager()->getChessVec();
        
        int chessFixNum = 0;
        int fixIndex = 0;
        vector<int>::iterator it;
        for (it = chessVec.begin(); it != chessVec.end(); it++)
        {

            int c = *it;
            if (c == 1)
            {
                chessFixNum++;
                
                //此时把对方玩家完成的成语 也放入m_rightWordsIndexVec里了 所以计算自身完成多少的时候应该用m_ownChessVec
                this->showPartnerFixAnswer(fixIndex);
            }
            
            fixIndex++;
        }
        
        int size = chessVec.size();
        
        char msgChar[15];
        
        //玩家自身信息
        OnLineUser *ownOnLineUser = DataManager::sharedDataManager()->getOwnOnLineUser();
        m_ownName->setString(ownOnLineUser->m_nick.c_str());
        
        sprintf(msgChar, "LV.%d", ownOnLineUser->m_lv);
        m_ownLv->setString(msgChar);
        
        sprintf(msgChar, "%d", ownOnLineUser->m_gold);//使用道具减少的m_gold需要传过来
        m_ownUserGold->setString(msgChar);
        
        sprintf(msgChar, "%d", ownOnLineUser->m_rewardGold);
        m_ownRewardGold->setString(msgChar);
        
        sprintf(msgChar, "%d", ownOnLineUser->m_rewardExp);
        m_ownRewardExp->setString(msgChar);
        
        int ownChessFixNum = DataManager::sharedDataManager()->getCoopOwnRightWordsIndexVec().size();
        sprintf(msgChar, "%d", ownChessFixNum);
        m_ownComNum->setString(msgChar);
        
        
        
        //other msg
        OnLineUser *otherOnLineUser = DataManager::sharedDataManager()->getOtherOnLineUserVec().at(0);
        m_otherName->setString(otherOnLineUser->m_nick.c_str());
        
        sprintf(msgChar, "LV.%d", otherOnLineUser->m_lv);
        m_otherLv->setString(msgChar);
        
        sprintf(msgChar, "%d", otherOnLineUser->m_gold);
        m_otherUserGold->setString(msgChar);
        
        sprintf(msgChar, "%d", otherOnLineUser->m_rewardGold);
        m_otherRewardGold->setString(msgChar);
        
        sprintf(msgChar, "%d", otherOnLineUser->m_rewardExp);
        m_otherRewardExp->setString(msgChar);
        
        sprintf(msgChar, "%d", chessFixNum);
        m_otherComNum->setString(msgChar);
        
        //总共完成的
        sprintf(msgChar, "%d / %d", chessFixNum + ownChessFixNum, size);
        m_allComNum->setString(msgChar);
    }
}

/*
 1.应该比较和自己m_rightWordsIndexVec不同的索引
 2.然后对不同索引的成语 显示动画
 3.最后把该索引放入m_rightWordsIndexVec 同时设置m_selectAnswerVec 这样当点击棋盘对应点时 才不会做错误的处理
 */
void MainLayer::showPartnerFixAnswer(int fixIndex)
{
    vector<int> ownRightWordsIndexVec = DataManager::sharedDataManager()->getRightWordsIndexVec();
    vector<int>::iterator it;
    for (it = ownRightWordsIndexVec.begin(); it != ownRightWordsIndexVec.end(); it++)
    {
        int index = *it;
        if (index == fixIndex)
        {
            return;
        }
    }
    
    //2
    vector<Grid*> gridsTemp = DataManager::sharedDataManager()->getGrids();
    vector<Answer*> selectAnswerVec = DataManager::sharedDataManager()->getSelectAnswerVec();
    
    for (vector<Grid*>::iterator it = gridsTemp.begin(); it != gridsTemp.end(); ++it)
    {
        Grid *grid = *it;
        int gridPhraseIndex = grid->getPhraseIndex();
        int gridPhrase2Index = grid->getPhrase2Index();
        
        if (fixIndex == gridPhraseIndex || fixIndex == gridPhrase2Index)
        {
            int gridIndex = grid->getIndex();
            CCSprite *button = m_gridButtons.at(gridIndex);
            
            CCLabelTTF *wordLabel = (CCLabelTTF*)button->getChildByTag(99);
            wordLabel->setColor(ccBLACK);
            string wordTemp = "";
            if (wordLabel != NULL)//显示动画的地方
            {
                Words *fixWords = DataManager::sharedDataManager()->getWords().at(fixIndex);
                vector<string> wordVec = Utilities::splitString(fixWords->getName(), "*");
                
                if (fixIndex == gridPhraseIndex)
                {
                    int gridWordIndex = grid->getWordIndex();
                    wordTemp = wordVec.at(gridWordIndex);
                    wordLabel->setString(wordTemp.c_str());
                }
                else
                {
                    int gridWord2Index = grid->getWord2Index();
                    wordTemp = wordVec.at(gridWord2Index);
                    wordLabel->setString(wordTemp.c_str());
                }
            }
            
            
            //3 设置m_selectAnswerVec
            for (vector<Answer*>::iterator itAnswer = selectAnswerVec.begin(); itAnswer != selectAnswerVec.end(); itAnswer++)
            {
                Answer *a = *itAnswer;
                if (a->getIndex() == gridIndex)
                {
                    a->setAnswerWord(wordTemp);
                    a->setIsSame();
                    a->setIsFix(true);
                    break;
                }
            }
            
            
        }
        
    }
    
    //3 
    DataManager::sharedDataManager()->setRightWordsIndexVec(fixIndex);
    
}


void MainLayer::initCompCompleteProTimer()
{
    if (DataManager::sharedDataManager()->getGameType() == GameTypeCompetitive)
    {
        int level = DataManager::sharedDataManager()->getLevel();
        int allNum = 10;//这里不对 应该取真正的值
        switch (level) {
            case 0:
                allNum = 10;
                break;
            case 1:
                allNum = 15;
                break;
            case 2:
                allNum = 20;
                break;
                
            default:
                break;
        }
        char completeChar[15];
        sprintf(completeChar, "0/%d", allNum);
        m_ownUserGame->getParent()->reorderChild(m_ownUserGame, 6);
        m_ownUserGame->setString(completeChar);
        m_otherUserGame->getParent()->reorderChild(m_otherUserGame, 6);
        m_otherUserGame->setString(completeChar);
        
        m_ownUserGamePro->setVisible(false);
        m_ownUserGameProTimer = CCProgressTimer::create(m_ownUserGamePro);
        m_ownUserGameProTimer->setType(kCCProgressTimerTypeBar);
        m_ownUserGameProTimer->setMidpoint(ccp(0, 0));
        m_ownUserGameProTimer->setBarChangeRate(ccp(1, 0));
        m_ownUserGameProTimer->setPosition(ccp(m_ownUserGameProBg->getContentSize().width / 2, m_ownUserGameProBg->getContentSize().height/2));
        m_ownUserGameProTimer->setPercentage(0.0f);
        m_ownUserGameProBg->addChild(m_ownUserGameProTimer);
        
        
        m_otherUserGamePro->setVisible(false);
        m_otherUserGameProTimer = CCProgressTimer::create(m_otherUserGamePro);
        m_otherUserGameProTimer->setType(kCCProgressTimerTypeBar);
        m_otherUserGameProTimer->setMidpoint(ccp(0, 0));
        m_otherUserGameProTimer->setBarChangeRate(ccp(1, 0));
        m_otherUserGameProTimer->setPosition(ccp(m_otherUserGameProBg->getContentSize().width / 2, m_otherUserGameProBg->getContentSize().height/2));
        m_otherUserGameProTimer->setPercentage(0.0f);
        m_otherUserGameProBg->addChild(m_otherUserGameProTimer);
        
    }
}

void MainLayer::initCompCoopUserMsg()
{

    if (DataManager::sharedDataManager()->getGameType() == GameTypeCompetitive || DataManager::sharedDataManager()->getGameType() == GameTypeCooperation)
    {
        OnLineUser *ownOnLineUser = DataManager::sharedDataManager()->getOwnOnLineUser();
        m_ownName->setString(ownOnLineUser->m_nick.c_str());
        
        char msgChar[15];
        sprintf(msgChar, "LV.%d", ownOnLineUser->m_lv);
        m_ownLv->setString(msgChar);
        
        sprintf(msgChar, "%d", ownOnLineUser->m_gold);
        m_ownUserGold->setString(msgChar);
        
        sprintf(msgChar, "%d", ownOnLineUser->m_rewardGold);
        m_ownRewardGold->setString(msgChar);
        
        sprintf(msgChar, "%d", ownOnLineUser->m_rewardExp);
        m_ownRewardExp->setString(msgChar);
        
        
        //other msg
        OnLineUser *otherOnLineUser = DataManager::sharedDataManager()->getOtherOnLineUserVec().at(0);
        m_otherName->setString(otherOnLineUser->m_nick.c_str());
        
        sprintf(msgChar, "LV.%d", otherOnLineUser->m_lv);
        m_otherLv->setString(msgChar);
        
        sprintf(msgChar, "%d", otherOnLineUser->m_gold);
        m_otherUserGold->setString(msgChar);
        
        sprintf(msgChar, "%d", otherOnLineUser->m_rewardGold);
        m_otherRewardGold->setString(msgChar);
        
        sprintf(msgChar, "%d", otherOnLineUser->m_rewardExp);
        m_otherRewardExp->setString(msgChar);

    }
    
}

void MainLayer::initCoopTimeProTimer()
{
    if (DataManager::sharedDataManager()->getGameType() == GameTypeCooperation)
    {
        m_timePro->setVisible(false);
        m_timeProTimer = CCProgressTimer::create(m_timePro);
        m_timeProTimer->setType(kCCProgressTimerTypeBar);
        m_timeProTimer->setMidpoint(ccp(0, 0));
        m_timeProTimer->setBarChangeRate(ccp(1, 0));
        m_timeProTimer->setPosition(ccp(m_timeProBg->getContentSize().width * 0.5f, m_timeProBg->getContentSize().height  * 0.5f));
        m_timeProTimer->setPercentage(100.0f);
        m_timeProBg->addChild(m_timeProTimer);
        
        m_timeProPoint->getParent()->reorderChild(m_timeProPoint, 6);
    }
}

void MainLayer::coopCountTime(float dt)
{
    float coopTime = DataManager::sharedDataManager()->getCoopTime();
    int showCoopTime = coopTime - 1;//(int)dt; 1秒一次 近似直接减1 然后在onGameTime里纠正
    DataManager::sharedDataManager()->setCoopTime(showCoopTime);
    
    m_coopTime->setString(Utilities::getTimeString(showCoopTime));
    
    float oriCoopTime = DataManager::sharedDataManager()->getOriCoopTime();
    float per = 100 * DataManager::sharedDataManager()->getCoopTime() / oriCoopTime;
    m_timeProTimer->setPercentage(per);
    
    m_timeProPoint->setPosition(ccp(m_timeProBg->getContentSize().width * per / 100, m_timeProBg->getContentSize().height  * 0.5f));

}

void MainLayer::handleGameStop(Event *event)
{
    GameStopEventEx *gameStopEventEx = (GameStopEventEx*)event;
    GameType gameType = DataManager::sharedDataManager()->getGameType();
    
    int flag = gameStopEventEx->getFlag();
    switch (flag)
    {
        case 0:
        {
            bool isWin = gameStopEventEx->getIsWin();
            //提示不同的语言
            //0 1 种情况  弹出结算框   提示谁赢谁输   点击确定进入下一次等待  取消回到对应的竞技合作难度选择界面
            //最后要换成结算框  这里不需要getInfo 因为gamestop时 已经把奖励放进m_ownOnLineUser了
            if (gameType == GameTypeCooperation && isWin)//合作胜利
            {
                CGDialog::show(GameOKCancelButtonType, "dialog_coop_win", this, menu_selector(MainLayer::onCoopOk), menu_selector(MainLayer::onCoopCancel));
            }
            else if (gameType == GameTypeCompetitive && isWin)//竞技自己胜利
            {
                CGDialog::show(GameOKCancelButtonType, "dialog_comp_win", this, menu_selector(MainLayer::onCompOk), menu_selector(MainLayer::onCompCancel));
            }
            else if (gameType == GameTypeCompetitive && !isWin)
            {
                CGDialog::show(GameOKCancelButtonType, "dialog_comp_lose", this, menu_selector(MainLayer::onCompOk), menu_selector(MainLayer::onCompCancel));
            }
            
            break;
        }
            
        case 1:
        {
            //合作模式倒计时时间到
            if (gameType == GameTypeCooperation)
            {
                CGDialog::show(GameOKCancelButtonType, "dialog_coop_lose", this, menu_selector(MainLayer::onCoopOk), menu_selector(MainLayer::onCoopCancel));
                
                
                //加上onExit()移除了两次 不知道是否有问题
                CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(MainLayer::coopCountTime), this);
                m_coopTime->setString(Utilities::getTimeString(0));
                m_timeProTimer->setPercentage(0);
                m_timeProPoint->setPosition(ccp(0, m_timeProBg->getContentSize().height  * 0.5f));
            }
            break;
        }
            
        case 2:
        {
            //对方逃跑
            if (gameType == GameTypeCompetitive)
            {
                CGDialog::show(GameOKCancelButtonType, "dialog_comp_escape_win", this, menu_selector(MainLayer::onCompOk), menu_selector(MainLayer::onCompCancel));
            }
            break;
        }
            
        default:
            break;
    }
}

void MainLayer::onCompOk(CCObject* obj)
{
    GameType gameType = DataManager::sharedDataManager()->getGameType();
    int level = DataManager::sharedDataManager()->getLevel();
    
    //gamestop的时候没有kick 需要自己先situp 再发送坐下消息
    SitUpEvent *sue = new SitUpEvent(gameType, level);//1-竞技 2-合作
    EventManager::sharedEventManager()->addEvent(sue);
    
    SitDownEvent *sde = new SitDownEvent(gameType, level);
    EventManager::sharedEventManager()->addEvent(sde);
}

void MainLayer::onCompCancel(CCObject* obj)
{
    GameType gameType = DataManager::sharedDataManager()->getGameType();
    int level = DataManager::sharedDataManager()->getLevel();
    
    //gamestop的时候没有kick 需要自己先situp 再回到竞技页面  免得发送坐下消息有异常
    SitUpEvent *sue = new SitUpEvent(gameType, level);//1-竞技 2-合作
    EventManager::sharedEventManager()->addEvent(sue);
    
    Event *e = new Event(EventTypeEnterCompetitiveGame);
    EventManager::sharedEventManager()->addEvent(e);
}

void MainLayer::onCoopOk(CCObject* obj)
{
    GameType gameType = DataManager::sharedDataManager()->getGameType();
    int level = DataManager::sharedDataManager()->getLevel();
    
    //gamestop的时候没有kick 需要自己先situp 再发送坐下消息
    SitUpEvent *sue = new SitUpEvent(gameType, level);//1-竞技 2-合作
    EventManager::sharedEventManager()->addEvent(sue);
    
    SitDownEvent *sde = new SitDownEvent(gameType, level);
    EventManager::sharedEventManager()->addEvent(sde);
}

void MainLayer::onCoopCancel(CCObject* obj)
{
    GameType gameType = DataManager::sharedDataManager()->getGameType();
    int level = DataManager::sharedDataManager()->getLevel();
    
    //gamestop的时候没有kick 需要自己先situp 再回到竞技页面  免得发送坐下消息有异常
    SitUpEvent *sue = new SitUpEvent(gameType, level);//1-竞技 2-合作
    EventManager::sharedEventManager()->addEvent(sue);
    
    Event *e = new Event(EventTypeEnterCooperationGame);
    EventManager::sharedEventManager()->addEvent(e);
}

void MainLayer::onBackOk(CCObject* obj)
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
        //sit up
        SitUpEvent *sue = new SitUpEvent(gameType, level);//1-竞技 2-合作
        EventManager::sharedEventManager()->addEvent(sue);
        
        Event *e = new Event(EventTypeEnterCooperationGame);
        EventManager::sharedEventManager()->addEvent(e);
    }
}

void MainLayer::onBack(CCObject* pObject, CCControlEvent event)
{
    CGDialog::show(GameOKCancelButtonType, "dialog_txt", this, menu_selector(MainLayer::onBackOk), NULL);
}

void MainLayer::onChooseTool(CCObject* pObject, CCControlEvent event)
{
    int index = -1;
    CCControlButton *cb = (CCControlButton*)pObject;
    index = cb->getTag();
    
    if (index != -1)
    {
        CCLog("I AM TOOL %d", index);//58 118 132
    }
}
