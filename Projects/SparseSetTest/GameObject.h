#if not defined fro_GAME_OBJECT_H
#define fro_GAME_OBJECT_H

#include <cstddef>

namespace fro
{
	class GameObject final
	{
	public:
		GameObject() = default;
		GameObject(GameObject const&) = default;
		GameObject(GameObject&&) noexcept = default;

		~GameObject() = default;

		GameObject& operator=(GameObject const&) = default;
		GameObject& operator=(GameObject&&) noexcept = default;
		operator std::size_t& ();
		operator std::size_t const& () const;

	private:
		static std::size_t NEXT_ID;

		std::size_t m_ID{ NEXT_ID++ };
	};
}

#endif