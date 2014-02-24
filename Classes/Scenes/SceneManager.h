//
//  SceneManager.h
//  SnakeClient
//
//  Created by cy on 13-12-18.
//
//

#ifndef __SnakeClient__SceneManager__
#define __SnakeClient__SceneManager__

#include <iostream>
#include "cocos2d.h"

typedef enum
{
    SceneTypeNone,
    SceneTypeLoading,
    SceneTypeMainLayer,
    
    SceneTypeMainMenu,
    SceneTypeGamePlay
}
SceneType;

class SceneManager {
public:
	SceneManager();
    ~SceneManager();

    /*!
     * @brief		创建一个SceneManager单例。
     */
    static SceneManager* sharedSceneManager();
    
    /*!
     * @brief		设置启动场景。
     * @param       type   场景类型
     */
    void startWithScene(SceneType type);
    
    /*!
     * @brief		切换场景。
     * @param       type   场景类型
     */
    void changeScene(SceneType type);
    
private:
    /*!
     * @brief		通过场景类型创建场景。
     * @param       type   场景类型
     * @return      创建的场景
     */
    cocos2d::CCScene* createScene(SceneType type);
    
    
    SceneType   m_currentScene;     // 当前场景
    SceneType   m_lastScene;        // 上一个场景

};

#endif /* defined(__SnakeClient__SceneManager__) */
