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
#include "../Common/Utilities.h"
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
            //显示等待框 然后在成功或者失败的时候消去 失败消去的时候需要给提示
            //目前此框出现和消失太快 后期改进或者直接去掉  没有网络的时候有问题
            m_waiting = CGWaiting::create();
            CCDirector::sharedDirector()->getRunningScene()->addChild(m_waiting);
            
            
            LoginEvent *loginEvent = (LoginEvent*)event;
            NetServerEx::sharedNetServerEx()->login(loginEvent->getUsername(), loginEvent->getPassword());
        
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
            
        case EventTypeSitDown:
        {
            SceneManager::sharedSceneManager()->changeScene(SceneTypeWaitingRoom);
            break;
        }
            
        case EventTypeSitUp:
        {
            EventManager::sharedEventManager()->notifyEventSucceeded(event);
            break;
        }
            
        case EventTypeGameStart:
        {
            EventManager::sharedEventManager()->notifyEventSucceeded(event);
            break;
        }
            
        case EventTypeGameStartEx:
        {
            EventManager::sharedEventManager()->notifyEventSucceeded(event);
            break;
        }
            
        case EventTypeTouchGrid://点击单元格事件
        {
            TouchGridEvent *touchGridEvent = (TouchGridEvent*)event;
            int index = touchGridEvent->getIndex();
            Grid *clickGrid = DataManager::sharedDataManager()->getClickGrid(index);
            
            if (clickGrid != NULL)
            {
                int phraseIndex = clickGrid->getPhraseIndex();
                int phrase2Index = clickGrid->getPhrase2Index();
                if (phraseIndex == -1 && phrase2Index == -1)
                {
                    EventManager::sharedEventManager()->notifyEventFailed(event);
                }
                else
                {
                    if (phraseIndex != -1)
                    {
                        touchGridEvent->setPhraseIndex(phraseIndex);
                        touchGridEvent->setWordIndex(clickGrid->getWordIndex());
                    }
                    
                    if (phrase2Index != -1)
                    {
                        touchGridEvent->setPhrase2Index(phrase2Index);
                        touchGridEvent->setWord2Index(clickGrid->getWord2Index());
                    }
                    
                    //遍历设置vector<int>
                    vector<int> wordsIndexVector;
                    wordsIndexVector.clear();
                    //直接写it = DataManager::sharedDataManager()->getGrids().begin();为乱数据 why?
                    vector<Grid*> gridsTemp = DataManager::sharedDataManager()->getGrids();
                    for (vector<Grid*>::iterator it = gridsTemp.begin(); it != gridsTemp.end(); ++it)
                    {
                        Grid *grid = *it;
                        int gridPhraseIndex = grid->getPhraseIndex();
                        int gridPhrase2Index = grid->getPhrase2Index();
                        if (phraseIndex != -1 && gridPhraseIndex == phraseIndex)
                        {
                            wordsIndexVector.push_back(grid->getIndex());
                            continue;//去掉交叉点
                        }
                        
                        if (phrase2Index != -1 && gridPhrase2Index == phrase2Index)
                        {
                            wordsIndexVector.push_back(grid->getIndex());
                        }

                    }
                    touchGridEvent->setWordsIndexVector(wordsIndexVector);
                    
                    
                    //设置候选答案
                    initAnswers(clickGrid);
                                        
                    EventManager::sharedEventManager()->notifyEventSucceeded(event);
                }
            }
            else
            {
                //点到block或者异常  显示block grid的动画
                EventManager::sharedEventManager()->notifyEventFailed(event);
            }
            
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
            
        case EventTypeSitDown:
        {
            SitDownEvent *sde = (SitDownEvent*)event;
            int type = sde->getType();
            int level = sde->getLevel();
            
            NetServerEx::sharedNetServerEx()->sitDownOrUp(0, type, level);
            break;
        }
            
        case EventTypeSitUp:
        {
            SitUpEvent *sue = (SitUpEvent*)event;
            int type = sue->getType();
            int level = sue->getLevel();
            
            NetServerEx::sharedNetServerEx()->sitDownOrUp(1, type, level);
            break;
        }
            
        case EventTypeGameStart:
        {
            Event *e = new Event(EventTypeGameStartEx);
            EventManager::sharedEventManager()->addEvent(e);
            CCLog("have fun boy!~~~~~~~~~~");
            break;
        }
            
        case EventTypeGameStartEx:
        {
            SceneManager::sharedSceneManager()->changeScene(SceneTypeMainLayer);
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


void OnlineGameController::initAnswers(Grid *grid)
{
    int phraseIndex = grid->getPhraseIndex();
    int phrase2Index = grid->getPhrase2Index();
    
    //设置候选答案
    srand(time(0));
    vector<string> answers;
    
    string fileName = CCFileUtils::sharedFileUtils()->fullPathForFilename("Text/answers.json");
    CCString *answersCstr = CCString::createWithContentsOfFile(fileName.c_str());
    
    vector<Words*> wordsVec = DataManager::sharedDataManager()->getWords();
    string word = "";
    int answerIndex = 0;
    bool flag = false;
    //交叉字以横为准
    if ((phraseIndex != -1 && phrase2Index == -1) || (phraseIndex != -1 && phrase2Index != -1))
    {
        Words *words = wordsVec.at(phraseIndex);
        vector<string> vTemp = Utilities::splitString(words->getName(), "*");
        word = vTemp.at(grid->getWordIndex());
        vTemp.clear();
        
        flag = true;
    }
    else if (phraseIndex == -1 && phrase2Index != -1)
    {
        Words *words = wordsVec.at(phrase2Index);
        vector<string> vTemp = Utilities::splitString(words->getName(), "*");
        word = vTemp.at(grid->getWord2Index());
        vTemp.clear();
        
        flag = true;
    }
    
    if (flag)
    {
        vector<string> answersVTemp = Utilities::splitString(answersCstr->m_sString, "*");
        //随机选7个放入answers 然后再加入word 最后随机排列
        for (int i = 0; i < ANSWER_NUM - 1; i++)
        {
            answerIndex = rand() % answersVTemp.size();
            answers.push_back(answersVTemp.at(answerIndex));
        }
        answersVTemp.clear();
        
        answers.push_back(word);
        
        Utilities::random_permute(answers);
        
        DataManager::sharedDataManager()->initAnswers(answers);
    }
}
