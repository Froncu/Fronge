#if not defined fro_FYGAR_DEFAULT_STATE_H
#define fro_FYGAR_DEFAULT_STATE_H

#include "GameObject.h"
#include "GridMovement.h"
#include "State.h"

namespace fro
{
	class FygarDefaultState final : public State
	{
	public:
		FygarDefaultState(Reference<GameObject> const parentingGameObject);

		virtual ~FygarDefaultState() override = default;

		virtual Reference<State> fixedUpdate(float const) override;

	private:
		FygarDefaultState(FygarDefaultState const&) = delete;
		FygarDefaultState(FygarDefaultState&&) noexcept = delete;

		FygarDefaultState& operator=(FygarDefaultState const&) = delete;
		FygarDefaultState& operator=(FygarDefaultState&&) noexcept = delete;

		Reference<GridMovement> const m_GridMovement{ parentingGameObject.get().forceGetComponent<GridMovement>() };
	};
}

#endif