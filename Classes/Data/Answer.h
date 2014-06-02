//
//  Answer.h
//  CrossWordClient
//
//  Created by cy on 14-4-28.
//
//  玩家选择的答案类

#ifndef __CrossWordClient__Answer__
#define __CrossWordClient__Answer__

#include <iostream>

class Answer
{
public:
    Answer(int index, int phraseIndex, int wordIndex, int phrase2Index, int word2Index);
    ~ Answer();
    
    int getIndex();
    bool getIsSame();
    bool getIsFix();
    
    int getPhraseIndex();
    int getWordIndex();
    int getPhrase2Index();
    int getWord2Index();
    
    const std::string& getOriAnswerWord();
    const std::string& getAnswerWord();
   
    
    void setOriAnswerWord(const std::string& oriAnswerWord);
    void setAnswerWord(const std::string& answerWord);
    void setIsSame();
    void setIsSameEx(bool isSame);
    void setIsFix(bool isFix);
    
    /*
    void setPhrase2Index(int phrase2Index);
    void setWord2Index(int word2Index);
     */
    
public:
    
    
private:
    //从左到右 从上到下 的第几个  从0开始计数
    int m_index;
    //第几个成语
    int m_phraseIndex;
    //成语的第几个字
    int m_wordIndex;
    //如果是交叉 则属于两个成语
    int m_phrase2Index;
    int m_word2Index;
    
    //成语原来的字
    std::string m_oriAnswerWord;
    //选择的答案字
    std::string m_answerWord;
    
    //选择的字是否和原字一样
    bool m_isSame;
    //是否已经是正确答案 不能更改了
    bool m_isFix;
};

#endif /* defined(__CrossWordClient__Answer__) */
