#if not defined fro_COMPONENTS_H
#define fro_COMPONENTS_H

#include <cstddef>
#include <string>

namespace fro
{
	struct Component
	{
		operator std::size_t& ()
		{
			return m_ParentingGameObjectID;
		};
		operator std::size_t const& () const
		{
			return m_ParentingGameObjectID;
		}

		std::size_t m_ParentingGameObjectID;
	};

	struct TagComponent final : public Component
	{
		std::string name;
	};

	struct TransfromComponent final : public Component
	{
		float rotation;
		int x;
		int y;
	};
}

#endif