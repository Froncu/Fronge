#if not defined EVENT_QUEUE_HPP
#define EVENT_QUEUE_HPP

#include "froch.hpp"

#include "Core.hpp"

namespace fro
{
	template<std::semiregular EventType>
	class EventQueue final
	{
	public:
		using ProcessorType = std::function<void(EventType&&)>;

		EventQueue(ProcessorType eventProcessor)
			: mEventProcessor{ std::move(eventProcessor) }
		{
			if (mEventProcessor == nullptr)
				FRO_EXCEPTION("the event processor cannot be a nullptr!");
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
			requires std::equality_comparable<EventType> and std::constructible_from<EventType, Arguments...>
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
		EventType mNextEvent{};
		ProcessorType mEventProcessor;
		std::deque<EventType> mQueue{};
		bool mIsNextEventFetched{};
	};
}

#endif