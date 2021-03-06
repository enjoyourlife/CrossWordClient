//
//  SingleGameController.cpp
//  SnakeClient
//
//  Created by cy on 13-12-11.
//
//

#include "SingleGameController.h"
#include "../Events/EventManager.h"
#include "../Events/GameEvents.h"
#include "../Scenes/SceneManager.h"
#include "../Data/DataManager.h"
#include "../Common/Utilities.h"
#include "../Data/Answer.h"

USING_NS_CC;
using namespace std;

SingleGameController::SingleGameController()
{
    EventManager::sharedEventManager()->addObserver(this);
}

SingleGameController::~SingleGameController()
{
    EventManager::sharedEventManager()->removeObserver(this);
}

void SingleGameController::startEvent(int event, void* data)
{
    switch (event)
    {
        default:
            break;
	}
}

void SingleGameController::startEvent(Event *event)
{
    int type = event->getType();
    switch (type)
    {
        case EventTypeEnterHall:
        {
            EventManager::sharedEventManager()->notifyEventSucceeded(event);
            //界面转换
            SceneManager::sharedSceneManager()->changeScene(SceneTypeHall);
            break;
        }
            
        case EventTypeEnterSingleGame:
        {
            EventManager::sharedEventManager()->notifyEventSucceeded(event);
            //界面转换
            SceneManager::sharedSceneManager()->changeScene(SceneTypeSingleRoom);
            break;
        }
        
        case EventTypeEnterSingleSubGame:
        {
            EventManager::sharedEventManager()->notifyEventSucceeded(event);
            //界面转换
            SceneManager::sharedSceneManager()->changeScene(SceneTypeSingleSubRoom);
            break;
        }
        
        case EventTypeGameStart:
        {
            parseJson();
            DataManager::sharedDataManager()->clearRightWordsIndexVec();
            
            //设置随机奖励的成语
            DataManager::sharedDataManager()->randomInitLocalWordBonus();
            
            //加载上一局的信息
            DataManager::sharedDataManager()->loadLastSelectAnswerVec();
            
            EventManager::sharedEventManager()->notifyEventSucceeded(event);
            break;
        }
            
        case EventTypeTouchGrid://点击单元格事件
        {
            handleTouchGridEvent(event);
            break;
        }
            
        case EventTypeChooseAnswer:
        {
            handleChooseAnswerEvent(event);
            break;
        }
            
        case EventTypeFixAnswer:
        {
            EventManager::sharedEventManager()->notifyEventSucceeded(event);
            break;
        }
            
        case EventTypeReward:
        {
            handleLocalUserRewardEvent(event);
            break;
        }
            
        case EventTypeSingleReset:
        {
            //清空玩家已经选择的答案
            DataManager::sharedDataManager()->resetSelectAnswerVec();
            //清空已经正确的词语
            DataManager::sharedDataManager()->clearRightWordsIndexVec();
            //不调用randomInitLocalWordBonus() 因为进入这个界面的EventTypeGameStart事件已经调用一次了
            
            
            //清空数据库 其实是否调用并不影响 因为退出时又重新存储了
            DataManager::sharedDataManager()->clearLastSelectAnswerVec();
            
            EventManager::sharedEventManager()->notifyEventSucceeded(event);
            break;
        }
            
        default:
            break;
            
    }
}

void SingleGameController::cancelEvent(Event *event)
{
	
}


void SingleGameController::onEventSucceeded(Event* event)
{
    if (DataManager::sharedDataManager()->getGameType() != GameTypeSingle)
    {
        return;
    }
    
    int type = event->getType();
    switch (type)
    {
        case EventTypeGameStart:
        {
            SceneManager::sharedSceneManager()->changeScene(SceneTypeMainLayer);
            break;
        }
            
        case EventTypeFixAnswer:
        {
            //此时调用本地玩家奖励方法
            FixAnswerEvent *fixAnswerEvent = (FixAnswerEvent*)event;
            int phraseIndex = fixAnswerEvent->getPhraseIndex();
            int phrase2Index = fixAnswerEvent->getPhrase2Index();
            
            localUserBonus(phraseIndex, phrase2Index);
            break;
        }
    }
    
}

