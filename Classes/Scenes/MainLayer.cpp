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
#include "../Data/Words.h"
#include "../Data/DataManager.h"
#include "../Common/Utilities.h"
#include "../Events/GameEvents.h"
#include "../Events/EventManager.h"

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
    
    m_touchGridActionSprite = NULL;
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
//   CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onStart", MainLayer::onStart);
    
    return NULL;
}


bool MainLayer::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_topLayer", CCLayerColor*, m_topLayer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_bottomLayer", CCLayerColor*, m_bottomLayer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_letterGroove", CCLayerColor*, m_letterGroove);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_letterLayer", CCLayerColor*, m_letterLayer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_numberGroove", CCLayerColor*, m_numberGroove);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_numberLayer", CCLayerColor*, m_numberLayer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_gridGroove", CCLayerColor*, m_gridGroove);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_gridLayer", CCLayerColor*, m_gridLayer);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_tipsBg", CCLayer*, m_tipsBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_tip1Bg", CCLayer*, m_tip1Bg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_tip1", CCLabelTTF*, m_tip1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_tip2Bg", CCLayer*, m_tip2Bg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_tip2", CCLabelTTF*, m_tip2);


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
            showTips(true, touchGridEvent->getPhraseIndex(), touchGridEvent->getPhrase2Index());
            showTouchAction(touchGridEvent);
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
            
            m_touchGridActionSprite->setVisible(false);
            
            int size = m_wordsActionSpriteV.size();
            if (size > 0)
            {
                for (vector<CCSprite*>::iterator it = m_wordsActionSpriteV.begin(); it != m_wordsActionSpriteV.end(); it++) {
                    CCSprite *sprite = *it;
                    sprite->removeFromParentAndCleanup(true);
                }
                m_wordsActionSpriteV.clear();
            }
            
            break;
        }
            
        default:
            break;
    }
}

/*
void MainLayer::initGridButtons()
{
    float beginPosX = 0.0f;
    float beginPosY = m_gridLayer->getPosition().y;

    vector<Grid*> grids = DataManager::sharedDataManager()->getGrids();
    for (vector<Grid*>::iterator it = grids.begin(); it != grids.end(); ++it)
    {
        Grid *grid = *it;
        CGControlButton *button = CGControlButton::create();
        button->setEnabled(false);
        int index = grid->getIndex();
        button->setTag(index);
        button->addTarget(this, menu_selector(MainLayer::onGridClicked));
        
        int type = grid->getType();
        CCSpriteFrame* gridFrame = NULL;
        if (type == 0)
        {
            gridFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("block.png");
        }
        else
        {
            gridFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("blank.png");
        }
        button->setDisplayFrame(gridFrame);
        
        button->setScale(m_ccbScale);
        button->setAnchorPoint(ccp(0.0f, 1.0f));
        
        m_btnWidth = button->getContentSize().width;//不知道是否scale了没
        m_btnHeight = button->getContentSize().height;
        
        float btnPosX = beginPosX + (index % m_col) * m_btnWidth;
        float btnPosY = beginPosY - (index / m_col) * m_btnHeight;
        
        
        //m_gridLayer的AnchorPoint设为(0.0f, 1.0f) 但其坐标原点还是在左下角
        //而把button的AnchorPoint设为(0.0f, 1.0f) 然后setPosition设置的就是button锚点在m_gridLayer中的坐标
        button->setPosition(ccp(btnPosX, btnPosY));
        
        m_gridLayer->addChild(button);
        
        
        char num[3];
        sprintf(num, "%d", index);
        CCLabelTTF *label = CCLabelTTF::create(num, "Cochin", 36);
        label->setPosition(ccp(button->getContentSize().width * 0.5f, button->getContentSize().height * 0.5f));
        label->setColor(ccYELLOW);
        button->addChild(label);
        
        
        m_gridButtons.push_back(button);
        
    }

}
*/

void MainLayer::onGridClicked(CCObject* obj)
{
    CCLog("i am button %d", ((CGControlButton*)obj)->getTag());
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
            gridFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("block.png");
        }
        else
        {
            gridFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("blank.png");
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
        
        
        string word = "";
        int phraseIndex = grid->getPhraseIndex();//第几个成语
        int wordIndex = grid->getWordIndex();//第几个字
        int phrase2Index = grid->getPhrase2Index();
        int word2Index = grid->getWord2Index();
        if (phraseIndex != -1)
        {
            Words *words = wordsVec.at(phraseIndex);
            vector<string> vTemp = Utilities::splitString(words->getName(), "*");
            word = vTemp.at(wordIndex);
            vTemp.clear();
        }
        else if (phrase2Index != -1)
        {
            Words *words = wordsVec.at(phrase2Index);
            vector<string> vTemp = Utilities::splitString(words->getName(), "*");
            word = vTemp.at(word2Index);
            vTemp.clear();
        }
        CCLabelTTF *wordLabel = CCLabelTTF::create(word.c_str(), "Cochin", 36);
        wordLabel->setPosition(ccp(button->getContentSize().width * 0.5f, button->getContentSize().height * 0.5f));
        wordLabel->setColor(ccBLUE);
        button->addChild(wordLabel);
        
        
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
    float beginX = m_size.width * 0.125f;
    float beginY = m_size.height * 0.15f;
    
    CCRect rect = CCRect(beginX, beginY, m_size.width * 0.75f, m_size.height * 0.7f);
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
    CCPoint gridLayerLocInThis = ccp(gridLayerLoc.x + m_size.width * 0.175f, gridLayerLoc.y + m_size.height * 0.15f);
    
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

void MainLayer::initTouchGridActionSprite()
{
    CCSpriteFrame* frame = NULL;
    frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("wait01.png");
    m_touchGridActionSprite = CCSprite::createWithSpriteFrame(frame);

    CCAnimate* animate = Utilities::createAnimate("wait", 14, 0.1f);
    m_touchGridActionSprite->runAction(CCRepeatForever::create(animate));
    
    m_touchGridActionSprite->setVisible(false);
    m_touchGridActionSprite->setColor(ccRED);
    
}

void MainLayer::showTouchAction(Event *event)
{
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
    
    
    
}

void MainLayer::onStart(CCObject* pObject, CCControlEvent event)
{
        
    /*
    const char *route = "chat.chatHandler.send";
    json_t *msg = json_object();
    json_t *content = json_string("Good!");
    json_t *channelName = json_string("channelname");
    json_t *userName = json_string("username");
    json_t *target = json_string("*");
    json_object_set(msg, "content", content);
    json_object_set(msg, "rid", channelName);
    json_object_set(msg, "from", userName);
    json_object_set(msg, "target", target);
    // 使用的时候记得删除不用的变量
    json_decref(content);
    json_decref(channelName);
    json_decref(userName);
    json_decref(target);
    NetServer::sharedNetServer()->sendMsg(route, msg);
     */
}

