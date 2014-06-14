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
    m_beginTouchTemp = CCPointZero;
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
    m_bonusSpriteH = NULL;
    m_bonusBMFontH = NULL;
    m_bonusSpriteV = NULL;
    m_bonusBMFontV = NULL;
    
    m_touchGridActionSprite = NULL;
    
    for (int i = 0; i < ANSWER_NUM; i++)
    {
        m_answersV.push_back(CCLabelTTF::create());
        m_answersBgV.push_back(CCSprite::create());
    }
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
    CC_SAFE_RELEASE_NULL(m_bonusSpriteH);
    CC_SAFE_RELEASE_NULL(m_bonusBMFontH);
    CC_SAFE_RELEASE_NULL(m_bonusSpriteV);
    CC_SAFE_RELEASE_NULL(m_bonusBMFontV);
    
    vector<CCLabelTTF*>::iterator it;
    for (it = m_answersV.begin(); it != m_answersV.end(); it++)
    {
        CCLabelTTF* label = *it;
        CC_SAFE_RELEASE_NULL(label);
    }
    
    vector<CCSprite*>::iterator itSprite;
    for (itSprite = m_answersBgV.begin(); itSprite != m_answersBgV.end(); itSprite++)
    {
        CCSprite* sprite = *itSprite;
        CC_SAFE_RELEASE_NULL(sprite);
    }
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
    CCNode* node = reader.readCCBFile("main.ccbi", this);
    addChild(node);
    
    m_ccbScale = reader.getCCBScale();
    
    this->setTouchEnabled(true);
    this->setKeypadEnabled(true);
    
    m_mainBorderLayer = MainBorderLayer::create();
    this->addChild(m_mainBorderLayer);
    
    
    m_size = this->getContentSize();
    m_ignoreDis = CCDirector::sharedDirector()->getWinSize().width * 0.005f;
    
    
    m_line = DataManager::sharedDataManager()->getLine();
    m_col = DataManager::sharedDataManager()->getCol();
    
    
    
    
    initGridButtons();
    initMoveDirection();
    initXYRange();
    
    initTipsBg();
    
    initTouchGridActionSprite();
    
    setAnswers(false);
    
    loadSingleSelectAnswer();
    
    
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
}

