//
//  LoadingLayer.cpp
//  SnakeClient
//
//  Created by cy on 13-12-21.
//
//

#include "LoadingLayer.h"
#include "SceneManager.h"

USING_NS_CC;
using namespace std;

LoadingLayer::LoadingLayer()
{
    m_loadingPanel = NULL;
    m_loadingProgress = NULL;
}

LoadingLayer::~ LoadingLayer()
{
    
}

CCScene* LoadingLayer::scene()
{
    CCScene* scene = CCScene::create();
    LoadingLayer* layer = LoadingLayer::create();
    scene->addChild(layer);
    return scene;
}

bool LoadingLayer::init()
{
    if (!CCLayerColor::initWithColor(ccc4(0, 0, 0, 255)))
    {
        return false;
    }
    
    /*
    CCDirector *pDirector = CCDirector::sharedDirector();
	TargetPlatform target = CCApplication::sharedApplication()->getTargetPlatform();//获取当前设备类型
    if (target == kTargetIpad) {
        pDirector->enableRetinaDisplay(false);
    } else {
        pDirector->enableRetinaDisplay(true);
    }
    */
    
     m_loadingPanel = CCSprite::create("Images/startpanel.png");
     
     m_loadingProgress = CCProgressTimer::create(CCSprite::create("Images/startprogress.png"));
     m_loadingProgress->setType(kCCProgressTimerTypeBar);
     //Setup for a bar starting from the bottom since the midpoint is 0 for the y
     m_loadingProgress->setMidpoint(ccp(0.0f, 0.0f));
     //Setup for a vertical bar since the bar change rate is 0 for x meaning no horizontal change
     m_loadingProgress->setBarChangeRate(ccp(1.0f, 0.0f));
     m_loadingProgress->setPercentage(0.0f);
     
     CCSize size = m_loadingPanel->getContentSize();
     float loadingPanelWidth = size.width;
     float loadingPanelHeight = size.height;
     
     m_loadingProgress->setPosition(ccp(loadingPanelWidth * 0.5f, loadingPanelHeight * 0.53f));
     m_loadingPanel->addChild(m_loadingProgress);
     
     float width = CCDirector::sharedDirector()->getWinSize().width;
     float height = CCDirector::sharedDirector()->getWinSize().height;
     m_loadingPanel->setPosition(ccp(width * 0.5f, height * 0.45f));
     this->addChild(m_loadingPanel);

    this->setTouchEnabled(true);
    return true;
}

void LoadingLayer::onEnter()
{
    CCLayer::onEnter();
    CCDirector::sharedDirector()->getScheduler()->scheduleUpdateForTarget(this, 0, false);
}

void LoadingLayer::onExit()
{
    CCDirector::sharedDirector()->getScheduler()->unscheduleUpdateForTarget(this);
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCLayer::onExit();
}

void LoadingLayer::update(float time)
{
    static int count = 0;
    
    if (count == 0)
    {
        CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("Images/game_play.plist");
    }
    else if (count == 1)
    {
        //		CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("dragonrun1.plist");
    }
    /*
     else if (count == 2)
     {
     MusicManager::getInstance()->preloadBackgroundMusic(GAMEBEGINMUSIC);
     }
     else if (count == 3)
     {
     MusicManager::getInstance()->preloadBackgroundMusic(GAMEONMUSIC);
     }
     else if (count == 4)
     {
     MusicManager::getInstance()->preloadEffect(BC1SOUND);
     }
     else if (count == 5)
     {
     MusicManager::getInstance()->preloadEffect(BC2SOUND);
     }
     else if (count == 6)
     {
     MusicManager::getInstance()->preloadEffect(BC3SOUND);
     }
     else if (count == 7)
     {
     MusicManager::getInstance()->preloadEffect(C1SOUND);
     }
     else if (count == 8)
     {
     MusicManager::getInstance()->preloadEffect(C2SOUND);
     }
     else if (count == 9)
     {
     MusicManager::getInstance()->preloadEffect(C3SOUND);
     }
     else if (count == 10)
     {
     MusicManager::getInstance()->preloadEffect(C4SOUND);
     }
     else if (count == 11)
     {
     MusicManager::getInstance()->preloadEffect(C5SOUND);
     }
     else if (count == 12)
     {
     MusicManager::getInstance()->preloadEffect(CANNONHURTSOUND);
     }
     else if (count == 13)
     {
     MusicManager::getInstance()->preloadEffect(CB1SOUND);
     }
     else if (count == 14)
     {
     MusicManager::getInstance()->preloadEffect(CB2SOUND);
     }
     else if (count == 15)
     {
     MusicManager::getInstance()->preloadEffect(CB3SOUND);
     }
     else if (count == 16)
     {
     MusicManager::getInstance()->preloadEffect(CB4SOUND);
     }
     else if (count == 17)
     {
     MusicManager::getInstance()->preloadEffect(CB5SOUND);
     }
     else if (count == 18)
     {
     MusicManager::getInstance()->preloadEffect(CLICKSOUND);
     }
     else if (count == 19)
     {
     MusicManager::getInstance()->preloadEffect(COINSOUND);
     }
     else if (count == 20)
     {
     MusicManager::getInstance()->preloadEffect(COINZEROSOUND);
     }
     else if (count == 21)
     {
     MusicManager::getInstance()->preloadEffect(CS1SOUND);
     }
     else if (count == 22)
     {
     MusicManager::getInstance()->preloadEffect(CS2SOUND);
     }
     else if (count == 23)
     {
     MusicManager::getInstance()->preloadEffect(CS3SOUND);
     }
     else if (count == 24)
     {
     MusicManager::getInstance()->preloadEffect(CS4SOUND);
     }
     else if (count == 25)
     {
     MusicManager::getInstance()->preloadEffect(CS5SOUND);
     }
     else if (count == 26)
     {
     MusicManager::getInstance()->preloadEffect(DEADSOUND);
     }
     else if (count == 27)
     {
     MusicManager::getInstance()->preloadEffect(EGGSOUND);
     }
     else if (count == 28)
     {
     MusicManager::getInstance()->preloadEffect(GAMEBEGINSOUND);
     }
     else if (count == 29)
     {
     MusicManager::getInstance()->preloadEffect(GAMEONSOUND);
     }
     else if (count == 30)
     {
     MusicManager::getInstance()->preloadEffect(HURTSOUND);
     }
     else if (count == 31)
     {
     MusicManager::getInstance()->preloadEffect(LOSESOUND);
     }
     else if (count == 32)
     {
     MusicManager::getInstance()->preloadEffect(MOVESOUND);
     }
     else if (count == 33)
     {
     MusicManager::getInstance()->preloadEffect(TIMESOUND);
     }
     else if (count == 34)
     {
     MusicManager::getInstance()->preloadEffect(UNLOCKSOUND);
     }
     else if (count == 35)
     {
     MusicManager::getInstance()->preloadEffect(WINSOUND);
     }
     else if (count == 36)
     {
     MusicManager::getInstance()->preloadEffect(ZEROSOUND);
     }*/
    else if (count == 50)
	{
        SceneManager::sharedSceneManager()->changeScene(SceneTypeHall);
	}
    
    count++;
    
    
     if (m_loadingPanel != NULL)
     {
         float percent = (float)count / 50.0f * 100.0f;
         m_loadingProgress->setPercentage(percent);
     }
    return;
}

void LoadingLayer::registerWithTouchDispatcher(void)
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority, true);
}

bool LoadingLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    
    return true;
}

void LoadingLayer::ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    
}

void LoadingLayer::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    
}

void LoadingLayer::ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    
}