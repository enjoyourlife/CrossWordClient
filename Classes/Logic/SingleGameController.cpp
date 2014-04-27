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
            EventManager::sharedEventManager()->notifyEventSucceeded(event);
            break;
        }
            
        case EventTypeTouchGrid://点击单元格事件
        {
            handleTouchGridEvent(event);
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
    char jsonFileName[20];
    switch (level)
    {
        case 0:
            sprintf(jsonFileName, "Text/e%d.json", subLevel);
            break;
        case 1:
            sprintf(jsonFileName, "Text/n%d.json", subLevel);
            break;
        case 2:
            sprintf(jsonFileName, "Text/h%d.json", subLevel);
            break;
            
        default:
            sprintf(jsonFileName, "Text/e%d.json", subLevel);
            break;
    }
    
    //开始解析
    json_t* gameDataJson = Utilities::getJsonFromFile(jsonFileName);
    
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
            index++;
            
            gridVector.push_back(grid);
        }
    }
    
    DataManager::sharedDataManager()->initGrids(gridVector);
    
}