void MainLayer::onExit()
{
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
   CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onAnswer0", MainLayer::onAnswer0);
    
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
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_bonusSpriteH", CCSprite*, m_bonusSpriteH);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_bonusBMFontH", CCLabelBMFont*, m_bonusBMFontH);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_bonusSpriteV", CCSprite*, m_bonusSpriteV);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_bonusBMFontV", CCLabelBMFont*, m_bonusBMFontV);


    char name[15];
    for (int i = 0; i < ANSWER_NUM; i++)
    {
        sprintf(name, "m_answer%d", i);
        CCB_MEMBERVARIABLEASSIGNER_GLUE(this, name, CCLabelTTF*, m_answersV.at(i));
        
        sprintf(name, "m_answer%dBg", i);
        CCB_MEMBERVARIABLEASSIGNER_GLUE(this, name, CCSprite*, m_answersBgV.at(i));
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
    m_beginTouchTemp = pTouch->getLocation();

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
    CCLog("TOUCH ANSWERBG INDEX IS %d", this->touchAnswerBg(m_beginTouchTemp, m_endTouch));
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
                m_mainBorderLayer->updateLocalUserData();
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
        CCLabelTTF *label = CCLabelTTF::create(letter, "Cochin", 20);
        label->setPosition(ccp(letterBeginPosX + i * m_btnWidth, letterPosY));
        label->setColor(ccBLUE);
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
        CCLabelTTF *label = CCLabelTTF::create(number, "Cochin", 20);
        label->setPosition(ccp(numberPosX, numberBeginPosY - i * m_btnHeight));
        label->setColor(ccBLUE);
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
            
            showCountDownBonus(words, true);
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
         
            showCountDownBonus(words, false);
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

void MainLayer::showCountDownBonus(Words *words, bool isH)
{
    int bonusType = words->getBonusType();
    int bonusValue = words->getBonusValue();
    char bonus[10];
    
    CCSpriteFrame *frame = NULL;
    switch (bonusType)
    {
        case 1:
        {
            frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("silver_icon.png");
            break;
        }
            
        case 2:
        {
            frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("sublevel_star.png");
            break;
        }
            
        default:
            frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("silver_icon.png");
            break;
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

void MainLayer::initTouchGridActionSprite()
{
    /*
    CCSpriteFrame* frame = NULL;
    frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("wait01.png");
    m_touchGridActionSprite = CCSprite::createWithSpriteFrame(frame);

    CCAnimate* animate = Utilities::createAnimate("wait", 14, 0.1f);
    m_touchGridActionSprite->runAction(CCRepeatForever::create(animate));
    
    m_touchGridActionSprite->setVisible(false);
    m_touchGridActionSprite->setColor(ccRED);
    */
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
    
    /*
    TouchGridEvent* touchGridEvent = (TouchGridEvent*)event;
    
    int index = touchGridEvent->getIndex();
    vector<int> wordsIndexVector = touchGridEvent->getWordsIndexVector();
    
    m_touchGridActionSprite->setVisible(true);
    m_touchGridActionSprite->removeFromParentAndCleanup(false);
    
    CCSprite *button = m_gridButtons.at(index);
    m_touchGridActionSprite->setPosition(ccp(button->getContentSize().width * 0.5f, button->getContentSize().height * 0.5f));
    button->addChild(m_touchGridActionSprite);
    
    
    //显示横竖动画
    int size = m_wordsActionSpriteV.size();
    if (size > 0)
    {
        for (vector<CCSprite*>::iterator it = m_wordsActionSpriteV.begin(); it != m_wordsActionSpriteV.end(); it++) {
            CCSprite *sprite = *it;
            sprite->removeFromParentAndCleanup(true);
        }
        m_wordsActionSpriteV.clear();
    }
    
    CCSpriteFrame* frame = NULL;
    frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("wait01.png");
    for (vector<int>::iterator it = wordsIndexVector.begin(); it != wordsIndexVector.end(); it++)
    {
        int wordsIndex = *it;
        
        if (wordsIndex != index)//点击grid播放上面的动画
        {
            CCSprite *sprite = CCSprite::createWithSpriteFrame(frame);
            CCAnimate* animate = Utilities::createAnimate("wait", 14, 0.1f);
            sprite->runAction(CCRepeatForever::create(animate));
            
            
            CCSprite *button = m_gridButtons.at(wordsIndex);
            sprite->setPosition(ccp(button->getContentSize().width * 0.5f, button->getContentSize().height * 0.5f));
            button->addChild(sprite);
            
            m_wordsActionSpriteV.push_back(sprite);
        }
    }
    */
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
    
    /*
    m_touchGridActionSprite->setVisible(false);
    
    int size = m_wordsActionSpriteV.size();
    if (size > 0)
    {
        for (vector<CCSprite*>::iterator it = m_wordsActionSpriteV.begin(); it != m_wordsActionSpriteV.end(); it++) {
            CCSprite *sprite = *it;
            sprite->removeFromParentAndCleanup(true);
        }
        m_wordsActionSpriteV.clear();
    }*/
}

void MainLayer::setAnswers(bool isShow)
{
    vector<CCSprite*>::iterator it;
    for (it = m_answersBgV.begin(); it != m_answersBgV.end(); it++)
    {
        CCSprite* bg = *it;
        bg->setVisible(isShow);
    }
}

void MainLayer::showAnswers()
{
    vector<string> answers = DataManager::sharedDataManager()->getAnswers();
    int size = answers.size();
    for (int i = 0; i < size; i++)
    {
        m_answersBgV.at(i)->setVisible(true);
        
        m_answersV.at(i)->setVisible(true);
        m_answersV.at(i)->setString(answers.at(i).c_str());
    }
}

CCRect MainLayer::changeAnswerBgToThisCoordRect(int index)
{
    float bottomLayerWidth = m_bottomLayer->getContentSize().width;//在其他分辨率手机上已经自动缩放了 看ccb我也不知道为什么
    float bottomLayerHeight = m_bottomLayer->getContentSize().height;
    
    CCSize answerBgSize = m_answersBgV.at(0)->getContentSize();
    float answerBgWidth = answerBgSize.width * m_ccbScale;//需要乘以m_ccbScale 否则在其他分辨率的手机上不准
    float answerBgHeight = answerBgSize.height * m_ccbScale;
    
    //m_bottomLayer左下角的坐标
    CCPoint bottomPosLocInThis = ccp(m_size.width * 0.125f, 0);
    
    //answerBg左下角坐标在this中的位置
    float answerBgX = bottomPosLocInThis.x + bottomLayerWidth * 0.02f + index * bottomLayerWidth * 0.12f;
    float answerBgY = (bottomLayerHeight - answerBgHeight) * 0.5f;
    
    return CCRectMake(answerBgX, answerBgY, answerBgWidth, answerBgHeight);
}

int MainLayer::touchAnswerBg(CCPoint beginTouch, CCPoint endTouch)
{
    int index = -1;
    for (vector<CCSprite*>::iterator it = m_answersBgV.begin(); it != m_answersBgV.end(); ++it)
    {
        CCSprite *answerBg = *it;
        
        if (!answerBg->isVisible())
        {
            break;
        }
        
        int tag = answerBg->getTag();
        CCRect answerBgRect = this->changeAnswerBgToThisCoordRect(tag);
        if (answerBgRect.containsPoint(beginTouch) && answerBgRect.containsPoint(endTouch))
        {
            index = tag;
            break;
        }
    }
    
    if (index != -1)
    {
        ChooseAnswerEvent *cae = new ChooseAnswerEvent();
        cae->setIndex(index);
        EventManager::sharedEventManager()->addEvent(cae);
    }
    
    return index;
}

void MainLayer::onAnswer0(CCObject* pObject, CCControlEvent event)
{
    CCControlButton *cb = (CCControlButton*)pObject;
    CCLog("%s......", cb->getTitleForState(CCControlStateHighlighted)->getCString());
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
        CCLog("gridIndex %d wordLabel is not NULL......", gridIndex);
    }
    else
    {
        CCLog("gridIndex %d wordLabel is NULL......", gridIndex);
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
    
    //先站起然后回到对应的界面
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
//        float per = chessFixNum / chessVec.size();
//        float ownPer = DataManager::sharedDataManager()->getRightWordsIndexVec().size() / chessVec.size();
        
        char perArr[9];
        sprintf(perArr, "%d / %d", chessFixNum, size);
        
        int ownChessFixNum = DataManager::sharedDataManager()->getRightWordsIndexVec().size();
        char ownPerArr[9];
        sprintf(ownPerArr, "%d / %d", ownChessFixNum, size);
        
        OnLineUser *ownOnLineUser = DataManager::sharedDataManager()->getOwnOnLineUser();
        char ownInfo[30];
        sprintf(ownInfo, "info:[username:%s, gold:%d, exp:%d]", ownOnLineUser->m_username.c_str(), ownOnLineUser->m_gold, ownOnLineUser->m_exp);
        
        OnLineUser *otherOnLineUser = DataManager::sharedDataManager()->getOtherOnLineUserVec().at(0);
        char otherInfo[30];
        sprintf(otherInfo, "info:[username:%s, gold:%d, exp:%d]", otherOnLineUser->m_username.c_str(), otherOnLineUser->m_gold, otherOnLineUser->m_exp);
        
        m_mainBorderLayer->showPer(perArr, otherInfo, ownPerArr, ownInfo);
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
                
                this->showPartnerFixAnswer(fixIndex);
            }
            
            fixIndex++;
        }
        
        int size = chessVec.size();
        
        char perArr[9];
        sprintf(perArr, "%d / %d", chessFixNum, size);
        
        OnLineUser *ownOnLineUser = DataManager::sharedDataManager()->getOwnOnLineUser();
        char ownInfo[30];
        sprintf(ownInfo, "info:[username:%s, gold:%d, exp:%d]", ownOnLineUser->m_username.c_str(), ownOnLineUser->m_gold, ownOnLineUser->m_exp);
        
        OnLineUser *otherOnLineUser = DataManager::sharedDataManager()->getOtherOnLineUserVec().at(0);
        char otherInfo[30];
        sprintf(otherInfo, "info:[username:%s, gold:%d, exp:%d]", otherOnLineUser->m_username.c_str(), otherOnLineUser->m_gold, otherOnLineUser->m_exp);
        
        m_mainBorderLayer->showPer(perArr, otherInfo, "", ownInfo);
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
