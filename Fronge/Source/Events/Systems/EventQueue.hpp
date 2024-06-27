#if not defined EVENT_QUEUE_HPP
#define EVENT_QUEUE_HPP

#include "froch.hpp"

#include "Core.hpp"

namespace fro
{
	template<std::movable EventType, std::invocable<EventType&&> ProcessorType>
	class EventQueue final
	{
	public:
		EventQueue(ProcessorType eventProcessor)
			: mEventProcessor{ std::move(eventProcessor) }
		{
			FRO_ASSERT(mEventProcessor not_eq nullptr,
				"the event processor cannot be empty!");
		}

		EventQueue(EventQueue const&) = default;
		EventQueue(EventQueue&&) noexcept = default;

		~EventQueue() = default;

		EventQueue& operator=(EventQueue const&) = default;
		EventQueue& operator=(EventQueue&&) noexcept = default;

		template<typename... Arguments>
			requires std::constructible_from<EventType, Arguments...>
		void pushEvent(Arguments&&... arguments)
		{
			mQueue.emplace_back(std::forward<Arguments>(arguments)...);
		}

		template<typename... Arguments>
			requires std::constructible_from<EventType, Arguments...>
		void overridePushIf(std::function<bool(EventType const&)> unaryPredicate, Arguments&&... arguments)
		{
			auto const newEnd{ std::remove_if(mQueue.begin(), mQueue.end(), unaryPredicate) };
			if (newEnd not_eq mQueue.end())
				mQueue.erase(newEnd, mQueue.end());

			mQueue.emplace_back(std::forward<Arguments>(arguments)...);
		}

		void fetchNextEvent()
		{
			if (mIsNextEventFetched or mQueue.empty())
				return;

			mNextEvent = std::move(mQueue.front());
			mQueue.pop_front();

			mIsNextEventFetched = true;
		}

		void processEvent()
		{
			if (not mIsNextEventFetched)
				fetchNextEvent();

			mEventProcessor(std::move(mNextEvent));
			mIsNextEventFetched = false;
		}

		void processAllEvents()
		{
			while (not mQueue.empty())
				processEvent();
		}

		std::deque<EventType> const& getQueue() const
		{
			return mQueue;
		}

	private:
		ProcessorType mEventProcessor;
		std::deque<EventType> mQueue{};

		EventType mNextEvent{};
		bool mIsNextEventFetched{};
	};
}

#endif