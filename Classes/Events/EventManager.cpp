//
//  EventManager.cpp
//  SnakeClient
//
//  Created by cy on 13-12-11.
//
//

#include "EventManager.h"
#include <algorithm>
#include "../Data/DataManager.h"
#include "../Logic/OnlineGameController.h"
#include "../Logic/SingleGameController.h"

using namespace std;
using namespace cocos2d;

static EventManager* s_eventManager = NULL;

EventManager::EventManager() :
m_onlineController(NULL),
m_singleController(NULL)
{
	CCDirector::sharedDirector()->getScheduler()->scheduleUpdateForTarget(this, 0, false);
}

EventManager::~ EventManager()
{
	CCDirector::sharedDirector()->getScheduler()->unscheduleUpdateForTarget(this);
}

EventManager* EventManager::sharedEventManager()
{
	if (s_eventManager == NULL)
	{
		s_eventManager = new EventManager();
		s_eventManager->init();
	}
	return s_eventManager;
}

bool EventManager::init()
{
	m_onlineController = new OnlineGameController;
	m_singleController = new SingleGameController;
    
	return m_onlineController && m_singleController;
}

GameController* EventManager::getGameController()
{
	GameType gameType = DataManager::sharedDataManager()->getGameType();
	if (gameType == GameTypeSingle)
	{
		return m_singleController;
	}
	else 
	{
		return m_onlineController;
	}
}

void EventManager::process()
{
	Event* e = m_events.front();
	int state = e->getState();
    
	if (state == EventStateInit)
	{
		e->setState(EventStateStarted);
        
		getGameController()->startEvent(e);
		for (list<EventObserver*>::iterator it = m_observers.begin(); it != m_observers.end(); ++it)
		{
			(*it)->onEventStarted(e);
		}
	}
	else if (state == EventStateFailed)
	{
		e->setState(EventStateNotified);
        
		for (list<EventObserver*>::iterator it = m_observers.begin(); it != m_observers.end(); ++it)
		{
			(*it)->onEventFailed(e);
		}
	}
	else if (state == EventStateDone)
	{
		e->setState(EventStateNotified);
        
        //不能在m_observers循环的同时 改变其成员(比如切换界面引起成员销毁) 否则引起异常
        for (list<EventObserver*>::iterator it = m_observers.begin(); it != m_observers.end(); ++it)
		{
            (*it)->onEventSucceeded(e);
		}
	}
}

void EventManager::update(float t)
{
	if (m_events.size() > 0)
	{
		bool isDone = false;
		while (!isDone)
		{
			process();
            
			isDone = true;
			Event* e = m_events.front();
			if (e->getState() == EventStateNotified && !e->hasFocus())
			{
				delete e;
				m_events.remove(e);
				printEvents();
                
				if (m_events.size() > 0)
				{
					isDone = false;
				}
			}
		}
	}
}

void EventManager::doEvent(int type, void* data)
{
	getGameController()->startEvent(type, data);
}

void EventManager::notifyEventSucceeded(int event, void* data)
{
	for (list<EventObserver*>::iterator it = m_observers.begin(); it != m_observers.end(); ++it)
	{
		(*it)->onEventSucceeded(event, data);
	}
}

void EventManager::notifyEventFailed(int event, void* data)
{
	for (list<EventObserver*>::iterator it = m_observers.begin(); it != m_observers.end(); ++it)
	{
		(*it)->onEventFailed(event, data);
	}
}

void EventManager::addEvent(Event* event)
{
	m_events.push_back(event);
	printEvents();
    
	if (m_events.size() == 1)
	{
		process();
	}
}

void EventManager::cancelEvent(Event* event)
{
	for (list<EventObserver*>::iterator it = m_observers.begin(); it != m_observers.end(); ++it)
	{
		(*it)->onEventCancelled(event);
	}
    
	delete event;
	m_events.remove(event);
}

void EventManager::notifyEventSucceeded(Event* event)
{
	event->setState(EventStateDone);
    
	list<Event*>::iterator it = find(m_events.begin(), m_events.end(), event);
	if (it == m_events.end())
	{
		m_events.push_back(event);
		printEvents();
	}
    
	if (m_events.size() == 1)
	{
		process();
	}
}

