#if not defined fro_STATE_H
#define fro_STATE_H

#include <memory>

namespace fro
{
	class State
	{
	public:
		State() = default;

		virtual ~State() = default;
		
		virtual std::unique_ptr<State> update() = 0;
		virtual void enter(std::unique_ptr<State> const& pOldState);
		virtual void exit(std::unique_ptr<State> const& pNewState);

	private:
		State(State const&) = delete;
		State(State&&) noexcept = delete;

		State& operator=(State const&) = delete;
		State& operator=(State&&) noexcept = delete;
	};
}

#endif