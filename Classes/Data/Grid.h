//
//  Grid.h
//  CrossWordClient
//
//  Created by cy on 14-3-20.
//  格子类
//

#ifndef __CrossWordClient__Grid__
#define __CrossWordClient__Grid__

#include <iostream>

/*
 0xABCD，
 0xAB为第几个成语
 0xCD为第几个字
 */
class Grid
{
public:
    Grid(int index, int type, int type2, int phraseIndex, int wordIndex, int phrase2Index, int word2Index);
    ~ Grid();
    
    int getLine();
    void setLine(int line);
    
    int getCol();
    void setCol(int col);
    
    int getIndex();
    int getType();
    int getType2();
    
    const std::string getWord();
    
    int getPhraseIndex();
    int getWordIndex();
    
    int getPhrase2Index();
    int getWord2Index();
    void setPhrase2Index(int phrase2Index);
    void setWord2Index(int word2Index);
private:
    //坐标 m_line为第几行 m_col第几列
    int m_line;
    int m_col;
    //从左到右 从上到下 的第几个  从0开始计数
    int m_index;
    //类型序号 -1表示没有汉字 0xABCD表示有汉字
    int m_type;
    int m_type2;
    
    //第几个成语
    int m_phraseIndex;
    //成语的第几个字
    int m_wordIndex;
    //如果是交叉 则属于两个成语
    int m_phrase2Index;
    int m_word2Index;
    
    //所包含的字
    std::string m_word;
};

#endif /* defined(__CrossWordClient__Grid__) */
