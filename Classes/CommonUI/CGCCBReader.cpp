//
//  CGCCBReader.cpp
//  CrossWordClient
//
//  Created by cy on 14-2-22.
//
//

#include "CGCCBReader.h"

using namespace std;
USING_NS_CC;
USING_NS_CC_EXT;

#define DEFAULT_STANDARD_WIDTH	1024
#define DEFAULT_STANDARD_HEIGHT	768

CGCCBReader::CGCCBReader(CCNodeLoaderLibrary * pCCNodeLoaderLibrary) : CCBReader(pCCNodeLoaderLibrary)
{
    setResolutionScale(1.0f);
	
	m_standardSize.width = DEFAULT_STANDARD_WIDTH;
	m_standardSize.height = DEFAULT_STANDARD_HEIGHT;
    
    m_ccbScale = 1.0f;
}

CGCCBReader::~ CGCCBReader()
{
}

void CGCCBReader::setStandardSize(const cocos2d::CCSize &size)
{
	m_standardSize = size;
}

const CCSize& CGCCBReader::getStandardSize() const
{
	return m_standardSize;
}

void CGCCBReader::calculateResolutionScale()
{
	CCAssert(m_standardSize.width > 0 && m_standardSize.height > 0, "Standard size is invalid!");
	
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	float winRatio = winSize.width / winSize.height;
	float standardRatio = m_standardSize.width / m_standardSize.height;
	if (winRatio > standardRatio)
	{
		setResolutionScale(winSize.height / m_standardSize.height);//设置分辨率比 即设置0.62等等
	}
	else
	{
		setResolutionScale(winSize.width / m_standardSize.width);
	}
}

CCNode * CGCCBReader::readCCBFile(const char * pCCBFileName, CCObject * pOwner)
{
	setStandardSize(CCSize(1024,768));
    //getCCBFile()获取ccbi的路径
	CCString* filename = CCString::createWithFormat("%s/%s", getCCBFile().c_str(), pCCBFileName);
    
    //以1024X768为标准 计算其他分辨率的比
	calculateResolutionScale();
//    CCFileUtils::sharedFileUtils()->setPopupNotify(false);//找不到ccbi不弹出提示
    
    setCCBRootPath("");
	CCNode* node = readNodeGraphFromFile(filename->getCString(), pOwner);
    
    return node;
}

std::string CGCCBReader::getCCBFile()
{
	CCSize winsize = CCDirector::sharedDirector()->getWinSize();
	float scale = winsize.width/winsize.height;
    
	std::vector<float> scaleArray;
	scaleArray.push_back(1.333);
	scaleArray.push_back(1.5);
	scaleArray.push_back(1.667);
	scaleArray.push_back(1.778);
	float tureScale = 1;
	int scaleIndex = 0;
	float minNum = 1000;
	for (unsigned int i = 0; i < scaleArray.size(); i++)
	{
		float tmpScale = scaleArray.at(i);
		if (minNum > (fabsf(fabsf(tmpScale) - fabsf(scale))))
		{
			minNum = fabsf(fabsf(tmpScale) - fabsf(scale));
			scaleIndex = i;
		}
	}
	tureScale = scaleArray.at(scaleIndex);
	CCString* filepath = NULL;
	if (fabsf(1.333 - tureScale) < 0.00001 )
	{
		filepath = CCString::create("CCB-1024X768");
        m_ccbScale = 1.0f;
	}
	else if (fabsf(1.5 - tureScale) < 0.00001)
	{
		filepath = CCString::create("CCB-960X640");
        m_ccbScale = 0.83f;
	}
	else if (fabsf(1.667 - tureScale) < 0.00001)
	{
		filepath = CCString::create("CCB-800X480");
        m_ccbScale = 0.62f;
	}
	else if (fabsf(1.778 - tureScale) < 0.00001)
	{
		filepath = CCString::create("CCB-1280X720");
        m_ccbScale = 0.94f;
	}
	return filepath->getCString();
}

float CGCCBReader::getCCBScale()
{
    return m_ccbScale;
}
