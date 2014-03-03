//
//  CGToast.cpp
//  CrossWordClient
//
//  Created by cy on 14-3-2.
//
//

#include "CGToast.h"
#include "../CommonUI/CGCCBReader.h"
#include "../Common/Localize.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

CGToast::CGToast()
{
    m_target = NULL;
    m_action = NULL;
    m_toastBg = NULL;
    m_text = NULL;
}

CGToast::~ CGToast()
{

    CC_SAFE_RELEASE_NULL(m_toastBg);
    CC_SAFE_RELEASE_NULL(m_text);
    
}

CGToast* CGToast::create()
{
    CGToast *layer = new CGToast();
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

bool CGToast::init()
{
    if (!CCLayer::init())
    {
        return false;
    }
    
    CCNodeLoaderLibrary *ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
    CGCCBReader reader(ccNodeLoaderLibrary);
    CCNode* node = reader.readCCBFile("toast.ccbi", this);
    addChild(node);
    
    m_text->setString(Localize::sharedLocalize()->getString("toast_txt0"));
    
    return true;
}

void CGToast::addTarget(CCObject* target,SEL_MenuHandler action)
{
	m_target = target;
	m_action = action;
}

void CGToast::playAction(int actionType)
{
    switch (actionType)
    {
        case 0:
            CCDelayTime *delay = CCDelayTime::create(3.0f);
            CCCallFunc *actionDone = CCCallFunc::create(this, callfunc_selector(CGToast::actionDone));
            CCSequence* sequence = CCSequence::createWithTwoActions(delay, actionDone);
            m_toastBg->runAction(sequence);
            break;

    }
}

void CGToast::actionDone()
{
    if (m_target != NULL)
	{
		(m_target->*m_action)(this);
	}
    
    this->removeFromParentAndCleanup(true);
}

SEL_MenuHandler CGToast::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName)
{
    return NULL;
}

SEL_CCControlHandler CGToast::onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName)
{
    return NULL;
}

bool CGToast::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "toastBg", CCSprite*, m_toastBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "text", CCLabelTTF*, m_text);
    return false;
}

