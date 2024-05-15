#if not defined fro_COMPONENTS_H
#define fro_COMPONENTS_H

#include <cstddef>
#include <string>

namespace fro
{
	struct Component
	{
		operator std::size_t& ();
		operator std::size_t const& () const;

		std::size_t m_ParentingGameObjectID;
	};

	struct TransformComponent final : public Component
	{
		float transform[4][4];
		bool isDirty;
	};

	struct PlaneComponent final : public Component
	{
		struct Vector3f final
		{
			float x;
			float y;
			float z;
		};

		Vector3f vertexBuffer[4];
		int indexBuffer[6];
	};

	struct TagComponent final : public Component
	{
		std::string name;
	};
}

#endif