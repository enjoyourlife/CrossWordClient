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

unsigned char Utilities::toHex(const unsigned char &x)
{
    return x > 9 ? x -10 + 'A': x + '0';
}

unsigned char Utilities::fromHex(const unsigned char &x)
{
    return isdigit(x) ? x-'0' : x-'A'+10;
}

string Utilities::URLEncode(const char *sIn)
{
    string sOut;
    for( size_t ix = 0; ix < strlen(sIn); ix++ )
    {
        unsigned char buf[4];
        memset( buf, 0, 4 );
        if( isalnum( (unsigned char)sIn[ix] ) )
        {
            buf[0] = sIn[ix];
        }
        else
        {
            buf[0] = '%';
            buf[1] = toHex( (unsigned char)sIn[ix] >> 4 );
            buf[2] = toHex( (unsigned char)sIn[ix] % 16);
        }
        sOut += (char *)buf;
    }
    return sOut;
};

string Utilities::URLDecode(const char *sIn)
{
    string sOut;
    for( size_t ix = 0; ix < strlen(sIn); ix++ )
    {
        unsigned char ch = 0;
        if(sIn[ix]=='%')
        {
            ch = (fromHex(sIn[ix+1])<<4);
            ch |= fromHex(sIn[ix+2]);
            ix += 2;
        }
        else if(sIn[ix] == '+')
        {
            ch = ' ';
        }
        else
        {
            ch = sIn[ix];
        }
        sOut += (char)ch;
    }
    
    return sOut;
    
}

vector<string> Utilities::splitString(const string& str, const string& delimiter)
{
    string::size_type pos;
    vector<string> result;
    
    if (str.find(delimiter) == string::npos)//整个语句没有包含delimiter
    {
        result.push_back(str);
        return result;
    }
    
    int size = str.size();
    int lastIndex = 0;
    
    for(int i = 0; i < size; i++)
    {
        pos = str.find(delimiter, i);
        if(pos < size)
        {
            string s = str.substr(i,pos-i);
            if (s.size() > 0)
            {
                result.push_back(s);
            }
            i = pos + delimiter.size() - 1;
            lastIndex = i;
        }
    }
    
    if (lastIndex + 1 < size)
    {
        string s=str.substr(lastIndex + 1);
        result.push_back(s);
    }
    
    return result;
}
