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
	
protected:
	void calculateResolutionScale();
	std::string getCCBFile();
    
private:
	cocos2d::CCSize m_standardSize;
};

#endif /* defined(__CrossWordClient__CGCCBReader__) */
