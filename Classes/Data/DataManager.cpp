//
//  DataManager.cpp
//  CrossWordClient
//
//  Created by cy on 14-2-28.
//
//

#include "DataManager.h"
#include "../Common/Utilities.h"

using namespace std;
using namespace cocos2d;


static DataManager* s_dataManager = NULL;

DataManager::DataManager()
{
    
}

DataManager::~DataManager()
{
    
}

void DataManager::init()
{
    m_singleSubLevel = 0;
    m_isLogin = false;
    m_line = 0;
    m_col = 0;
    
    m_isVertical = false;
    
    initAnswerDic();
    
    m_userUid = "";
    m_username = "";
}

DataManager* DataManager::sharedDataManager()
{
    if (s_dataManager == NULL)
    {
        s_dataManager = new DataManager;
        s_dataManager->init();
    }
    return s_dataManager;
}

GameType DataManager::getGameType()
{
    return m_gameType;
}

void DataManager::setGameType(GameType gameType)
{
    m_gameType = gameType;
}

int DataManager::getLevel()
{
    return m_level;
}

void DataManager::setLevel(int level)
{
    m_level = level;
}

int DataManager::getSingleSubLevel()
{
    return m_singleSubLevel;
}

void DataManager::setSingleSubLevel(int singleSubLevel)
{
    m_singleSubLevel = singleSubLevel;
}

bool DataManager::getIsLogin()
{
    return m_isLogin;
}

void DataManager::setIsLogin(bool isLogin)
{
    m_isLogin = isLogin;
}


void DataManager::parseJson(json_t* gameDataJson)
{
    json_t* wJson = json_object_get(gameDataJson, "w");
    json_t* hJson = json_object_get(gameDataJson, "h");
    m_col = json_integer_value(wJson);
    m_line = json_integer_value(hJson);
    
    //    json_decref(wJson);
    //    json_decref(hJson);
    
    
    //parse words
    clearWords();
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
        m_words.push_back(words);
        //        json_decref(idJson);  //have wrongs
        //        json_decref(nameJson);
        //        json_decref(tipsJson);
        //        json_decref(wordJson);
    }
    
    
    //预先初始化玩家选择的答案
    clearSelectAnswerVec();
    
    //parse map_v and map_h
    clearGrids();
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
            m_grids.push_back(grid);
            
            if (phraseIndex != -1 || phrase2Index != -1)
            {
                Answer *answer = new Answer(index, phraseIndex, wordIndex, phrase2Index, word2Index);
                m_selectAnswerVec.push_back(answer);
            }
            
            index++;
            
        }
    }
    
    
    vector<Answer*>::iterator answerIt;
    for (int i = 0; i < m_words.size(); i++)
    {
        Words *w = m_words.at(i);
        vector<string> vTemp = Utilities::splitString(w->getName(), "*");
        
        for (answerIt = m_selectAnswerVec.begin(); answerIt != m_selectAnswerVec.end(); answerIt++)
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
    
}


vector<Grid*>& DataManager::getGrids()
{
    return m_grids;
}


void DataManager::initGrids(vector<Grid*>& gridVector)
{
    clearGrids();
    m_grids = gridVector;
}

void DataManager::clearGrids()
{
    for (vector<Grid*>::iterator it = m_grids.begin(); it != m_grids.end(); ++it)
    {
        delete *it;
    }
    m_grids.clear();
}


vector<Words*>& DataManager::getWords()
{
    return m_words;
}

void DataManager::initWords(vector<Words*>& words)
{
    clearWords();
    m_words = words;
}

void DataManager::clearWords()
{
    for (vector<Words*>::iterator it = m_words.begin(); it != m_words.end(); ++it)
    {
        delete *it;
    }
    m_words.clear();
}

int DataManager::getLine()
{
    return m_line;
}

int DataManager::getCol()
{
    return m_col;
}

void DataManager::setLine(int line)
{
    m_line = line;
}

void DataManager::setCol(int col)
{
    m_col = col;
}

Grid* DataManager::getTouchGrid(int index)
{
    int size = m_grids.size();
    if (index >= 0 && index < size)
    {
        return m_grids.at(index);
    }
    return NULL;
}

Grid* DataManager::getTouchGridDirect()
{
    return m_touchGrid;
}

void DataManager::setTouchGridDirect(Grid *grid)
{
    m_touchGrid = grid;
}

void DataManager::setPhraseWordsIndexVec(vector<int>& phraseWordsIndexVec)
{
    m_phraseWordsIndexVec = phraseWordsIndexVec;
}

vector<int>& DataManager::getPhraseWordsIndexVec()
{
    return m_phraseWordsIndexVec;
}

