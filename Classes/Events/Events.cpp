//
//  Events.cpp
//  SnakeClient
//
//  Created by cy on 13-12-11.
//
//

#include "Events.h"
#include <algorithm>

using namespace std;

Event::Event(int type)
{
    m_state = EventStateInit;
    m_type = type;
    m_tag = -1;
    m_errorCode = -1;
    m_tips = "";
}

Event::Event(int type, int tag)
{
    m_state = EventStateInit;
    m_type = type;
    m_tag = tag;
    m_errorCode = -1;
    m_tips = "";
}

Event::~ Event()
{
}

int Event::getType() const
{
    return m_type;
}

int Event::getState() const
{
    return m_state;
}

void Event::setState(int state)
{
    m_state = state;
}

int Event::getTag() const
{
    return m_tag;
}

void Event::setTag(int tag)
{
    m_tag = tag;
}

void Event::setErrorCode(int errorCode)
{
    m_errorCode = errorCode;
}

int Event::getErrorCode() const
{
    return m_errorCode;
}

void Event::setTips(const string& tips)
{
    m_tips = tips;
}

string& Event::getTips()
{
    return m_tips;
}

bool Event::hasFocus(EventObserver *observer) const
{
	list<EventObserver*>::const_iterator it = find(m_observers.begin(), m_observers.end(), observer);
	return it != m_observers.end();
}

bool Event::hasFocus() const
{
    return !m_observers.empty();
}

void Event::addFocus(EventObserver *observer)
{
    m_observers.push_back(observer);
}

void Event::removeFocus(EventObserver *observer)
{
    m_observers.remove(observer);
}

void Event::clearFocus()
{
    m_observers.clear();
}
