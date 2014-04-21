//
//  Grid.cpp
//  CrossWordClient
//
//  Created by cy on 14-3-20.
//
//

#include "Grid.h"

using namespace std;

Grid::Grid(int index, int type, int type2, int phraseIndex, int wordIndex, int phrase2Index, int word2Index)
{
    m_index = index;
    m_type = type;
    m_type2 = type2;
    m_phraseIndex = phraseIndex;
    m_wordIndex = wordIndex;
    m_phrase2Index = phrase2Index;
    m_word2Index = word2Index;
    
    m_line = -1;
    m_col = -1;
}


Grid::~ Grid()
{
    
}

int Grid::getLine()
{
    return m_line;
}

void Grid::setLine(int line)
{
    m_line = line;
}

int Grid::getCol()
{
    return m_col;
}

void Grid::setCol(int col)
{
    m_col = col;
}

int Grid::getIndex()
{
    return m_index;
}

int Grid::getType()
{
    return m_type;
}

int Grid::getType2()
{
    return m_type2;
}

const string Grid::getWord()
{
    return m_word;
}

int Grid::getPhraseIndex()
{
    return m_phraseIndex;
}

int Grid::getWordIndex()
{
    return m_wordIndex;
}

int Grid::getPhrase2Index()
{
    return m_phrase2Index;
}

int Grid::getWord2Index()
{
    return m_word2Index;
}

void Grid::setPhrase2Index(int phrase2Index)
{
    m_phrase2Index = phrase2Index;
}

void Grid::setWord2Index(int word2Index)
{
    m_word2Index = word2Index;
}