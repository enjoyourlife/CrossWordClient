//
//  OnlineGameController.cpp
//  SnakeClient
//
//  Created by cy on 13-12-11.
//
//

#include "OnlineGameController.h"
#include "../Events/EventManager.h"
#include "../Events/GameEvents.h"
#include "../Data/DataManager.h"
#include "../Data/SaveDataManager.h"
#include "../Platform/System.h"
#include "../Scenes/SceneManager.h"
#include "../CommonUI/CGToast.h"
#include "../Common/Localize.h"
#include "../Scenes/Login.h"
#include "../Network/NetServerEx.h"

USING_NS_CC;
using namespace std;

OnlineGameController::OnlineGameController()
{
    EventManager::sharedEventManager()->addObserver(this);
}

OnlineGameController::~OnlineGameController()
{
    EventManager::sharedEventManager()->removeObserver(this);
}

void OnlineGameController::startEvent(int event, void* data)
{
    switch (event)
    {
        case EventTypeLogin:
            break;
            
        default:
            break;
	}
}

void OnlineGameController::startEvent(Event *event)
{
    int type = event->getType();
    switch (type)
    {
        case EventTypeLogin:
        {
            LoginEvent *loginEvent = (LoginEvent*)event;
            NetServerEx::sharedNetServerEx()->login(loginEvent->getUsername(), loginEvent->getPassword());
            
            //显示等待框 然后在成功或者失败的时候消去 失败消去的时候需要给提示
            //目前此框出现和消失太快 后期改进或者直接去掉
            m_waiting = CGWaiting::create();
            CCDirector::sharedDirector()->getRunningScene()->addChild(m_waiting);
            break;
        }
            
        case EventTypeLoginEx:
        {
            int tag = event->getTag();
            if (tag == 0)
            {
                EventManager::sharedEventManager()->notifyEventSucceeded(event);//仅仅是过渡
            }
            else if (tag == 1)
            {
                EventManager::sharedEventManager()->notifyEventFailed(event);
            }
            break;
        }
            
        case EventTypeEnterHall:
        {
            EventManager::sharedEventManager()->notifyEventSucceeded(event);
            //界面转换
            SceneManager::sharedSceneManager()->changeScene(SceneTypeHall);
            break;
        }
            
        case EventTypeEnterCompetitiveGame:
        {
            enterCompOrCoopGame(event);
            break;
        }
            
        case EventTypeEnterCooperationGame:
        {
            enterCompOrCoopGame(event);
            break;
        }
            
            
        default:
            break;
            
    }
}

void OnlineGameController::cancelEvent(Event *event)
{
	
}



void OnlineGameController::onEventSucceeded(Event* event)
{
    int type = event->getType();
    switch (type)
    {
        case EventTypeLogin:
        {
            LoginEvent *loginEvent = (LoginEvent*)event;
            
            LoginEventEx *lee = new LoginEventEx(loginEvent->getLoginType(), loginEvent->getUsername(), loginEvent->getPassword());
            lee->setTag(0);//成功
            EventManager::sharedEventManager()->addEvent(lee);
            break;
        }
            
        case EventTypeLoginEx:
        {
            LoginEventEx *lee = (LoginEventEx*)event;
            //记录登录成功后的用户名 密码  同时设置DataManager m_isLogin
            SaveDataManager::sharedSaveDataManager()->setUsername(lee->getUsername());
            SaveDataManager::sharedSaveDataManager()->setPassword(lee->getPassword());
            
            DataManager::sharedDataManager()->setIsLogin(true);
            
            
            //根据loginType改变界面
            int loginType = lee->getLoginType();
            switch (loginType)
            {
                case 0://大厅 只变左下角文字
                    
                    break;
                case 1://CompetitiveRoom
                    SceneManager::sharedSceneManager()->changeScene(SceneTypeCompetitiveRoom);
                    break;
                case 2://合作场
                    SceneManager::sharedSceneManager()->changeScene(SceneTypeCompetitiveRoom);//应该是合作场
                    break;
                    
                default:
                    break;
            }
            
            break;
        }
            
    }
    
}

