//
//  LoadingLayer.h
//  SnakeClient
//
//  Created by cy on 13-12-21.
//
//

#ifndef __SnakeClient__LoadingLayer__
#define __SnakeClient__LoadingLayer__

#include <iostream>
#include "cocos2d.h"

class LoadingLayer : public cocos2d::CCLayerColor
{
public:
    LoadingLayer();
    ~ LoadingLayer();
    
    CREATE_FUNC(LoadingLayer);
    /*!
     * @brief	创建仅包含一个LoadingLayer的场景。
     */
    static cocos2d::CCScene* scene();
    
    /*!
     * @brief		覆盖CCLayer类的init方法。
     * @see         CCLayer::init
     */
    virtual bool init();
    
    /*!
     * @brief		覆盖CCLayer类的onEnter方法。
     * @see         cocos2d::CCLayer::onEnter
     */
    virtual void onEnter();
    
    /*!
     * @brief		覆盖CCLayer类的onExit方法。
     * @see         cocos2d::CCLayer::onExit
     */
    virtual void onExit();
    
    /*!
     * @brief		覆盖CCLayer类的update方法。
     * @see         CCLayer::update
     */
    virtual void update(float time);
    
private:
    
    virtual void registerWithTouchDispatcher(void);
    /*!
     * @brief		覆盖CCLayer类的ccTouchBegan方法。
     * @see         cocos2d::CCLayer::ccTouchBegan
     */
    virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    /*!
     * @brief		覆盖CCLayer类的ccTouchMoved方法。
     * @see         cocos2d::CCLayer::ccTouchMoved
     */
    virtual void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    /*!
     * @brief		覆盖CCLayer类的ccTouchEnded方法。
     * @see         cocos2d::CCLayer::ccTouchEnded
     */
    virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    /*!
     * @brief		覆盖CCLayer类的ccTouchCancelled方法。
     * @see         cocos2d::CCLayer::ccTouchCancelled
     */
    virtual void ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    /*!
     * @brief	进度条图片。
     */
    cocos2d::CCSprite* m_loadingPanel;
    
    /*!
     * @brief	进度条控制器。
     */
    cocos2d::CCProgressTimer* m_loadingProgress;
};

#endif /* defined(__SnakeClient__LoadingLayer__) */
