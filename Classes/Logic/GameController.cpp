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
#include "../Data/Answer.h"
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
    Grid *clickGrid = DataManager::sharedDataManager()->getTouchGrid(index);
    
    DataManager::sharedDataManager()->setTouchGridDirect(clickGrid);
    
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
            vector<int> phraseWordsIndexVec;
            vector<int> phrase2WordsIndexVec;
            vector<int> wordsIndexVec;
            phraseWordsIndexVec.clear();
            phrase2WordsIndexVec.clear();
            wordsIndexVec.clear();
            
            //直接写it = DataManager::sharedDataManager()->getGrids().begin();为乱数据 why?
            vector<Grid*> gridsTemp = DataManager::sharedDataManager()->getGrids();
            for (vector<Grid*>::iterator it = gridsTemp.begin(); it != gridsTemp.end(); ++it)
            {
                Grid *grid = *it;
                int gridPhraseIndex = grid->getPhraseIndex();
                int gridPhrase2Index = grid->getPhrase2Index();
                if (phraseIndex != -1 && gridPhraseIndex == phraseIndex)
                {
                    phraseWordsIndexVec.push_back(grid->getIndex());
                }
                
                if (phrase2Index != -1 && gridPhrase2Index == phrase2Index)
                {
                    phrase2WordsIndexVec.push_back(grid->getIndex());
                }
                
                //设置wordsIndexVec
                if (phraseIndex != -1 && gridPhraseIndex == phraseIndex)
                {
                    wordsIndexVec.push_back(grid->getIndex());
                    continue;//去掉交叉点
                }
                
                if (phrase2Index != -1 && gridPhrase2Index == phrase2Index)
                {
                    wordsIndexVec.push_back(grid->getIndex());
                }
                
            }

            touchGridEvent->setWordsIndexVector(wordsIndexVec);
            
            DataManager::sharedDataManager()->setPhraseWordsIndexVec(phraseWordsIndexVec);
            DataManager::sharedDataManager()->setPhrase2WordsIndexVec(phrase2WordsIndexVec);
            
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


void GameController::handleChooseAnswerEvent(Event *event)
{
    ChooseAnswerEvent *cae = (ChooseAnswerEvent*)event;
    Grid *touchGrid = DataManager::sharedDataManager()->getTouchGridDirect();
    if (touchGrid != NULL)
    {
        int touchGridIndex = touchGrid->getIndex();
        
        bool flag = false;
        vector<Answer*> selectAnswerVec = DataManager::sharedDataManager()->getSelectAnswerVec();
        vector<Answer*>::iterator it;
        
        for (it = selectAnswerVec.begin(); it != selectAnswerVec.end(); it++)
        {
            Answer *a = *it;
            if (a->getIndex() == touchGridIndex)
            {
                int answerTag = cae->getIndex();
                a->setAnswerWord(DataManager::sharedDataManager()->getAnswers().at(answerTag));
                //更新m_isSame
                a->setIsSame();
                
                flag = true;
                
                CCLog("tag is %d answer is %s", answerTag, a->getAnswerWord().c_str());
                break;
            }
        }
        
        if (flag)
        {
            EventManager::sharedEventManager()->notifyEventSucceeded(event);
            
            bool isFix = checkAnswerFix(touchGrid->getPhraseIndex(), touchGrid->getPhrase2Index());
            if (!isFix)
            {
                int nextTouchIndex = autoFindNextTouchGrid(touchGridIndex, touchGrid->getPhraseIndex(), touchGrid->getPhrase2Index());
                if (nextTouchIndex != -1)
                {
                    TouchGridEvent *touchGridEvent = new TouchGridEvent();
                    touchGridEvent->setIndex(nextTouchIndex);
                    
                    EventManager::sharedEventManager()->addEvent(touchGridEvent);
                }
            }

            
        }
        else
        {
            EventManager::sharedEventManager()->notifyEventFailed(event);
        }
        
        
    }
    EventManager::sharedEventManager()->notifyEventFailed(event);
}