void OnlineGameController::onEventFailed(Event *event)
{
    int type = event->getType();
    switch (type)
    {
        case EventTypeLogin:
        {
            LoginEvent *loginEvent = (LoginEvent*)event;
            LoginEventEx *lee = new LoginEventEx(loginEvent->getLoginType(), loginEvent->getUsername(), loginEvent->getPassword());
            lee->setTag(1);//失败
            EventManager::sharedEventManager()->addEvent(lee);
            break;
        }
            
        case EventTypeLoginEx:
        {
            //消去m_waiting
            if (m_waiting != NULL && m_waiting->getParent() != NULL)
            {
                m_waiting->removeFromParentAndCleanup(true);
            }
            CGToast *toast = CGToast::create();
            toast->setText(Localize::sharedLocalize()->getString("toast_txt5"));
            toast->playAction();
            CCDirector::sharedDirector()->getRunningScene()->addChild(toast);
            break;
        }
    }
}


void OnlineGameController::enterCompOrCoopGame(Event* event)
{
    int eventType = event->getType();
    int loginType = 1;
    if (eventType == EventTypeEnterCompetitiveGame)
    {
        loginType = 1;
    }
    else if (eventType == EventTypeEnterCooperationGame)
    {
        loginType = 2;
    }
    
    //检测是否处于登录状态 是则直接进入
    if (DataManager::sharedDataManager()->getIsLogin() && System::isNetAvailable())
    {
        EventManager::sharedEventManager()->notifyEventSucceeded(event);
        //界面转换
        if (loginType == 1)
        {
            SceneManager::sharedSceneManager()->changeScene(SceneTypeCompetitiveRoom);
        }
        else if (loginType == 2)
        {
            SceneManager::sharedSceneManager()->changeScene(SceneTypeCompetitiveRoom);//应该是合作场
        }

        CCLog("1~~~~~~~~~");
    }
    else
    {
        //如果没有网络 弹出提示
        if(!System::isNetAvailable())
        {
            CGToast *toast = CGToast::create();
            toast->setText(Localize::sharedLocalize()->getString("toast_txt0"));
            toast->playAction();
            CCDirector::sharedDirector()->getRunningScene()->addChild(toast);
            EventManager::sharedEventManager()->notifyEventFailed(event);
        }
        else
        {
            string username = SaveDataManager::sharedSaveDataManager()->getUsername();
            //如果SaveDataManager没有存有一个用户名 则弹出一个登录框
            if (username.compare("") == 0)
            {
                Login *login = Login::create();
                login->setLoginType(loginType);
//                SceneManager::sharedSceneManager()->getCurrentScene()->addChild(login);
                CCDirector::sharedDirector()->getRunningScene()->addChild(login);
                
                //提示
                CGToast *toast = CGToast::create();
                if (loginType == 1)
                {
                    toast->setText(Localize::sharedLocalize()->getString("toast_txt2"));
                }
                else if (loginType == 2)
                {
                    toast->setText(Localize::sharedLocalize()->getString("toast_txt6"));
                }
                toast->playAction();
//                SceneManager::sharedSceneManager()->getCurrentScene()->addChild(toast);
                CCDirector::sharedDirector()->getRunningScene()->addChild(toast);
                
                EventManager::sharedEventManager()->notifyEventFailed(event);
                
                CCLog("2~~~~~~~~~");

            }
            else//否则执行自动登录过程
            {
                EventManager::sharedEventManager()->notifyEventFailed(event);
                
                const string username = SaveDataManager::sharedSaveDataManager()->getUsername();
                const string password = SaveDataManager::sharedSaveDataManager()->getPassword();
                
                LoginEvent *loginEvent = new LoginEvent(loginType, username, password);
                EventManager::sharedEventManager()->addEvent(loginEvent);
                
                CCLog("3~~~~~~~~~");
            }
            
        }
        
        
    }
    
    
}
