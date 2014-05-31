//
//  SingleSubRoom.cpp
//  CrossWordClient
//
//  Created by cy on 14-2-26.
//
//

#include "SingleSubRoom.h"
#include "../CommonUI/CGCCBReader.h"
#include "../CommonUI/CGControlButton.h"
#include "../Data/DataManager.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

SingleSubRoom::SingleSubRoom()
{
    m_scrollViewBg = NULL;
    m_ballLayer = NULL;
    m_curPage = 0;
    m_bgBeginY = 0.0f;
    m_bgEndY = 0.0f;
    
    int subLevel = DataManager::sharedDataManager()->getLevel();
    switch (subLevel)
    {
        case 0:
            m_pageCount = 5;//暂时设置为5
            break;
        case 1:
            m_pageCount = 8;
            break;
        case 2:
            m_pageCount = 9;
            break;
            
        default:
            break;
    }
    
}

SingleSubRoom::~ SingleSubRoom()
{
    CC_SAFE_RELEASE_NULL(m_scrollViewBg);
    CC_SAFE_RELEASE_NULL(m_ballLayer);
}

CCScene* SingleSubRoom::scene()
{
    CCScene* scene = CCScene::create();
    SingleSubRoom* layer = SingleSubRoom::create();
    scene->addChild(layer);
    
    return scene;
}

SingleSubRoom* SingleSubRoom::create()
{
    SingleSubRoom *layer = new SingleSubRoom();
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

bool SingleSubRoom::init()
{
    if (!CCLayer::init())
    {
        return false;
    }
    
    CCNodeLoaderLibrary *ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
    CGCCBReader reader(ccNodeLoaderLibrary);
    CCNode* node = reader.readCCBFile("single_sub_room.ccbi", this);
    addChild(node);
    
    m_ccbScale = reader.getCCBScale();
    
    this->setTouchEnabled(true);
    this->setKeypadEnabled(true);
    
    
    initScrollView();
    initBallLayer();
    setCurPageBall(0);
    
    
    
    return true;
}

void SingleSubRoom::keyBackClicked()
{
	CCLog("SingleSubRoom::keyBackClicked()~~~");
}

void SingleSubRoom::keyMenuClicked()
{
    CCLog("SingleSubRoom::keyMenuClicked()~~~");
}

void SingleSubRoom::onEnter()
{
    CCLayer::onEnter();
    
}

void SingleSubRoom::onExit()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCLayer::onExit();
}

SEL_MenuHandler SingleSubRoom::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName)
{
    return NULL;
}

SEL_CCControlHandler SingleSubRoom::onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onBack", SingleSubRoom::onBack);
    
    return NULL;
}


bool SingleSubRoom::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "scrollViewBg", CCLayerColor*, m_scrollViewBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "ballLayer", CCLayer*, m_ballLayer);
    return false;
}

void SingleSubRoom::registerWithTouchDispatcher(void)
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority, true);
}

bool SingleSubRoom::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    //滑动如果不在scrollviewbg区域 则忽略  还需细微完善
//    if (pTouch->getLocation().y < m_bgBeginY || pTouch->getLocation().y > m_bgEndY)
//    {
//        return true;
//    }
    
    m_touchPoint = pTouch->getLocation();
    m_offset = m_scrollView->getContentOffset();//即container的起始位置
    return true;
}

void SingleSubRoom::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
//    if (pTouch->getLocation().y < m_bgBeginY || pTouch->getLocation().y > m_bgEndY)
//    {
//        return;
//    }
    
    CCPoint movePoint = pTouch->getLocation();
    float dis = movePoint.x - m_touchPoint.x;
    
    //设定当前偏移的位置
    CCPoint adjustPoint = ccp(m_offset.x + dis, 0);
    m_scrollView->setContentOffset(adjustPoint);
}

void SingleSubRoom::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
//    if (pTouch->getLocation().y < m_bgBeginY || pTouch->getLocation().y > m_bgEndY)
//    {
//        return;
//    }
    
    CCPoint endPoint = pTouch->getLocation();
    float dis = endPoint.x - m_touchPoint.x;
    
    float triggerDis = m_scrollViewBg->getContentSize().width * 0.05f;
    float ignoreDis = m_scrollViewBg->getContentSize().width * 0.003f;
    
    if (fabs(dis) < ignoreDis)
    {
        // 小于三，不做拖动操作，也排除了（抖动误操作）
    }
    else if (fabs(dis) > triggerDis)
    {
        adjustScrollView(dis);
    }
    else
    {
        adjustScrollView(0);
    }
}

void SingleSubRoom::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    adjustScrollView(0);
}

void SingleSubRoom::initScrollView()
{
    if (m_scrollViewBg != NULL)
    {
        /*
         参数是viewSize
         CCScrollView是一个CCLayer 在初始化的时候ContentSize被设为WinSize
         而CCScrollView的setContentSize是设置container的大小 
         所以CCScrollView的大小始终是WinSize 但CCScrollView显示的是container那部分 滑动的响应区域也是在container里面才有效
         */
        m_scrollView = CCScrollView::create(m_scrollViewBg->getContentSize());
//        m_scrollView->setContentSize(m_scrollViewBg->getContentSize());
        m_scrollView->setDirection(kCCScrollViewDirectionHorizontal);
        m_scrollView->setTouchEnabled(false);
        
        m_scrollView->setAnchorPoint(CCPointZero);
        m_scrollView->setPosition(m_scrollViewBg->getPosition());
        
        m_scrollViewBg->getParent()->addChild(m_scrollView);//注意加上getParent才起到占位符的作用
        
        m_scrollView->setContainer(getContainerLayer());
        
        //m_scrollViewBg的y轴范围
        m_bgBeginY = m_scrollViewBg->getPositionY();
        m_bgEndY = m_bgBeginY + m_scrollViewBg->getContentSize().height;
    }
}

