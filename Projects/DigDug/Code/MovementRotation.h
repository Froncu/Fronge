#if not defined fro_MOVEMENT_ROTATION_H
#define fro_MOVEMENT_ROTATION_H

#include "Behaviour.h"

#include <vec2.hpp>

namespace fro
{
	class MovementRotation : public Component
	{
	public:
		MovementRotation(Reference<GameObject> const parentingGameObject);

		~MovementRotation() = default;

		void onCorrectedMoveDirectionChanged(glm::vec2 const previous, glm::vec2 const current);

	private:
		MovementRotation(MovementRotation const&) = delete;
		MovementRotation(MovementRotation&&) noexcept = delete;

		MovementRotation& operator=(MovementRotation const&) = delete;
		MovementRotation& operator=(MovementRotation&&) noexcept = delete;
	};
}

#endif