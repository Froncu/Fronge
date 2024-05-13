#if not defined fro_BEHAVIOUR_H
#define fro_BEHAVIOUR_H

#include "Component.h"

namespace fro
{
	class Behaviour : public Component
	{
	public:
		virtual ~Behaviour() override = default;

		virtual void update(float deltaSeconds) = 0;

	protected:
		Behaviour(GameObject& parentingGameObject);

	private:
		Behaviour(Behaviour const&) = delete;
		Behaviour(Behaviour&&) noexcept = delete;

		Behaviour& operator=(Behaviour const&) = delete;
		Behaviour& operator=(Behaviour&&) noexcept = delete;
	};
}

#endif