#pragma once

#include "Events.h"

namespace fw
{
    class Event;
    class GameCore;

    using EventListenerFunction = std::function<void(Event*)>;

    struct EventListener
    {
        EventListener(EventListenerFunction f) : func(f) {}
        EventListenerFunction func;
    };

    class EventManager
    {
    public:
        EventManager();
        ~EventManager();

        void addEvent(Event* pEvent);
        void processEvents();

        template<class Type>
        void registerListener(EventListener* pListener)
        {
            EventType type = Type::getStaticEventType();
            m_listeners[type].push_back(pListener);
        }

        template<class Type>
        void unregisterListener(EventListener* pListener)
        {
            EventType type = Type::getStaticEventType();
            auto it = std::find(m_listeners[type].begin(), m_listeners[type].end(), pListener);
            if (it != m_listeners[type].end())
                m_listeners[type].erase(it);
        }

    protected:
        std::queue<Event*> m_eventQueue;




        std::map<EventType, std::vector<EventListener*>> m_listeners;
    };

} // namespace fw
