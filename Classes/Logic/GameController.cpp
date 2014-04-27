//
//  GameController.cpp
//  SnakeClient
//
//  Created by cy on 13-12-11.
//
//

#include "GameController.h"
#include "../Common/Utilities.h"
#include "../Data/Words.h"
#include "../Data/DataManager.h"
#include "../Events/GameEvents.h"


USING_NS_CC;
using namespace std;

GameController::GameController()
{
    
}

GameController::~GameController()
{
}


void GameController::initAnswers(Grid *grid)
{
    int phraseIndex = grid->getPhraseIndex();
    int phrase2Index = grid->getPhrase2Index();
    
    //设置候选答案
    srand(time(0));
    vector<string> answers;
    
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
        vector<string> vTemp = Utilities::splitString(words->getName(), "*");//分割一次就可以了
        word = vTemp.at(grid->getWord2Index());
        vTemp.clear();
        
        flag = true;
    }
    
    if (flag)
    {
        vector<string> answerDicTemp = DataManager::sharedDataManager()->getAnswerDic();
        int dicSize = answerDicTemp.size();
        //随机选7个放入answers 然后再加入word 最后随机排列
        for (int i = 0; i < ANSWER_NUM - 1; i++)
        {
            answerIndex = rand() % dicSize;
            answers.push_back(answerDicTemp.at(answerIndex));
        }
        answers.push_back(word);
        
        Utilities::random_permute(answers);
        
        DataManager::sharedDataManager()->initAnswers(answers);
    }
}

void GameController::handleTouchGridEvent(Event* event)
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
}