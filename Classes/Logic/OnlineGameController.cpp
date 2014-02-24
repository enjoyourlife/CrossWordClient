//
//  OnlineGameController.cpp
//  SnakeClient
//
//  Created by cy on 13-12-11.
//
//

#include "OnlineGameController.h"
#include "../Events/EventManager.h"

OnlineGameController::OnlineGameController()
{
    
}

OnlineGameController::~OnlineGameController()
{
}

void OnlineGameController::startEvent(int event, void* data)
{
    switch (event)
    {
        case EventTypeLogin:
            break;
            
        default:
            break;
	}
}

void OnlineGameController::startEvent(Event *event)
{
    int type = event->getType();
    switch (type)
    {
        case EventTypeLogin:
        {
            
            break;
        }
            
        default:
            break;
            
    }
}

void OnlineGameController::cancelEvent(Event *event)
{
	
}
