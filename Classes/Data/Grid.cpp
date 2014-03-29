//
//  Grid.cpp
//  CrossWordClient
//
//  Created by cy on 14-3-20.
//
//

#include "Grid.h"

Grid::Grid(int index, int type)
{
    m_index = index;
    m_type = type;
}

Grid::~ Grid()
{
    
}

float Grid::getPosX()
{
    return m_posX;
}

float Grid::getPosY()
{
    return m_posY;
}

int Grid::getIndex()
{
    return m_index;
}

int Grid::getType()
{
    return m_type;
}
