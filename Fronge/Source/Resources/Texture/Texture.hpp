#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include "froch.hpp"

#include "Idenitifer/IDGenerator.hpp"
#include "Maths/Structs/Vector2.hpp"
#include "Reference/Reference.hpp"

namespace fro
{
	class Renderer;
	class Surface;

	class Texture final : public Referencable
	{
	public:
		class Implementation;

		FRO_API Texture(Renderer& renderer, Surface const& surface);
		FRO_API Texture(Texture&& other) noexcept;

		FRO_API ~Texture();

		FRO_API Texture& operator=(Texture&& other) noexcept;

		FRO_API FRO_NODISCARD Implementation& getImplementation() const;

		FRO_API FRO_NODISCARD ID const& getID() const;
		FRO_API FRO_NODISCARD Reference<Renderer> getRenderer() const;
		FRO_API FRO_NODISCARD Vector2<int> getSize() const;

	private:
		static IDGenerator sIDGenerator;
		
		Texture(Texture const&) = delete;

		Texture& operator=(Texture const&) noexcept = delete;

		ID mID{ sIDGenerator.get() };
		Reference<Renderer> mRenderer;
		Vector2<int> mSize;
		std::unique_ptr<Implementation> mImplementation;
	};
}

#endif