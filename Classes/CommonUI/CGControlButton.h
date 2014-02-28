//
//  CGControlButton.h
//  CrossWordClient
//
//  Created by cy on 14-2-27.
//
//

#ifndef __CrossWordClient__CGControlButton__
#define __CrossWordClient__CGControlButton__

#include <iostream>
#include "cocos2d.h"
#include "touch_dispatcher/CCTouchDelegateProtocol.h"

class  CGControlButton : public cocos2d::CCSprite, public cocos2d::CCTouchDelegate
{
public:
    virtual ~CGControlButton(void);
    CGControlButton(void);
    static CGControlButton* create();
	virtual bool init();
    inline void setEnabled(bool bEnabled) { m_isEnabled = bEnabled; }
    inline bool isEnabled(void) { return m_isEnabled; }
    void setSwallowsTouches(bool flag);
	void setHighlighted(bool isHighlighted);
	void addTarget(CCObject* target, cocos2d::SEL_MenuHandler action);
    
public:
    virtual bool ccTouchBegan(cocos2d::CCTouch *touch, cocos2d::CCEvent *event);
    virtual void ccTouchMoved(cocos2d::CCTouch *touch, cocos2d::CCEvent *event);
    virtual void ccTouchEnded(cocos2d::CCTouch *touch, cocos2d::CCEvent *event);
    virtual void ccTouchCancelled(cocos2d::CCTouch *touch, cocos2d::CCEvent *event);
    
    virtual void registerWithTouchDispatcher(void);
	virtual void onEnter();
	virtual void onExit();
    
private:
	cocos2d::SEL_MenuHandler m_action;
	CCObject* m_target;
	bool m_isEnabled;
    bool m_isHighlighted;
	bool m_isSwallowsTouches;
    
    /*!
     * @brief	获取某种颜色经过高亮/去掉高亮后的颜色。
     * @param   color           原来的颜色
     * @param   isHighlighted   是否高亮
     * @return  高亮/去高亮后的颜色
     */
    cocos2d::ccColor3B getHighlightedColor(const cocos2d::ccColor3B& color, bool isHighlighted);
};


#endif /* defined(__CrossWordClient__CGControlButton__) */