CCLayerColor* SingleSubRoom::getContainerLayer()
{
    CCLayerColor *container = CCLayerColor::create(ccc4(25, 25, 25, 125));
//    container->setAnchorPoint(CCPointZero);//setContainer方法里把AnchorPoint设为CCPointZero
    container->setPosition(CCPointZero);
    
    
    float bgWidth = m_scrollViewBg->getContentSize().width;
    float bgHeight = m_scrollViewBg->getContentSize().height;
    
    for (int i = 0; i < m_pageCount; i++)
    {
        CGControlButton *button = CGControlButton::create();
        button->setTag(i);//子关数 现在每页只有1关 后期应该扩展
        button->addTarget(this, menu_selector(SingleSubRoom::onBtnClicked));
        
        CCSpriteFrame* pokerFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("poker_back.png");
        
        button->setDisplayFrame(pokerFrame);
        button->setScale(m_ccbScale);
        button->setPosition(ccpAdd(ccp(bgWidth * 0.5f, bgHeight * 0.5f), ccp(bgWidth * i, 0)));
        container->addChild(button);
        
        char num[3];
        sprintf(num, "%d", i + 1);
        CCLabelTTF *label = CCLabelTTF::create(num, "Cochin", 36);
        label->setPosition(ccp(button->getContentSize().width * 0.5f, button->getContentSize().height * 0.5f));
        label->setColor(ccYELLOW);
        button->addChild(label);
        
    }
    container->setContentSize(ccp(bgWidth * m_pageCount, bgHeight));
    
    return container;
}

void SingleSubRoom::adjustScrollView(float offset)
{
    if (offset < 0)//表示右移
    {
        m_curPage++;
    }
    else if (offset > 0)
    {
        m_curPage--;
    }
    
    //页数检测
    if (m_curPage < 0)
    {
        m_curPage = 0;
    }
    else if (m_curPage > m_pageCount - 1)
    {
        m_curPage = m_pageCount - 1;
    }
    
    // 根据当前的 页数 获得偏移量，并设定新的位置，且启用动画效果
    CCPoint adjustPoint = ccp(-m_scrollViewBg->getContentSize().width * m_curPage , 0);
    m_scrollView->setContentOffsetInDuration(adjustPoint, 0.2f);
    
    //设置指示球
    setCurPageBall(m_curPage);
}

void SingleSubRoom::onBtnClicked(CCObject* obj)
{
    int subLevel = ((CGControlButton*)obj)->getTag();
    DataManager::sharedDataManager()->setSingleSubLevel(subLevel);
    
    Event *e = new Event(EventTypeGameStart);
    EventManager::sharedEventManager()->addEvent(e);
    
    CCLog("i am button %d", ((CGControlButton*)obj)->getTag());
}

void SingleSubRoom::initBallLayer()
{
    if (m_ballLayer != NULL)
    {
        float ballLayerHeight = m_ballLayer->getContentSize().height;
        float ballLayerWidth = m_ballLayer->getContentSize().width;
        float dotInterval = ballLayerWidth * 0.08f;//两点间间隙
        
        CCLog("dotInterval is %f", dotInterval);
        
        float beginPosX = 0.0f;
        float posY = ballLayerHeight * 0.5f;
        if (m_pageCount % 2 == 0)//偶数
        {
            int rightMid = m_pageCount / 2;//右中间点序号
            float tempX = ballLayerWidth * 0.5f + dotInterval * 0.5f;//中间点x坐标
            beginPosX = tempX - rightMid * dotInterval;
        }
        else
        {
            int mid = m_pageCount / 2;//中间点序号
            float tempX = ballLayerWidth * 0.5f;//中间点x坐标
            beginPosX = tempX - mid * dotInterval;
        }
        
        CCSpriteFrame* grayDotFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("volume_drag_bth.png");
        for (int i = 0; i < m_pageCount; i++)
        {
            CCSprite *dot = CCSprite::createWithSpriteFrame(grayDotFrame);
            dot->setPosition(ccp(beginPosX + i * dotInterval, posY));
            dot->setScale(m_ccbScale);
            m_ballLayer->addChild(dot);
            m_ballVector.push_back(dot);
        }
    }
    
}

void SingleSubRoom::setCurPageBall(int curPage)
{
    CCSpriteFrame* lightedDotFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("big_point.png");
    CCSpriteFrame* grayDotFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("volume_drag_bth.png");
    
    for (int i = 0; i < m_pageCount; i++)
    {
        CCSprite *dot = m_ballVector.at(i);
        if (dot != NULL)
        {
            if (i == curPage)
            {
                dot->setDisplayFrame(lightedDotFrame);
            }
            else
            {
                dot->setDisplayFrame(grayDotFrame);
            }
        }

    }
    
}

void SingleSubRoom::onBack(CCObject* pObject, CCControlEvent event)
{
    Event *e = new Event(EventTypeEnterSingleGame);
    EventManager::sharedEventManager()->addEvent(e);
}

