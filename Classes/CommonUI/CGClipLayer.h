//
//  CGClipLayer.h
//  CrossWordClient
//
//  Created by cy on 14-6-14.
//
//

#ifndef __CrossWordClient__CGClipLayer__
#define __CrossWordClient__CGClipLayer__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"

class CGClipLayer : public cocos2d::CCLayer
{
public:
    CGClipLayer();
    
    ~CGClipLayer();
    
    /*!
     * @brief	创建一个MainLayer实例。
     */
    static CGClipLayer* create();
    
    /*!
     * @brief		覆盖CCLayer类的init方法。
     * @see         CCLayer::init
     */
    virtual bool init();
    
    void visit();
};

class CGClipLayerLoader : public cocos2d::extension::CCLayerLoader
{
public:
	/*!
     * @brief	创建一个PokerSpriteLoader实例。
     * @see     CCNodeLoader::loader
     */
	CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(CGClipLayerLoader, loader);
    
protected:
	/*!
     * @brief	创建一个GameButtonsLayer实例。
     * @see     CCNodeLoader::createCCNode
     */
	CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(CGClipLayer);
};

#endif /* defined(__CrossWordClient__CGClipLayer__) */
