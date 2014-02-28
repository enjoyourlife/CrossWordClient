//
//  CGControlButton.cpp
//  CrossWordClient
//
//  Created by cy on 14-2-27.
//
//

#include "CGControlButton.h"

USING_NS_CC;

CGControlButton::CGControlButton()
: m_isSwallowsTouches(true)
, m_target(NULL)
, m_action(NULL)
, m_isEnabled(true)
, m_isHighlighted(false)
{
    
}

CGControlButton::~ CGControlButton()
{
}

CGControlButton* CGControlButton::create()
{
	CGControlButton* button = new CGControlButton();
	if (button && button->init())
	{
		button->autorelease();
		return button;
	}
	else
	{
		delete button;
		return NULL;
	}
}

bool CGControlButton::init()
{
	if (!CCSprite::init())
	{
		return false;
	}
	return true;
}

bool CGControlButton::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
	CCPoint p = touch->getLocation();
	p = this->convertToNodeSpace(p);
	CCRect rect = CCRect(0, 0, getContentSize().width, getContentSize().height);
	if (rect.containsPoint(p) && m_isEnabled)
	{
		setHighlighted(true);
		return true;
	}
	else
	{
		return false;
	}
}

void CGControlButton::ccTouchEnded(CCTouch* touch, CCEvent* event)
{
	CCPoint p = touch->getLocation();
	p = this->convertToNodeSpace(p);
	CCRect rect = CCRect(0, 0, getContentSize().width, getContentSize().height);
	setHighlighted(false);
	if (m_target != NULL && rect.containsPoint(p) && m_isEnabled)
	{
		(m_target->*m_action)(this);
	}
}

void CGControlButton::ccTouchCancelled(CCTouch *touch, CCEvent* event)
{
}

void CGControlButton::ccTouchMoved(CCTouch* touch, CCEvent* event)
{
	CCPoint p = touch->getLocation();
	p = this->convertToNodeSpace(p);
	CCRect rect = CCRect(0, 0, getContentSize().width, getContentSize().height);
	if (!rect.containsPoint(p))
	{
		setHighlighted(false);
	}
}

void CGControlButton::onEnter()
{
	CCNode::onEnter();
    registerWithTouchDispatcher();
}

void CGControlButton::onExit()
{
	CCNode::onExit();
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}

void CGControlButton::registerWithTouchDispatcher(void)
{
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority, m_isSwallowsTouches);
}

void CGControlButton::setHighlighted(bool isHighlighted)
{
    if (m_isHighlighted != isHighlighted)
    {
        m_isHighlighted = isHighlighted;
        
        CCArray* children = getChildren();
        if (children != NULL)
        {
            for (unsigned int i = 0; i < children->count(); i++)
            {
                CCRGBAProtocol* node = dynamic_cast<CCRGBAProtocol*>(children->objectAtIndex(i));
                if (node != NULL)
                {
                    ccColor3B color = getHighlightedColor(node->getColor(), isHighlighted);
                    node->setColor(color);
                }
            }
        }
        ccColor3B color = getHighlightedColor(getColor(), isHighlighted);
        setColor(color);
    }
}

ccColor3B CGControlButton::getHighlightedColor(const ccColor3B& color, bool isHighlighted)
{
    // TODO：此算法会造成颜色的误差
    if (isHighlighted)
    {
        return ccc3((color.r + 1) / 2, (color.g + 1) / 2, (color.b + 1) / 2);
    }
    else
    {
        GLbyte r = color.r > 0 ? (2 * color.r - 1) : 0;
        GLbyte g = color.g > 0 ? (2 * color.g - 1) : 0;
        GLbyte b = color.b > 0 ? (2 * color.b - 1) : 0;
        return ccc3(r, g, b);
    }
}

void CGControlButton::addTarget(CCObject* target,SEL_MenuHandler action)
{
	m_target = target;
	m_action = action;
}

void CGControlButton::setSwallowsTouches(bool flag)
{
    if (m_isSwallowsTouches != flag)
    {
        m_isSwallowsTouches = flag;
        CCTouchHandler* handler = CCDirector::sharedDirector()->getTouchDispatcher()->findHandler(this);
        CCTargetedTouchHandler* targetedHandler = dynamic_cast<CCTargetedTouchHandler*>(handler);
        if (targetedHandler != NULL)
        {
            targetedHandler->setSwallowsTouches(m_isSwallowsTouches);
        }
    }
}
