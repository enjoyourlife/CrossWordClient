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

class Grid
{
public:
    Grid(int index, int type);
    ~ Grid();
    
    float getPosX();
    float getPosY();
    int getIndex();
    int getType();
private:
    //坐标
    float m_posX;
    float m_posY;
    //从左到右 从上到下 的第几个  从0开始计数
    int m_index;
    //类型序号 也代表第几个成语 0-x
    int m_type;
};

#endif /* defined(__CrossWordClient__Grid__) */
