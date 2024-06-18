#if not defined LAYER_HPP
#define LAYER_HPP

#include "Events/Events.hpp"
#include "Events/Polymorphic/Event.hpp"

namespace fro
{
	class Layer
	{
	public:
		virtual ~Layer() = default;

		FRO_API virtual void onAttach();
		FRO_API virtual void onDetach();
		FRO_API virtual void update();
		FRO_API virtual bool onEvent(Event& event);

	protected:
		Layer() = default;
		Layer(Layer const&) = default;
		Layer(Layer&&) noexcept = default;

		Layer& operator=(Layer const&) = default;
		Layer& operator=(Layer&&) noexcept = default;
	};
}

#endif