#if not defined fro_EVENT_QUEUE_H
#define fro_EVENT_QUEUE_H

#include <algorithm>
#include <cassert>
#include <concepts>
#include <deque>
#include <functional>

namespace fro
{
	template<std::copyable EventType, bool unique = std::equality_comparable<EventType>>
	class EventQueue final
	{
		static_assert(not unique or std::equality_comparable<EventType>,
			"queue declared unique, but event is not equality comparable");

	public:
		EventQueue(std::function<void(EventType)> eventProcessor)
			: m_EventProcessor{ std::move(eventProcessor) }
		{
			assert(m_EventProcessor not_eq nullptr);
		}

		EventQueue(EventQueue const&) = default;
		EventQueue(EventQueue&&) noexcept = default;

		~EventQueue() = default;

		EventQueue& operator=(EventQueue const&) = default;
		EventQueue& operator=(EventQueue&&) noexcept = default;

		void pushEvent(EventType event)
		{
			if constexpr (unique)
				if (not isEventUnqiue(event))
					return;

			m_dQueue.emplace_back(std::move(event));
		}

		void overridePushIf(EventType event, std::function<bool(EventType const&)> unaryPredicate)
		{
			if constexpr (unique)
				if (not isEventUnqiue(event))
					return;

			auto const iNewEnd{ std::remove_if(m_dQueue.begin(), m_dQueue.end(), unaryPredicate) };

			if (iNewEnd not_eq m_dQueue.end())
				m_dQueue.erase(iNewEnd, m_dQueue.end());

			m_dQueue.emplace_back(std::move(event));
		}

		void fetchNextEvent()
		{
			m_NextEvent = std::move(m_dQueue.front());
			m_dQueue.pop_front();

			m_IsNextEventFetched = true;
		}

		void processEvent()
		{
			if (not m_IsNextEventFetched)
				fetchNextEvent();

			m_EventProcessor(std::move(m_NextEvent));
			m_IsNextEventFetched = false;
		}

		void processAllEvents()
		{
			while (not m_dQueue.empty())
				processEvent();
		}

		std::deque<EventType> const& getQueue() const
		{
			return m_dQueue;
		}

	private:
		bool isEventUnqiue(EventType const& event)
		{
			return std::none_of(m_dQueue.begin(), m_dQueue.end(),
				[&event](EventType const& queuedEvent)
				{
					return event == queuedEvent;
				});
		}

		std::function<void(EventType)> m_EventProcessor{};
		std::deque<EventType> m_dQueue{};

		EventType m_NextEvent{};
		bool m_IsNextEventFetched{};
	};
}

#endif