void SingleGameController::onEventFailed(Event *event)
{
    if (DataManager::sharedDataManager()->getGameType() != GameTypeSingle)
    {
        return;
    }
    
    int type = event->getType();
    switch (type)
    {
        
    }
}



void SingleGameController::parseJson()
{
    int level = DataManager::sharedDataManager()->getLevel();
    int subLevel = DataManager::sharedDataManager()->getSingleSubLevel();
    char jsonFileName[30];
    switch (level)
    {
        case 0:
            sprintf(jsonFileName, "Text/map10_%04d.json", subLevel);
            break;
        case 1:
            sprintf(jsonFileName, "Text/map15_%04d.json", subLevel);
            break;
        case 2:
            sprintf(jsonFileName, "Text/map20_%04d.json", subLevel);
            break;
            
        default:
            sprintf(jsonFileName, "Text/map10_%04d.json", subLevel);
            break;
    }
    
    //开始解析
    json_t* gameDataJson = Utilities::getJsonFromFile(jsonFileName);
    DataManager::sharedDataManager()->parseJson(gameDataJson);
    
    /*
    json_t* wJson = json_object_get(gameDataJson, "w");
    json_t* hJson = json_object_get(gameDataJson, "h");
    int col = json_integer_value(wJson);
    int line = json_integer_value(hJson);
    
    DataManager::sharedDataManager()->setCol(col);
    DataManager::sharedDataManager()->setLine(line);
    
    //    json_decref(wJson);
    //    json_decref(hJson);
    
    
    //parse words
    vector<Words*> wordsVector;
    json_t* wordsJson = json_object_get(gameDataJson, "words");
    size_t wordsSize = json_array_size(wordsJson);
    for (int i = 0; i < wordsSize; i++)
    {
        json_t *wordJson = json_array_get(wordsJson, i);
        
        json_t* idJson = json_object_get(wordJson, "id");
        int wordId = json_integer_value(idJson);
        
        json_t *nameJson = json_object_get(wordJson, "name");
        const char *nameData = json_string_value(nameJson);
        
        json_t *tipsJson = json_object_get(wordJson, "tips");
        const char *tipsData = json_string_value(tipsJson);
        
        Words *words = new Words(wordId, Utilities::URLDecode(nameData), Utilities::URLDecode(tipsData));
        wordsVector.push_back(words);
        //        json_decref(idJson);  //have wrongs
        //        json_decref(nameJson);
        //        json_decref(tipsJson);
        //        json_decref(wordJson);
    }
    DataManager::sharedDataManager()->initWords(wordsVector);
    
    
    //预先初始化玩家选择的答案
    vector<Answer*> selectAnswerVec;
    
    //parse map_v and map_h
    vector<Grid*> gridVector;
    int index = 0;
    int base = 16 * 16;
    json_t* mapVJson = json_object_get(gameDataJson, "map_v");
    size_t mapVSize = json_array_size(mapVJson);
    
    json_t* mapHJson = json_object_get(gameDataJson, "map_h");
    
    for (int i = 0; i < mapVSize; i++)
    {
        json_t *vJson = json_array_get(mapVJson, i);
        size_t vSize = json_array_size(vJson);
        
        json_t *hJson = json_array_get(mapHJson, i);
        
        for (int j = 0; j < vSize; j++)
        {
            json_t *gridVJson = json_array_get(vJson, j);
            int vType = json_integer_value(gridVJson);
            
            int phraseIndex = -1;//第几个成语
            int wordIndex = -1;//第几个字
            if (vType != -1)
            {
                phraseIndex = vType / base;
                wordIndex = vType % base;
            }
            
            
            json_t *gridHJson = json_array_get(hJson, j);
            int hType = json_integer_value(gridHJson);
            int phrase2Index = -1;
            int word2Index = -1;
            if (hType != -1)
            {
                phrase2Index = hType / base;
                word2Index = hType % base;
            }
            
            Grid *grid = new Grid(index, vType, hType, phraseIndex, wordIndex, phrase2Index, word2Index);
            gridVector.push_back(grid);
            
            if (phraseIndex != -1 || phrase2Index != -1)
            {
                Answer *answer = new Answer(index, phraseIndex, wordIndex, phrase2Index, word2Index);
                selectAnswerVec.push_back(answer);
            }
                        
            index++;
            
        }
    }
    
    
    vector<Answer*>::iterator answerIt;
    for (int i = 0; i < wordsVector.size(); i++)
    {
        Words *w = wordsVector.at(i);
        vector<string> vTemp = Utilities::splitString(w->getName(), "*");
        
        for (answerIt = selectAnswerVec.begin(); answerIt != selectAnswerVec.end(); answerIt++)
        {
            Answer *a = *answerIt;
            if (a->getPhraseIndex() == i)
            {
                a->setOriAnswerWord(vTemp.at(a->getWordIndex()));
                continue;
            }
            
            if (a->getPhrase2Index() == i)
            {
                a->setOriAnswerWord(vTemp.at(a->getWord2Index()));
            }
        }
    }
    
    DataManager::sharedDataManager()->initSelectAnswerVec(selectAnswerVec);
    
    
    DataManager::sharedDataManager()->initGrids(gridVector);
    */
}

