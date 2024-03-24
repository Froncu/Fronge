#pragma once

#include "Event.hpp"

#include <unordered_map>
#include <string>
#include <any>

namespace fro
{
	class EventDispatcher
	{
	public:
		EventDispatcher() = default;
		EventDispatcher(const EventDispatcher&) = default;
		EventDispatcher(EventDispatcher&&) noexcept = default;

		~EventDispatcher() = default;

		EventDispatcher& operator=(const EventDispatcher&) = default;
		EventDispatcher& operator=(EventDispatcher&&) noexcept = default;

	public:
		template<typename ...Payload>
		auto& getEvent(const std::string& eventName)
		{
			return std::any_cast<Event<Payload&&...>&>(m_umEvents[eventName]);
		};

	protected:
		template<typename ...Payload>
		void createEvent(const std::string& eventName)
		{
			m_umEvents.emplace(eventName, Event<Payload...>{});
		}

	private:
		std::unordered_map<std::string, std::any> m_umEvents{};
	};
}