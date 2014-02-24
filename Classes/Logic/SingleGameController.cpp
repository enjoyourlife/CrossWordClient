//
//  SingleGameController.cpp
//  SnakeClient
//
//  Created by cy on 13-12-11.
//
//

#include "SingleGameController.h"
#include "../Events/EventManager.h"
#include "../Events/GameEvents.h"


SingleGameController::SingleGameController()
{
    
}

SingleGameController::~SingleGameController()
{
}

void SingleGameController::startEvent(int event, void* data)
{
    switch (event)
    {
        case EventTypeLogin:
            break;
            
        default:
            break;
	}
}

void SingleGameController::startEvent(Event *event)
{
    int type = event->getType();
    switch (type)
    {
        case EventTypeEnterGame:
        {
            EnterGameEvent* e = (EnterGameEvent*)event;
                        
            EventManager::sharedEventManager()->notifyEventSucceeded(event);
            
            break;
        }
            
        
            
        default:
            break;
            
    }
}

void SingleGameController::cancelEvent(Event *event)
{
	
}

int SingleGameController::rollLogic()
{
    int diceNumber = 1;
    
    srand(time(0));
    diceNumber = rand() % 6 + 1;
    
    return diceNumber;
}
