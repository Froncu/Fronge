#if not defined fro_FIXED_BEHAVIOUR_H
#define fro_FIXED_BEHAVIOUR_H

#include "Component.h"

namespace fro
{
	class FixedBehaviour : public Component
	{
	public:
		virtual ~FixedBehaviour() override = default;

		virtual void fixedUpdate(float fixedDeltaSeconds) = 0;

	protected:
		FixedBehaviour(Reference<GameObject> const parentingGameObject);

	private:
		FixedBehaviour(FixedBehaviour const&) = delete;
		FixedBehaviour(FixedBehaviour&&) noexcept = delete;

		FixedBehaviour& operator=(FixedBehaviour const&) = delete;
		FixedBehaviour& operator=(FixedBehaviour&&) noexcept = delete;
	};
}

#endif