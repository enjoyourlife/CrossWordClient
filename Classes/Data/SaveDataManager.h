//
//  SaveDataManager.h
//  CrossWordClient
//
//  Created by cy on 14-3-4.
//
//

#ifndef __CrossWordClient__SaveDataManager__
#define __CrossWordClient__SaveDataManager__

#include <iostream>
#include "cocos2d.h"

class SaveDataManager : public cocos2d::CCObject
{
private:
    SaveDataManager();
    ~SaveDataManager();
    
    void init();
    
public:
    /*!
     * @brief		获取一个SaveDataManager单例。
     */
    static SaveDataManager* sharedSaveDataManager();
    
    void setUsername(const std::string& username);
    const std::string getUsername();
    
    void setPassword(const std::string& password);
    const std::string getPassword();
};

#endif /* defined(__CrossWordClient__SaveDataManager__) */