bool GameController::checkAnswerFix(int phraseIndex, int phrase2Index)
{
    FixAnswerEvent *fixAnswerEvent = new FixAnswerEvent();
    
    bool isFix = false;
    
    vector<Answer*> selectAnswerVec = DataManager::sharedDataManager()->getSelectAnswerVec();
    int size = selectAnswerVec.size();
    vector<int> indexVec;
    
    if (phraseIndex != -1)
    {
        int answerPhraseIndex = -1;
        bool flag = true;
        for (int i = 0; i < size; i++)
        {
            Answer *a = selectAnswerVec.at(i);
            answerPhraseIndex = a->getPhraseIndex();
            if (answerPhraseIndex == phraseIndex && !a->getIsFix())
            {
                if (!a->getIsSame())
                {
                    flag = false;
                    break;
                }
                else
                {
                    indexVec.push_back(i);
                }
            }
        }
        
        //m_isSame全true
        if (flag)
        {
            for (int j = 0; j < indexVec.size(); j++)
            {
                int index = indexVec.at(j);
                Answer *a = selectAnswerVec.at(index);
                a->setIsFix(true);
            }
            
            DataManager::sharedDataManager()->setRightWordsIndexVec(phraseIndex);
            
            fixAnswerEvent->setPhraseIndex(phraseIndex);
            
            isFix = true;
            
            //合作模式
            if (DataManager::sharedDataManager()->getGameType() == GameTypeCooperation)
            {
                DataManager::sharedDataManager()->setCoopOwnRightWordsIndexVec(phraseIndex);
            }
        }
        indexVec.clear();
    }
    
    
    if (phrase2Index != -1)
    {
        int answerPhrase2Index = -1;
        bool flag = true;
        for (int i = 0; i < size; i++)
        {
            Answer *a = selectAnswerVec.at(i);
            answerPhrase2Index = a->getPhrase2Index();
            if (answerPhrase2Index == phrase2Index && !a->getIsFix())
            {
                if (!a->getIsSame())
                {
                    flag = false;
                    break;
                }
                else
                {
                    indexVec.push_back(i);
                }
            }
        }
        
        //m_isSame全true
        if (flag)
        {
            for (int j = 0; j < indexVec.size(); j++)
            {
                int index = indexVec.at(j);
                Answer *a = selectAnswerVec.at(index);
                a->setIsFix(true);
            }
            
            DataManager::sharedDataManager()->setRightWordsIndexVec(phrase2Index);
            
            fixAnswerEvent->setPhrase2Index(phrase2Index);
            
            isFix = true;
            
            //合作模式
            if (DataManager::sharedDataManager()->getGameType() == GameTypeCooperation)
            {
                DataManager::sharedDataManager()->setCoopOwnRightWordsIndexVec(phrase2Index);
            }
        }
        indexVec.clear();
    }
    
    if (isFix)//有匹配的词语
    {
        EventManager::sharedEventManager()->addEvent(fixAnswerEvent);
    }
    else
    {
        delete fixAnswerEvent;//?
    }
    
    return isFix;
}

bool GameController::checkGameWin()
{
    return DataManager::sharedDataManager()->isWin();
}

int GameController::autoFindNextTouchGrid(int touchGridIndex, int phraseIndex, int phrase2Index)
{
    int index = -1;
    bool isVertical = DataManager::sharedDataManager()->getIsVertical();
    if (phraseIndex != -1 && !isVertical)
    {
        vector<int> phraseWordsIndexVec = DataManager::sharedDataManager()->getPhraseWordsIndexVec();
        int size = phraseWordsIndexVec.size();
        int nowVecIndex = 0;
        for (int i = 0; i < size; i++)
        {
            if (touchGridIndex == phraseWordsIndexVec.at(i))
            {
                nowVecIndex = i;
                break;
            }
        }
        
        vector<Answer*> selectAnswerVec = DataManager::sharedDataManager()->getSelectAnswerVec();
        vector<Answer*>::iterator it;
        
        bool flag = true;
        int times = 0;//防止无限循环 其实应该不会出现这种情况
        while (flag)
        {
            int nextVecIndex = ++nowVecIndex % size;
            int indexTemp = phraseWordsIndexVec.at(nextVecIndex);
            
            for (it = selectAnswerVec.begin(); it != selectAnswerVec.end(); it++)
            {
                Answer *a = *it;
                if (a->getIndex() == indexTemp)
                {
                    if (!a->getIsFix())//整个词语还不是正确答案时
                    {
                        index = indexTemp;
                        flag = false;
                    } //否则跳到下一个字
                    break;
                }
            }
            
            times++;
            if (times == size)
            {
                flag = false;
                index = -1;
            }
        }
        
        return index;
    }
    
    
    
    if (phrase2Index != -1)
    {
        vector<int> phrase2WordsIndexVec = DataManager::sharedDataManager()->getPhrase2WordsIndexVec();
        int size = phrase2WordsIndexVec.size();
        int nowVecIndex = 0;
        for (int i = 0; i < size; i++)
        {
            if (touchGridIndex == phrase2WordsIndexVec.at(i))
            {
                nowVecIndex = i;
                break;
            }
        }
        
        vector<Answer*> selectAnswerVec = DataManager::sharedDataManager()->getSelectAnswerVec();
        vector<Answer*>::iterator it;
        
        bool flag = true;
        int times = 0;//防止无限循环 其实应该不会出现这种情况
        while (flag)
        {
            int nextVecIndex = ++nowVecIndex % size;
            int indexTemp = phrase2WordsIndexVec.at(nextVecIndex);
            
            for (it = selectAnswerVec.begin(); it != selectAnswerVec.end(); it++)
            {
                Answer *a = *it;
                if (a->getIndex() == indexTemp)
                {
                    if (!a->getIsFix())//整个词语还不是正确答案时
                    {
                        index = indexTemp;
                        flag = false;
                    } //否则跳到下一个字
                    break;
                }
            }
            
            times++;
            if (times == size)
            {
                flag = false;
                index = -1;
            }
        }
        
        if (index != -1)
        {
            DataManager::sharedDataManager()->setIsVertical(true);
        }
        
        return index;
    }
    
    return index;
}
