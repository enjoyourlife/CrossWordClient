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
#include "Hall.h"
#include "SingleRoom.h"
#include "SingleSubRoom.h"
#include "CompetitiveRoom.h"
#include "WaitingRoom.h"
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
    
    m_nowScene = createScene(type);
	CCDirector::sharedDirector()->replaceScene(m_nowScene);
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
            
        case SceneTypeSingleRoom:
            scene = SingleRoom::scene();
            break;
            
        case SceneTypeSingleSubRoom:
            scene = SingleSubRoom::scene();
            break;
            
        case SceneTypeHall:
            scene = Hall::scene();
            break;
            
        case SceneTypeCompetitiveRoom:
            scene = CompetitiveRoom::scene();
            break;
            
        case SceneTypeWaitingRoom:
            scene = WaitingRoom::scene();
            break;
            
        case SceneTypeMainLayer:
            scene = MainLayer::scene();
            break;
            
        default:
            break;
	}
	return scene;
}

CCScene* SceneManager::getCurrentScene()
{
    return m_nowScene;
}
