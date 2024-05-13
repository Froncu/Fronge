#if not defined fro_FIXED_BEHAVIOUR_H
#define fro_FIXED_BEHAVIOUR_H

#include "Component.h"

namespace fro
{
	class FixedBehaviour : public Component
	{
	public:
		FixedBehaviour(GameObject& parentingGameObject);

		virtual ~FixedBehaviour() override = default;

		virtual void fixedUpdate(float fixedDeltaSeconds) = 0;

	private:
		FixedBehaviour(FixedBehaviour const&) = delete;
		FixedBehaviour(FixedBehaviour&&) noexcept = delete;

		FixedBehaviour& operator=(FixedBehaviour const&) = delete;
		FixedBehaviour& operator=(FixedBehaviour&&) noexcept = delete;
	};
}

#endif