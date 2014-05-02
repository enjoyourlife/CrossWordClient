//
//  Answer.cpp
//  CrossWordClient
//
//  Created by cy on 14-4-28.
//
//

#include "Answer.h"
using namespace std;

Answer::Answer(int index, int phraseIndex, int wordIndex, int phrase2Index, int word2Index)
{
    m_index = index;
    m_phraseIndex = phraseIndex;
    m_wordIndex = wordIndex;
    m_phrase2Index = phrase2Index;
    m_word2Index = word2Index;
    
    m_answerWord = "";
    m_isSame = false;
    m_isFix = false;
}

Answer::~ Answer()
{
    
}

int Answer::getIndex()
{
    return m_index;
}

bool Answer::getIsSame()
{
    return m_isSame;
}

bool Answer::getIsFix()
{
    return m_isFix;

}

int Answer::getPhraseIndex()
{
    return m_phraseIndex;
}

int Answer::getWordIndex()
{
    return m_wordIndex;
}

int Answer::getPhrase2Index()
{
    return m_phrase2Index;
}

int Answer::getWord2Index()
{
    return m_word2Index;
}

const string& Answer::getOriAnswerWord()
{
    return m_oriAnswerWord;
    
}

const string& Answer::getAnswerWord()
{
    return m_answerWord;
    
}

void Answer::setOriAnswerWord(const string& oriAnswerWord)
{
    m_oriAnswerWord = oriAnswerWord;
}

void Answer::setAnswerWord(const string& answerWord)
{
    m_answerWord = answerWord;
}

void Answer::setIsSame()
{
    if (m_oriAnswerWord.compare(m_answerWord) == 0)
    {
        m_isSame = true;
    }
    else
    {
        m_isSame = false;
    }

}

void Answer::setIsFix(bool isFix)
{
    m_isFix = isFix;

}