void DataManager::setPhrase2WordsIndexVec(vector<int>& phrase2WordsIndexVec)
{
    m_phrase2WordsIndexVec = phrase2WordsIndexVec;
}

vector<int>& DataManager::getPhrase2WordsIndexVec()
{
    return m_phrase2WordsIndexVec;
}

void DataManager::setIsVertical(bool isVertical)
{
    m_isVertical = isVertical;
}

bool DataManager::getIsVertical()
{
    return m_isVertical;
}

void DataManager::initAnswers(vector<string>& answers)
{
    for (vector<string>::iterator it = m_answers.begin(); it != m_answers.end(); ++it)
    {
        *it = "";//right?
    }
    m_answers.clear();
    
    m_answers = answers;
}

vector<string>& DataManager::getAnswers()
{
    return m_answers;
}

void DataManager::initAnswerDic()
{
    string fileName = CCFileUtils::sharedFileUtils()->fullPathForFilename("Text/answers.json");
    CCString *answerDisCstr = CCString::createWithContentsOfFile(fileName.c_str());
    m_answerDic = Utilities::splitString(answerDisCstr->m_sString, "*");
}

vector<string>& DataManager::getAnswerDic()
{
    return m_answerDic;
}

void DataManager::clearSelectAnswerVec()
{
    for (vector<Answer*>::iterator it = m_selectAnswerVec.begin(); it != m_selectAnswerVec.end(); ++it)
    {
        delete *it;
    }
    m_selectAnswerVec.clear();
}

void DataManager::initSelectAnswerVec(vector<Answer*>& selectAnswerVec)
{
    clearSelectAnswerVec();
    
    m_selectAnswerVec = selectAnswerVec;
    
    /*
    for (vector<Answer*>::iterator it = m_selectAnswerVec.begin(); it != m_selectAnswerVec.end(); ++it)
    {
        CCLog("answer index %d %s", (*it)->getIndex(), (*it)->getOriAnswerWord().c_str());
        delete *it;
    }*/
}

vector<Answer*>& DataManager::getSelectAnswerVec()
{
    return m_selectAnswerVec;
}

void DataManager::setRightWordsIndexVec(int index)
{
    bool flag = true;
    //防止重复放入
    for (vector<int>::iterator it = m_rightWordsIndexVec.begin(); it != m_rightWordsIndexVec.end(); ++it)
    {
        int temp = *it;
        if (temp == index)
        {
            flag = false;
        }
    }
    if (flag)
    {
        m_rightWordsIndexVec.push_back(index);
    }
    
}

vector<int>& DataManager::getRightWordsIndexVec()
{
    return m_rightWordsIndexVec;
}

void DataManager::clearRightWordsIndexVec()
{
    m_rightWordsIndexVec.clear();
}

bool DataManager::isWin()
{
    bool flag = false;
    
    int rightSize = m_rightWordsIndexVec.size();
    int wordsSize = m_words.size();
    
    if (rightSize == wordsSize)
    {
        flag = true;
    }
    
    return flag;
}

void DataManager::setUserUid(const string& userUid)
{
    m_userUid = userUid;
}

string& DataManager::getUserUid()
{
    return m_userUid;
}

void DataManager::setUsername(const string& username)
{
    m_username = username;
}

string& DataManager::getUsername()
{
    return m_username;
}

void DataManager::parseGameProcJson(json_t* gameProcJson)
{
    json_t* usersJson = json_object_get(gameProcJson, "users");
    size_t usersSize = json_array_size(usersJson);
    
    if (usersSize > 0)//防止gameProcJson有错
    {
        m_chessVec.clear();
        m_ownChessVec.clear();
    }
    
    for (int i = 0; i < usersSize; i++)
    {
        json_t *userJson = json_array_get(usersJson, i);
        
        json_t *uidJson = json_object_get(userJson, "uid");
        const char *uidData = json_string_value(uidJson);
        
        json_t *chessJson = json_object_get(userJson, "chess");
        size_t chessSize = json_array_size(chessJson);
        
        if (m_username.compare(uidData) == 0)
        {
            for (int j = 0; j < chessSize; j++)
            {
                json_t *cJson = json_array_get(chessJson, j);
                int c = json_integer_value(cJson);
                m_ownChessVec.push_back(c);
            }
        }
        else
        {
            for (int j = 0; j < chessSize; j++)
            {
                json_t *cJson = json_array_get(chessJson, j);
                int c = json_integer_value(cJson);
                m_chessVec.push_back(c);
            }
        }
    }
    
    
}

vector<int>& DataManager::getChessVec()
{
    return m_chessVec;
}

vector<int>& DataManager::getOwnChessVec()
{
    return m_ownChessVec;
}



