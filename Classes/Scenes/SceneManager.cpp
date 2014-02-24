//
//  SceneManager.cpp
//  SnakeClient
//
//  Created by cy on 13-12-18.
//
//

#include "SceneManager.h"
#include "LoadingLayer.h"
#include "MainLayer.h"
#include "../HelloWorldScene.h"

using namespace cocos2d;

static SceneManager* s_sceneManager = NULL;

SceneManager* SceneManager::sharedSceneManager()
{
	if (s_sceneManager == NULL)
	{
		s_sceneManager = new SceneManager;
	}
	return s_sceneManager;
}

SceneManager::SceneManager()
{
    m_currentScene		= SceneTypeNone;
	m_lastScene			= SceneTypeNone;
}

SceneManager::~SceneManager()
{
	
}

void SceneManager::startWithScene(SceneType type)
{
	m_currentScene = type;
	CCDirector::sharedDirector()->runWithScene(createScene(type));
}

void SceneManager::changeScene(SceneType type)
{
	m_lastScene = m_currentScene;
	m_currentScene = type;
    
	CCDirector::sharedDirector()->replaceScene(createScene(type));
}

CCScene* SceneManager::createScene(SceneType type)
{
	CCScene* scene = NULL;
	switch (type)
	{
        case SceneTypeNone:
            scene = HelloWorld::scene();
            break;
            
        case SceneTypeLoading:
            scene = LoadingLayer::scene();
            break;
            
            
        default:
            break;
	}
	return scene;
}