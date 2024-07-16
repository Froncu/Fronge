#if not defined UNIQUE_EVENT_QUEUE_HPP
#define UNIQUE_EVENT_QUEUE_HPP

#include "Events/Systems/EventQueue.hpp"

namespace fro
{
	template<std::regular EventType>
	class UniqueEventQueue final
	{
	public:
		using ComparerType = std::function<bool(EventType const&, EventType const&)>;

		UniqueEventQueue(EventQueue<EventType>::ProcessorType eventProcessor, ComparerType comparer = std::equal_to<EventType>{})
			: mEventQueue{ std::move(eventProcessor) }
			, mComparer{ std::move(comparer) }
		{
		}

		template<typename... Arguments>
			requires std::constructible_from<EventType, Arguments...>
		void pushEvent(Arguments&&... arguments)
		{
			EventType event{ std::forward<Arguments>(arguments)... };

			if (isEventUnique(event))
				mEventQueue.pushEvent(std::move(event));
		}

		template<typename... Arguments>
			requires std::constructible_from<EventType, Arguments...>
		void overridePushIf(std::function<bool(EventType const&)> unaryPredicate, Arguments&&... arguments)
		{
			EventType event{ std::forward<Arguments>(arguments)... };

			if (isEventUnique(event))
				mEventQueue.overridePushIf(std::move(unaryPredicate), std::move(event));
		}

		void fetchNextEvent()
		{
			mEventQueue.fetchNextEvent();
		}

		void processEvent()
		{
			mEventQueue.processEvent();
		}

		void processAllEvents()
		{
			mEventQueue.processAllEvents();
		}

		std::deque<EventType> const& getQueue() const
		{
			return mEventQueue.getQueue();
		}

	private:
		bool isEventUnique(EventType const& event)
		{
			auto const& queue{ mEventQueue.getQueue() };
			return std::none_of(queue.begin(), queue.end(),
				[this, &event](EventType const& queuedEvent)
				{
					return mComparer(event, queuedEvent);
				});
		}

		EventQueue<EventType> mEventQueue;
		ComparerType mComparer;
	};
}

#endif