void EventManager::notifyEventFailed(Event *event)
{
	event->setState(EventStateFailed);
    
	list<Event*>::iterator it = find(m_events.begin(), m_events.end(), event);
	if (it == m_events.end())
	{
		m_events.push_back(event);
		printEvents();
	}
    
	if (m_events.size() == 1)
	{
		process();
	}
}

Event* EventManager::getEvent(int type, int tag)
{
	for (list<Event*>::iterator it = m_events.begin(); it != m_events.end(); ++it)
	{
		Event* e = *it;
		if (e->getType() == type && e->getTag() == tag)
		{
			return e;
		}
	}
	return NULL;
}

Event* EventManager::getActiveEvent() const
{
	if (m_events.size() > 0)
	{
		return m_events.front();
	}
	return NULL;
}

void EventManager::removeAllEvents()
{
	for (list<Event*>::iterator it = m_events.begin(); it != m_events.end(); ++it)
	{
		delete *it;
	}
	m_events.clear();
}

void EventManager::addFocusOnEvent(EventObserver *observer, Event *event)
{
	assert(observer != NULL && event != NULL);
	event->addFocus(observer);
}

void EventManager::removeFocusOnEvent(EventObserver *observer, Event *event)
{
    //	assert(observer != NULL && event != NULL);
    if (observer != NULL && event != NULL)
    {
        event->removeFocus(observer);
    }
}

void EventManager::removeFocusOnEvent(EventObserver *observer, int type, int tag)
{
	Event* event = getEvent(type, tag);
	removeFocusOnEvent(observer, event);
}

void EventManager::addObserver(EventObserver *observer)
{
	removeObserver(observer);
	m_observers.push_back(observer);
}

void EventManager::removeObserver(EventObserver *observer)
{
	m_observers.remove(observer);
    
	for (list<Event*>::iterator it = m_events.begin(); it != m_events.end(); ++it)
	{
		if ((*it)->hasFocus(observer))
		{
			removeFocusOnEvent(observer, (*it));
		}
	}
}

int EventManager::getEventNumber()
{
    return m_events.size();
}

const char* EventManager::getEventName(int type) const
{
	switch (type)
	{
        case EventTypeLogin:
            return "EventTypeLogin";
            
        case EventTypeLoginEx:
            return "EventTypeLoginEx";
            
        case EventTypeEnterHall:
            return "EventTypeEnterHall";
            
        case EventTypeEnterSingleGame:
            return "EventTypeEnterSingleGame";
            
        case EventTypeEnterCompetitiveGame:
            return "EventTypeEnterCompetitiveGame";
            
        case EventTypeEnterCooperationGame:
            return "EventTypeEnterCooperationGame";
            
        case EventTypeEnterSingleSubGame:
            return "EventTypeEnterSingleSubGame";
            
        case EventTypeSitDown:
            return "EventTypeSitDown";
            
        case EventTypeSitUp:
            return "EventTypeSitUp";
            
        case EventTypeGameStart:
            return "EventTypeGameStart";
            
        case EventTypeGameStartEx:
            return "EventTypeGameStartEx";
            
        case EventTypeTouchGrid:
            return "EventTypeTouchGrid";
            
        case EventTypeChooseAnswer:
            return "EventTypeChooseAnswer";
            
        case EventTypeFixAnswer:
            return "EventTypeFixAnswer";
            
        case EventTypeReward:
            return "EventTypeReward";
            
        default:
            return "UnknownEvent";
	}
}

void EventManager::printEvents()
{
	CCLog("\n\n...................................");
	for (list<Event*>::iterator it = m_events.begin(); it != m_events.end(); ++it)
	{
		CCLog("event = %s, type = %d, state = %d, tag = %d, focus = %d", getEventName((*it)->getType()), (*it)->getType(), (*it)->getState(), (*it)->getTag(), (*it)->hasFocus());
	}
	CCLog("...................................\n\n");
}