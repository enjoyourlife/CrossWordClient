//
//  Words.cpp
//  CrossWordClient
//
//  Created by cy on 14-4-20.
//
//

#include "Words.h"

using namespace std;

Words::Words(int wordsId, const string& name, const string& tips)
{
    m_wordsId = wordsId;
    m_name = name;
    m_tips = tips;
    
    m_bonusType = 0;
    m_bonusValue = 0;
    m_isGet = false;

}

Words::~ Words()
{
}

int Words::getWordsId()
{
    return m_wordsId;
}

const string& Words::getName()
{
    return m_name;
}

const string& Words::getTips()
{
    return m_tips;
}

void Words::setBonusType(int bonusType)
{
    m_bonusType = bonusType;
}

void Words::setBonusValue(int bonusValue)
{
    m_bonusValue = bonusValue;
}

int Words::getBonusType()
{
    return m_bonusType;
}

int Words::getBonusValue()
{
    return m_bonusValue;
}

void Words::setIsGet(bool isGet)
{
    m_isGet = isGet;
}

bool Words::isGet()
{
    return m_isGet;
}