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