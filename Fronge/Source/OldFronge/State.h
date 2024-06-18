#if not defined fro_STATE_H
#define fro_STATE_H

#include "Component.h"

#include <memory>

namespace fro
{
	class State : public Component
	{
	public:
		State(Reference<GameObject> const parentingGameObject);

		virtual ~State() = default;
		
		virtual Reference<State> fixedUpdate(float const);
		virtual Reference<State> update(float const);
		virtual Reference<State> lateUpdate(float const);

		virtual void enter(Reference<State> const oldState);
		virtual void exit(Reference<State> const newState);

	private:
		State(State const&) = delete;
		State(State&&) noexcept = delete;

		State& operator=(State const&) = delete;
		State& operator=(State&&) noexcept = delete;
	};
}

#endif