void SingleGameController::localUserBonus(int phraseIndex, int phrase2Index)
{
    //不管如何先给对应的奖励 
    localUserBonusByPhraseIndex(phraseIndex);
    localUserBonusByPhraseIndex(phrase2Index);
    
    //然后胜利还给胜利奖励 同时在mainlayer弹出结算框
    bool isWin = checkGameWin();
    int bonusType = 0;
    if (isWin)//单机游戏结束
    {
        bonusType = 4;
        
        RewardEvent *rewardEvent = new RewardEvent(bonusType);
        rewardEvent->setIsWin(isWin);
        
        vector<int> localPassBonus = DataManager::sharedDataManager()->getLocalPassBonus();
        if (localPassBonus.size() == 2)
        {
            rewardEvent->setBonusSilver(localPassBonus.at(0));
            rewardEvent->setBonusExp(localPassBonus.at(1));
        }
        
        //同时还需要解锁关卡 后期解锁的时候 在界面上给一个提示
        DataManager::sharedDataManager()->updateLocalUnLockLevel();
        
        //单机游戏结束时 需要保存玩家所选的答案
        DataManager::sharedDataManager()->saveSelectAnswerVec();
        
        EventManager::sharedEventManager()->addEvent(rewardEvent);
    }
    
    
}

void SingleGameController::localUserBonusByPhraseIndex(int phraseIndex)
{
    if (phraseIndex != -1)
    {
        RewardEvent *rewardEvent = NULL;
        int bonusType = 0;
        
        Words *words = DataManager::sharedDataManager()->getWords().at(phraseIndex);
        int wordsBonusType = words->getBonusType();
        if (wordsBonusType > 0)
        {
            rewardEvent = new RewardEvent(wordsBonusType);
            if (wordsBonusType == 1)
            {
                rewardEvent->setBonusSilver(words->getBonusValue());
            }
            else if (wordsBonusType == 2)
            {
                rewardEvent->setBonusExp(words->getBonusValue());
            }
        }
        else
        {
            bonusType = 3;
            rewardEvent = new RewardEvent(bonusType);
            
            vector<int> localEveryBonus = DataManager::sharedDataManager()->getLocalEveryBonus();
            if (localEveryBonus.size() == 2)
            {
                rewardEvent->setBonusSilver(localEveryBonus.at(0));
                rewardEvent->setBonusExp(localEveryBonus.at(1));
            }
        }
        
        EventManager::sharedEventManager()->addEvent(rewardEvent);
    }
}

void SingleGameController::handleLocalUserRewardEvent(Event *event)
{
    RewardEvent *re = (RewardEvent*)event;
    int bonusType = re->getBonusType();
    switch (bonusType)
    {
        case 1:
        {
            //钱满给提示
            DataManager::sharedDataManager()->updateLocalUserSilver(re->getBonusSilver());
            break;
        }
        case 2:
        {
            DataManager::sharedDataManager()->updateLocalUser(re->getBonusExp());
            break;
        }
        case 3:
        case 4:
        {
            DataManager::sharedDataManager()->updateLocalUser(re->getBonusExp());
            DataManager::sharedDataManager()->updateLocalUserSilver(re->getBonusSilver());
            break;
        }

        default:
            break;
    }
    
    EventManager::sharedEventManager()->notifyEventSucceeded(event);
}
