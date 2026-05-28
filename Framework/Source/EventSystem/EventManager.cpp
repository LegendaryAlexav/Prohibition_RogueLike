#include "EventManager.h"
#include "Events.h"
#include "GameCore.h"

namespace fw
{
    EventManager::EventManager()
    {
    }

    EventManager::~EventManager()
    {
        // Clean up any unprocessed events on shutdown.
        while (!m_eventQueue.empty())
        {
            delete m_eventQueue.front();
            m_eventQueue.pop();
        }
    }

    void EventManager::addEvent(Event* pEvent)
    {
        assert(pEvent != nullptr);
        m_eventQueue.push(pEvent);
    }

    void EventManager::processEvents()
    {
        while (!m_eventQueue.empty())
        {
            Event* pEvent = m_eventQueue.front();
            assert(pEvent != nullptr);

            EventType type = pEvent->getType();

            // Send it to all listeners listening for this type of event.
            for (auto listener : m_listeners[type])
            {
                listener->func(pEvent);
            }

            delete pEvent;
            m_eventQueue.pop();
        }
    }
}