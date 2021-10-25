#pragma once

#include "gepch.h"

#include "GameEngine/Events/Event.h"
#include "GameEngine/Events/ApplicationEvent.h"
#include "GameEngine/Events/KeyEvent.h"
#include "GameEngine/Events/MouseEvent.h"

namespace GameEngine {

	/**
	* For possible future use.
	*
	* Provides an API by which one can "subscribe" to events, without needing
	* to check every event for its type and then dispatch accordingly.
	*/
	class EventManager {
	private:
		class EventCallbackParent {
		public:
			virtual bool operator()(const Event& e) {
				return false;
			}
		};
		template<typename T>
		class EventCallback : public EventCallbackParent {
		public:
			EventCallback(std::function<bool(const T&)> callback) : m_callback(callback) {}
			bool operator()(const Event& e) override {
				return m_callback(*(T*)&e);
			}

		private:
			std::function<bool(const T&)> m_callback;
		};
	public:
		//template <typename T>
		//using EventCallback = std::function<bool(const T&)>;

		template <typename T>
		void Subscribe(std::function<bool(const T&)> callback)
		{
			int idx = (int)(T::GetStaticEventType());
			m_eventCallbacks[idx].emplace(m_eventCallbacks[idx].begin(), new EventCallback<T>(callback));
		}

		bool OnEvent(const Event& e)
		{
			for (EventCallbackParent* callback : m_eventCallbacks[(int)e.GetEventType()])
			{
				if ((*callback)(e)) {
					return true;
				}
			}

			return false;
		}

	private:

		std::vector<EventCallbackParent*> m_eventCallbacks[(int)EventType::NumEventTypes];
	};

}
