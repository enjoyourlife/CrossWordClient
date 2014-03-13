//
//  Utilities.cpp
//  CrossWordClient
//
//  Created by cy on 14-3-2.
//
//

#include "Utilities.h"

USING_NS_CC;
using namespace std;

//记得在用完json后删除
json_t* Utilities::getJsonFromFile(const char* fileName)
{
    string name = CCFileUtils::sharedFileUtils()->fullPathForFilename(fileName);
    
    unsigned long size = 0;
    unsigned char * pBytes = CCFileUtils::sharedFileUtils()->getFileData(name.c_str(), "rb", &size);
    
    json_t *json;
    json_error_t error;
    
    const char *buffer = (const char *)pBytes;
    
    json = json_loadb(buffer, size, 0, &error);
    if(!json) {
        CCLog("error line is %d", error.line);
        CCLog("error text is %s", error.text);
    }
    
    CC_SAFE_DELETE_ARRAY(pBytes);
    
    /*
    json_t *json;
    json_error_t error;
    
    json = json_load_file(name.c_str(), 0, &error);
    if(!json) {
        CCLog("error line is %d", error.line);
        CCLog("error text is %s", error.text);
    }*/ //这种方法在android机中读不到文件 估计json_load_file不是以rb格式读取
    
    return json;
}

CCAnimate* Utilities::createAnimate(const char* fileName, int frameCount, float delay)
{
    CCArray* frameArray = CCArray::create();
    for (int i = 1; i <= frameCount; i++)
    {
        CCString* name = CCString::createWithFormat("%s%02d.png", fileName, i);
        CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(name->getCString());
        
        frameArray->addObject(frame);
    }
    CCAnimation* animation = CCAnimation::createWithSpriteFrames(frameArray, delay);
    return CCAnimate::create(animation);
}