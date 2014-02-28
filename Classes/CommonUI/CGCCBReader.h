//
//  CGCCBReader.h
//  CrossWordClient
//
//  Created by cy on 14-2-22.
//
//

#ifndef __CrossWordClient__CGCCBReader__
#define __CrossWordClient__CGCCBReader__

#include <iostream>
#include "CCBReader.h"
#include "CCNodeLoaderLibrary.h"

class CGCCBReader : public cocos2d::extension::CCBReader
{
public:
	CGCCBReader(cocos2d::extension::CCNodeLoaderLibrary * pCCNodeLoaderLibrary);
	virtual ~ CGCCBReader();
    
    cocos2d::CCNode * readCCBFile(const char * pCCBFileName, CCObject * pOwner = NULL);
    
	void setStandardSize(const cocos2d::CCSize& size);
	const cocos2d::CCSize& getStandardSize() const;
	
    float getCCBScale();
    
protected:
	void calculateResolutionScale();
	std::string getCCBFile();
    
private:
	cocos2d::CCSize m_standardSize;
    /*
     ccb中元素应该缩放的比例 用在cocosbuilder不方便直接缩放的场合
     Android800X480 0.62  (以768为标准 即美术是以1024x768为标准给图的 480/768=0.625)
     Android480X320 0.42
     Android1280X720 0.94
     Android1280X800 1.04
     Android960X640 0.83
     */
    float m_ccbScale;
};

#endif /* defined(__CrossWordClient__CGCCBReader__) */
