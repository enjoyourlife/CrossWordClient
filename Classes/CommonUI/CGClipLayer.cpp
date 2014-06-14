//
//  CGClipLayer.cpp
//  CrossWordClient
//
//  Created by cy on 14-6-14.
//
//

#include "CGClipLayer.h"

USING_NS_CC;

CGClipLayer::CGClipLayer()
{
    
}

CGClipLayer::~CGClipLayer()
{
    
}

void CGClipLayer::visit()
{
    glEnable(GL_SCISSOR_TEST);
    
    CCPoint point = convertToWorldSpace(CCPointZero);
    CCDirector::sharedDirector()->getOpenGLView()->setScissorInPoints(point.x, point.y, this->getContentSize().width, this->getContentSize().height);
    
    CCNode::visit();
    glDisable(GL_SCISSOR_TEST);
}

CGClipLayer* CGClipLayer::create()
{
    CGClipLayer *layer = new CGClipLayer();
    if (layer && layer->init())
    {
        layer->autorelease();
    }
    else
    {
        delete layer;
        layer = NULL;
    }
    return layer;
}

bool CGClipLayer::init()
{
    if (!CCLayer::init())
    {
        return false;
    }
    return true;
}