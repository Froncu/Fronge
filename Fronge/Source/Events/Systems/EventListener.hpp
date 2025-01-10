#ifndef EVENT_LISTENER_HPP
#define EVENT_LISTENER_HPP

#include "froch.hpp"

#include "EventDispatcher.hpp"

namespace fro
{
	template<Dispatchable...>
	class EventDispatcher;

	// NOTE: be careful with this in case the onNotify is a callable which holds 
	// references and/or pointers (use fro::Reference if you encounter issues)
	template<Dispatchable... Payload>
	class EventListener final
	{
		friend EventDispatcher;
		using CallbackType = std::function<bool(Payload&...)>;

	public:
		template<std::same_as<EventDispatcher<Payload...>>... EventDispatcherTypes>
		EventListener(CallbackType onNotify, EventDispatcherTypes&... eventDispatchers)
			: mOnNotify{ std::move(onNotify) }
		{
			if (mOnNotify == nullptr)
				FRO_EXCEPTION("the onNotify callback cannot be a nullptr!");

			(eventDispatchers.addListener(*this), ...);
		}

		EventListener(EventListener const& other)
			: mOnNotify{ other.mOnNotify }
			, mDispatchers{ other.mDispatchers }
		{
			for (auto const dispatcher : mDispatchers)
				dispatcher->mListeners.insert(this);
		}

		EventListener(EventListener&& other) noexcept
			: mOnNotify{ std::move(other.mOnNotify) }
			, mDispatchers{ std::move(other.mDispatchers) }
		{
			for (auto const dispatcher : mDispatchers)
			{
				dispatcher->mListeners.erase(&other);
				dispatcher->mListeners.insert(this);
			}
		}

		~EventListener()
		{
			for (auto const dispatcher : mDispatchers)
				dispatcher->mListeners.erase(this);
		}

		EventListener& operator=(EventListener const& other)
		{
			if (this == &other)
				return *this;

			mOnNotify = other.mOnNotify;

			for (auto const dispatcher : mDispatchers)
				dispatcher->mListeners.erase(this);

			mDispatchers = other.mDispatchers;

			for (auto const dispatcher : mDispatchers)
				dispatcher->mListeners.insert(this);

			return *this;
		}

		EventListener& operator=(EventListener&& other) noexcept
		{
			if (this == &other)
				return *this;

			mOnNotify = std::move(other.mOnNotify);

			for (auto const dispatcher : mDispatchers)
				dispatcher->mListeners.erase(this);

			mDispatchers = std::move(other.mDispatchers);

			for (auto const dispatcher : mDispatchers)
			{
				dispatcher->mListeners.erase(&other);
				dispatcher->mListeners.insert(this);
			}

			return *this;
		}

	private:
		CallbackType mOnNotify;
		std::unordered_set<EventDispatcher<Payload...>*> mDispatchers{};
	};
}

#endif