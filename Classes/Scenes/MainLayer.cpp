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
    
    m_size = this->getContentSize();
    m_ignoreDis = CCDirector::sharedDirector()->getWinSize().width * 0.005f;
    
    m_line = 15;
    m_col = 15;
    
    DataManager::sharedDataManager()->initGrids(m_line, m_col);
    
    
    initGridButtons();
    initMoveDirection();
    initXYRange();
    
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
    
}

void MainLayer::onExit()
{
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
    
    vector<Grid*> grids = DataManager::sharedDataManager()->getGrids();
    for (vector<Grid*>::iterator it = grids.begin(); it != grids.end(); ++it)
    {
        Grid *grid = *it;
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
        CCLabelTTF *label = CCLabelTTF::create(num, "Cochin", 36);
        label->setPosition(ccp(button->getContentSize().width * 0.5f, button->getContentSize().height * 0.5f));
        label->setColor(ccYELLOW);
        button->addChild(label);
        
        
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
    
    return index;
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

