//
//  DataManager.cpp
//  CrossWordClient
//
//  Created by cy on 14-2-28.
//
//

#include "DataManager.h"
#include "../Common/Utilities.h"
#include "DBManager.h"

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
    
    
    //单机相关
    initLocalUser();
    m_localBonusJson = Utilities::getJsonFromFile("Configs/localBonus.json");
    initLocalUnLockLevel();//在initLocalUser()之后
    
    
    //联网相关
    m_userUuid = "";
    m_username = "";
    initOwnOnLineUser();
    
    OnLineUser *temp = new OnLineUser();
    m_otherOnLineUserVec.push_back(temp);
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
    if (gameDataJson == NULL)
    {
        return;
    }
    
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

void DataManager::randomInitLocalWordBonus()
{
    int size = m_words.size();
    if (size > 0)
    {
        srand(time(0));
        
//        int bonusIndex = rand() % size;
        int bonusIndex = size / 2;//固定中间那一个
        int bonusType = rand() % 2 + 1;
        
        json_t* levelsJson = json_object_get(m_localBonusJson, "levels");
        //单机m_level取值0 1 2
        json_t* levelBonusJson = json_array_get(levelsJson, m_level);
        
        float mul = 1.0f;
        if (m_singleSubLevel > 50)
        {
            mul = 2.0f;
        }
        else if (m_singleSubLevel > 40)
        {
            mul = 1.8f;
        }
        else if (m_singleSubLevel > 30)
        {
            mul = 1.6f;
        }
        else if (m_singleSubLevel > 20)
        {
            mul = 1.4f;
        }
        else if (m_singleSubLevel > 10)
        {
            mul = 1.2f;
        }
        
        int bonusValue = 0;
        if (bonusType == 1)//银币
        {
            json_t* silverJson = json_object_get(levelBonusJson, "countdownsilver");
            bonusValue = (int)(json_integer_value(silverJson) * mul);
        }
        else if (bonusType == 2)//经验
        {
            json_t* expJson = json_object_get(levelBonusJson, "countexp");
            bonusValue = (int)(json_integer_value(expJson) * mul);
        }
        
        Words *w = m_words.at(bonusIndex);
        w->setBonusType(bonusType);
        w->setBonusValue(bonusValue);
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

void DataManager::setUserUuid(const string& userUuid)
{
    m_userUuid = userUuid;
}

string& DataManager::getUserUuid()
{
    return m_userUuid;
}

void DataManager::setOwnUid(int ownUid)
{
    m_ownUid = ownUid;
}

int DataManager::getOwnUid()
{
    return m_ownUid;
}

void DataManager::setUsername(const string& username)
{
    m_username = username;
}

string& DataManager::getUsername()
{
    return m_username;
}

void DataManager::initOwnOnLineUser()
{
    m_ownOnLineUser = new OnLineUser();
    m_onLineUserJson = Utilities::getJsonFromFile("Configs/onlineLevelName.json");
}

int DataManager::parseOnLineUserInfo(json_t* userInfo)
{
    //{"code": 200, "info": {"gold": 1, "id": 2, "uid": 2, "exp": 1}} 或者 null
    if (userInfo != NULL)
    {
        json_t *uidJson = json_object_get(userInfo, "uid");
        int uid = json_integer_value(uidJson);
        int gold = json_integer_value(json_object_get(userInfo, "gold"));
        int exp = json_integer_value(json_object_get(userInfo, "exp"));
        if (m_ownUid == uid)
        {
            m_ownOnLineUser->m_uid = uid;
            m_ownOnLineUser->m_gold = gold;
            m_ownOnLineUser->m_exp = exp;
            m_ownOnLineUser->m_username = m_username;
            
            //是否升级 取称号等逻辑
        }
        else
        {
            //目前只有一个对手 如果有多个对手 此方法调用多次 也就增加多个对手了
            OnLineUser *olu = new OnLineUser();
            
            olu->m_uid = uid;
            olu->m_gold = gold;
            olu->m_exp = exp;
            
            //是否升级 取称号等逻辑
            
            m_otherOnLineUserVec.push_back(olu);
        }
        
        return uid;
    }
    return -1;
}

OnLineUser* DataManager::getOwnOnLineUser()
{
    return m_ownOnLineUser;
}

vector<OnLineUser*>& DataManager::getOtherOnLineUserVec()
{
    return m_otherOnLineUserVec;
}

//此处的uid尚未更改
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
        
        //这里变成uid后 应该是数字
        json_t *uidJson = json_object_get(userJson, "uid");
        int uidData = json_integer_value(uidJson);
        
        json_t *chessJson = json_object_get(userJson, "chess");
        size_t chessSize = json_array_size(chessJson);
        
        json_t *rewardsJson = json_object_get(userJson, "rewards");
        json_t *goldJson = json_object_get(rewardsJson, "gold");
        json_t *expJson = json_object_get(rewardsJson, "exp");
        int gold = json_integer_value(goldJson);
        int exp = json_integer_value(expJson);
        
        if (uidData == m_ownUid)//自己
        {
            for (int j = 0; j < chessSize; j++)
            {
                json_t *cJson = json_array_get(chessJson, j);
                int c = json_integer_value(cJson);
                m_ownChessVec.push_back(c);
            }
            
            m_ownOnLineUser->m_gold = gold;
            m_ownOnLineUser->m_exp = exp;
        }
        else//其他玩家 这里是2个玩家的情况 后期要修改
        {
            for (int j = 0; j < chessSize; j++)
            {
                json_t *cJson = json_array_get(chessJson, j);
                int c = json_integer_value(cJson);
                m_chessVec.push_back(c);
            }
            
            m_otherOnLineUserVec.at(0)->m_gold = gold;
            m_otherOnLineUserVec.at(0)->m_exp = exp;
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

void DataManager::initLocalUser()
{
    m_localUser = DBManager::sharedDBManager()->getLocalUserByUsername("user");
    m_localUser->m_localUserJson = Utilities::getJsonFromFile("Configs/localLevelName.json");
}

LocalUser* DataManager::getLocalUser()
{
    return m_localUser;
}

void DataManager::updateLocalUser(int exp)
{
    int preLevel = m_localUser->m_lv;
    
    json_t* levelsJson = json_object_get(m_localUser->m_localUserJson, "levels");
    size_t levelsSize = json_array_size(levelsJson);
    if (preLevel == levelsSize)
    {
        return;
    }
    
    //更新实时的m_localUser
    int nowLocalUserExp = m_localUser->m_exp + exp;
    
    //下一级的json
    json_t* nextLevelJson = json_array_get(levelsJson, preLevel);
    json_t* nextExpJson = json_object_get(nextLevelJson, "experience");
    int nextExp = json_integer_value(nextExpJson);
    if (nowLocalUserExp >= nextExp)//升级了
    {
        m_localUser->m_isLevelUp = true;
        
        int nowLevel = preLevel + 1;//只升一级
        m_localUser->m_lv = nowLevel;
        
        if (nowLevel == levelsSize)//满级
        {
            nowLocalUserExp = nextExp;//经验设为最大值
            m_localUser->m_nextLvExp = nextExp;//满级 下一级经验为当前级别的经验
        }
        m_localUser->m_exp = nowLocalUserExp;
        
        json_t* nextNameJson = json_object_get(nextLevelJson, "name");
        m_localUser->m_name = json_string_value(nextNameJson);
        
        //没有满级
        m_localUser->m_nextLvExp = json_integer_value(json_object_get(json_array_get(levelsJson, nowLevel), "experience"));;
    }
    else
    {
        m_localUser->m_exp = nowLocalUserExp;
        m_localUser->m_nextLvExp = nextExp;
    }
    
    //更新数据库
    DBManager::sharedDBManager()->updateLocalUserByUsername("user", m_localUser->m_sex, m_localUser->m_exp, m_localUser->m_nextLvExp, m_localUser->m_lv, m_localUser->m_name, m_localUser->m_silver, m_localUser->m_version);
    
    //连升两级 避免这种情况出现 则上面算法可以用
    /*
    int nowLevel = 1;
    for (unsigned int i = 0; i < levelsSize; i++)
    {
        json_t *levelJson = json_array_get(levelsJson, i);
        json_t* expJson = json_object_get(levelJson, "experience");
        int exp = json_integer_value(expJson);
        if (nowLocalUserExp >= exp)
        {
            nowLevel = json_integer_value(json_object_get(levelJson, "level"));
            if (i == levelsSize - 1)//满级
            {
                nowLocalUserExp = exp;//经验设为最大值
                break;
            }
        }
        else
        {
            break;
        }
    }*/
    
}

void DataManager::updateLocalUserSilver(int silver)
{
    if (m_localUser->m_silver >= MAX_SILVER)
    {
        return;
    }
    
    int nowSilver = m_localUser->m_silver + silver;
    if (nowSilver > MAX_SILVER)
    {
        nowSilver = MAX_SILVER;
    }
    
    m_localUser->m_silver = nowSilver;
    DBManager::sharedDBManager()->updateLocalUserSilver("user", nowSilver);
}

vector<int>& DataManager::getLocalPassBonus()
{
    m_localPassBonus.clear();
    
    json_t* levelsJson = json_object_get(m_localBonusJson, "levels");
    //单机m_level取值0 1 2
    json_t* levelBonusJson = json_array_get(levelsJson, m_level);
    
    float mul = 1.0f;
    if (m_singleSubLevel > 50)
    {
        mul = 2.0f;
    }
    else if (m_singleSubLevel > 40)
    {
        mul = 1.8f;
    }
    else if (m_singleSubLevel > 30)
    {
        mul = 1.6f;
    }
    else if (m_singleSubLevel > 20)
    {
        mul = 1.4f;
    }
    else if (m_singleSubLevel > 10)
    {
        mul = 1.2f;
    }
    
    json_t* silverJson = json_object_get(levelBonusJson, "passsilver");
    int passSilver = (int)(json_integer_value(silverJson) * mul);
    
    json_t* expJson = json_object_get(levelBonusJson, "passexp");
    int passExp = (int)(json_integer_value(expJson) * mul);
    
    m_localPassBonus.push_back(passSilver);
    m_localPassBonus.push_back(passExp);
    
    return m_localPassBonus;
}

vector<int>& DataManager::getLocalEveryBonus()
{
    if (m_localEveryBonus.size() == 2)
    {
        return m_localEveryBonus;
    }
    else
    {
        m_localEveryBonus.clear();
        
        json_t* levelsJson = json_object_get(m_localBonusJson, "levels");
        //单机m_level取值0 1 2
        json_t* levelBonusJson = json_array_get(levelsJson, m_level);
        
        //不需要加成 所以解析m_localBonusJson一次即可
        json_t* silverJson = json_object_get(levelBonusJson, "everysilver");
        int everySilver = json_integer_value(silverJson);
        
        json_t* expJson = json_object_get(levelBonusJson, "everyexp");
        int everyExp = json_integer_value(expJson);
        
        m_localEveryBonus.push_back(everySilver);
        m_localEveryBonus.push_back(everyExp);
        
        
        return m_localEveryBonus;
    }
    
}

int DataManager::getLocalUserExpBase(int level)
{
    int expBase = 100;
    
    json_t* levelsJson = json_object_get(m_localUser->m_localUserJson, "levels");
    size_t levelsSize = json_array_size(levelsJson);
    
    if (levelsSize == level)//满级
    {
        json_t* lastLevelJson = json_array_get(levelsJson, level - 1);
        json_t* lastExpJson = json_object_get(lastLevelJson, "experience");
        int lastExp = json_integer_value(lastExpJson);
        
        json_t* preLevelJson = json_array_get(levelsJson, level - 2);
        json_t* preExpJson = json_object_get(preLevelJson, "experience");
        int preExp = json_integer_value(preExpJson);
        
        expBase = lastExp - preExp;
    }
    else if (level >= 1 && level < levelsSize)
    {
        json_t* lastLevelJson = json_array_get(levelsJson, level);
        json_t* lastExpJson = json_object_get(lastLevelJson, "experience");
        int lastExp = json_integer_value(lastExpJson);
        
        json_t* preLevelJson = json_array_get(levelsJson, level - 1);
        json_t* preExpJson = json_object_get(preLevelJson, "experience");
        int preExp = json_integer_value(preExpJson);
        
        expBase = lastExp - preExp;
    }
    
    return expBase;
}

int DataManager::getLocalUserNumerator(int userNowExp, int level)
{
    int numerator = 0;
    
    json_t* levelsJson = json_object_get(m_localUser->m_localUserJson, "levels");
    size_t levelsSize = json_array_size(levelsJson);
    
    if (levelsSize == level)//满级
    {
        json_t* lastLevelJson = json_array_get(levelsJson, level - 1);
        json_t* lastExpJson = json_object_get(lastLevelJson, "experience");
        int lastExp = json_integer_value(lastExpJson);
        
        json_t* preLevelJson = json_array_get(levelsJson, level - 2);
        json_t* preExpJson = json_object_get(preLevelJson, "experience");
        int preExp = json_integer_value(preExpJson);
        
        numerator = lastExp - preExp;
    }
    else if (level >= 1 && level < levelsSize)
    {
        json_t* preLevelJson = json_array_get(levelsJson, level - 1);
        json_t* preExpJson = json_object_get(preLevelJson, "experience");
        int preExp = json_integer_value(preExpJson);
        
        numerator = userNowExp - preExp;
    }
    
    return numerator;
}

void DataManager::initLocalUnLockLevel()
{
    m_localUnLockLevel = DBManager::sharedDBManager()->getLocalUnLockLevelByVersion(m_localUser->m_version);
    m_localUnLockLevel->m_localUnLockLevelJson = Utilities::getJsonFromFile("Configs/localUnLockLevel.json");
}

LocalUnLockLevel* DataManager::getLocalUnLockLevel()
{
    return m_localUnLockLevel;
}

void DataManager::updateLocalUnLockLevel()
{
    //需要更新m_localUnLockLevel 和 数据库
    switch (m_level)
    {
        case 0:
        case 1:
        {
            json_t* levelsJson = json_object_get(m_localUnLockLevel->m_localUnLockLevelJson, "levels");
            json_t* levelJson = json_array_get(levelsJson, m_level);//普通关卡的解锁json
            json_t* unLockNumJson = json_object_get(levelJson, "unlocknum");
            int unLockNum = json_integer_value(unLockNumJson);//普通关卡的解锁条件：简单关卡开启到一定的关数
            
            if (m_level == 0)
            {
                //m_localUnLockLevel->m_unLockSubLevel0 < (m_localUnLockLevel->m_level0Num - 1) 总关卡的限制条件
                if (m_singleSubLevel == m_localUnLockLevel->m_unLockSubLevel0 && m_localUnLockLevel->m_unLockSubLevel0 < (m_localUnLockLevel->m_level0Num - 1))
                {
                    m_localUnLockLevel->m_unLockSubLevel0 += 1;
                    
                    if (m_singleSubLevel == (unLockNum - 1) && m_localUnLockLevel->m_unLockSubLevel1 == -1)//未解锁则解锁普通关卡
                    {
                        m_localUnLockLevel->m_unLockSubLevel1 = 0;//普通第一关
                    }
                }
            }
            else if (m_level == 1)
            {
                if (m_singleSubLevel == m_localUnLockLevel->m_unLockSubLevel1 && m_localUnLockLevel->m_unLockSubLevel1 < (m_localUnLockLevel->m_level1Num - 1))
                {
                    m_localUnLockLevel->m_unLockSubLevel1 += 1;
                    
                    if (m_singleSubLevel == (unLockNum - 1) && m_localUnLockLevel->m_unLockSubLevel2 == -1)//未解锁则解锁困难关卡
                    {
                        m_localUnLockLevel->m_unLockSubLevel2 = 0;//困难第一关
                    }
                }
            }
            
            break;
        }   
            
        case 2:
        {
            if (m_singleSubLevel == m_localUnLockLevel->m_unLockSubLevel2 && m_localUnLockLevel->m_unLockSubLevel2 < (m_localUnLockLevel->m_level2Num - 1))
            {
                m_localUnLockLevel->m_unLockSubLevel2 += 1;
            }
            break;
        }
            
            
        default:
            break;
    }
    
    DBManager::sharedDBManager()->updateLocalUnLockLevelByVersion(m_localUnLockLevel->m_unLockSubLevel0, m_localUnLockLevel->m_unLockSubLevel1, m_localUnLockLevel->m_unLockSubLevel2, m_localUser->m_version);
    
}

void DataManager::saveSelectAnswerVec()
{
    //先删除旧的答案 再保存
    DBManager::sharedDBManager()->deleteSelectAnswer(m_level, m_singleSubLevel, m_localUser->m_version);
    
    vector<Answer*>::iterator answerIt;
    for (answerIt = m_selectAnswerVec.begin(); answerIt != m_selectAnswerVec.end(); answerIt++)
    {
        Answer *a = *answerIt;
        DBManager::sharedDBManager()->addSelectAnswer(m_level, m_singleSubLevel, m_localUser->m_version, a->getIndex(), a->getPhraseIndex(), a->getWordIndex(), a->getPhrase2Index(), a->getWord2Index(), a->getOriAnswerWord(), a->getAnswerWord(), a->getIsSame(), a->getIsFix());
    }
}

void DataManager::loadLastSelectAnswerVec()
{
    vector<Answer*> lastSelectAnswerVec = DBManager::sharedDBManager()->listSelectAnswer(m_level, m_singleSubLevel, m_localUser->m_version);
    //需要给m_selectAnswerVec赋值 才能使得这一局起作用
    if (lastSelectAnswerVec.size() > 0)
    {
        this->initSelectAnswerVec(lastSelectAnswerVec);
        
        //同时需要重新加载m_rightWordsIndexVec 只有m_isFix为true的m_index才是正确的索引
        vector<Answer*>::iterator answerIt;
        for (answerIt = m_selectAnswerVec.begin(); answerIt != m_selectAnswerVec.end(); answerIt++)
        {
            Answer *a = *answerIt;
            if (a->getIsFix())
            {
                //不用if...if 是因为除了交叉的字之外 肯定还有其他字属于某个成语
                if (a->getPhraseIndex() != -1)
                {
                    this->setRightWordsIndexVec(a->getPhraseIndex());
                }
                else
                {
                    this->setRightWordsIndexVec(a->getPhrase2Index());
                }
            }
        }
    }
}

void DataManager::clearLastSelectAnswerVec()
{
    DBManager::sharedDBManager()->deleteSelectAnswer(m_level, m_singleSubLevel, m_localUser->m_version);
}

void DataManager::resetSelectAnswerVec()
{
    for (vector<Answer*>::iterator it = m_selectAnswerVec.begin(); it != m_selectAnswerVec.end(); ++it)
    {
        Answer *a = *it;
        a->setAnswerWord("");
        a->setIsSameEx(false);
        a->setIsFix(false);
    }
}