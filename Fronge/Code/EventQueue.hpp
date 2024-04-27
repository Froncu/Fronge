#if not defined fro_EVENT_QUEUE_H
#define fro_EVENT_QUEUE_H

#include <functional>
#include <deque>
#include <algorithm>

namespace fro
{
	template<typename EventType, std::invocable<EventType&> EventProcesserType, bool unique = std::equality_comparable<EventType>>
	class EventQueue final
	{
	public:
		EventQueue(EventProcesserType eventProcesser)
			: m_EventProcesser{ eventProcesser }
		{
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

		void processEvents()
		{
			while (!m_dEvents.empty())
			{
				m_EventProcesser(m_dEvents.front());
				m_dEvents.pop_front();
			}
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
	};
}

#endif