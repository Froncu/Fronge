#if not defined fro_EVENT_QUEUE_H
#define fro_EVENT_QUEUE_H

#include <algorithm>
#include <cassert>
#include <concepts>
#include <deque>
#include <functional>

namespace fro
{
	template<std::copyable EventType, std::invocable<EventType> EventProcesserType, bool unique = std::equality_comparable<EventType>>
	class EventQueue final
	{
	public:
		EventQueue(EventProcesserType const& eventProcesser = {})
			: m_EventProcesser{ eventProcesser }
		{
			if constexpr (std::_Is_specialization_v<EventProcesserType, std::function>)
				assert(m_EventProcesser not_eq nullptr);
		}

		EventQueue(EventQueue const&) = default;
		EventQueue(EventQueue&&) noexcept = default;

		~EventQueue() = default;

		EventQueue& operator=(EventQueue const&) = default;
		EventQueue& operator=(EventQueue&&) noexcept = default;

		void pushEvent(EventType&& event)
		{
			if constexpr (unique)
				if (!isEventUnqiue(event))
					return;

			m_dEvents.emplace_back(std::move(event));
		}

		void pushEvent(EventType const& event)
		{
			if constexpr (unique)
				if (!isEventUnqiue(event))
					return;

			m_dEvents.push_back(event);
		}

		void fetchNextEvent()
		{
			m_NextEvent = std::move(m_dEvents.front());
			m_dEvents.pop_front();

			m_IsNextEventFetched = true;
		}

		void processEvent()
		{
			if (not m_IsNextEventFetched)
				fetchNextEvent();

			m_EventProcesser(std::move(m_NextEvent));
			m_IsNextEventFetched = false;
		}

		void processAllEvents()
		{
			while (!m_dEvents.empty())
				processEvent();
		}

		std::deque<EventType> const& getQueue() const
		{
			return m_dEvents;
		}

	private:
		bool isEventUnqiue(EventType const& event)
		{
			return std::none_of(m_dEvents.begin(), m_dEvents.end(),
				[&event](EventType const& queuedEvent)
				{
					return event == queuedEvent;
				});
		}

		EventProcesserType m_EventProcesser{};
		std::deque<EventType> m_dEvents{};

		EventType m_NextEvent{};
		bool m_IsNextEventFetched{};
	};
}

#endif