#include "HelloWorldScene.h"
#include "Network/NetServer.h"
#include "Scenes/SceneManager.h"
#include "CommonUI/CGToast.h"
#include "Common/Utilities.h"
#include "CommonUI/CGWaiting.h"
#include "Scenes/Login.h"
#include "Platform/System.h"
#include "Network/NetServerEx.h"
#include "CommonUI/CGDialog.h"

USING_NS_CC;


CCScene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
                                        "CloseNormal.png",
                                        "CloseSelected.png",
                                        this,
                                        menu_selector(HelloWorld::menuCloseCallback));
    
	pCloseItem->setPosition(ccp(origin.x + visibleSize.width - pCloseItem->getContentSize().width/2 ,
                                origin.y + pCloseItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
    pMenu->setPosition(CCPointZero);
    this->addChild(pMenu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    CCLabelTTF* pLabel = CCLabelTTF::create("Hello World", "Arial", 24);
    
    // position the label on the center of the screen
    pLabel->setPosition(ccp(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - pLabel->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(pLabel, 1);

    // add "HelloWorld" splash screen"
    CCSprite* pSprite = CCSprite::create("HelloWorld.png");

    // position the sprite on the center of the screen
    pSprite->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    // add the sprite as a child to this layer
    this->addChild(pSprite, 0);
    
    
    
    CCSize winsize = CCDirector::sharedDirector()->getWinSize();
	float scale = winsize.width/winsize.height;
    
    CCLog("width is %f and height is %f and scale is %f", winsize.width, winsize.height, scale);
    char text[100];
    sprintf(text, "width is %f and height is %f and scale is %f", winsize.width, winsize.height, scale);
    pLabel->setString(text);
    
    
    
    
    return true;
}



void HelloWorld::menuCloseCallback(CCObject* pSender)
{
    /*
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
	CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
#else
    CCDirector::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
#endif
     */
    
    /*
    const char *route = "chat.chatHandler.send";
    json_t *msg = json_object();
    json_t *content = json_string("Good!");
    json_t *channelName = json_string("pwd");
    json_t *userName = json_string("name");
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
    
    /*
    MainLayer *mainLayer = MainLayer::create();
    this->addChild(mainLayer);
     */
    
//    SceneManager::sharedSceneManager()->changeScene(SceneTypeSingleSubRoom);
    
    /*
    CGToast *toast = CGToast::create();
    toast->addTarget(this, menu_selector(HelloWorld::onToast));
    toast->playAction();
    this->addChild(toast);
    */
    
//    CGWaiting *waiting = CGWaiting::create();
//    this->addChild(waiting);

//    Login* login = Login::create();
//    this->addChild(login);
    
//    CCLog("%d %d %d", System::isNetAvailable(), System::is3GEnabled(), System::isWIFIEnabled());
    
//    NetServerEx::sharedNetServerEx()->login("user1", "pwd");

    
//    SceneManager::sharedSceneManager()->changeScene(SceneTypeMainLayer);
    
    CGDialog::show(GameOKCancelButtonType, "waiting_txt", this, menu_selector(HelloWorld::onToast), menu_selector(HelloWorld::onToast));
}

void HelloWorld::onToast(CCObject* obj)
{
    CCLog("just toast!");
}
