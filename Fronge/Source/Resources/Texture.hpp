#if not defined TEXTURE_HPP
#define TEXTURE_HPP

#include "froch.hpp"

#include "Core.hpp"
#include "Reference/Reference.hpp"
#include "Resources/Font.hpp"
#include "Idenitifer/IDGenerator.hpp"

namespace fro
{
	class Renderer;

	class Texture final : public Referencable
	{
		class Implementation;

	public:
		FRO_API Texture(Reference<Renderer> const renderer, std::string_view const imagePath);
		FRO_API Texture(Reference<Renderer> const renderer, Font const& font, std::string_view const text);
		FRO_API Texture(Texture&& other) noexcept;

		FRO_API ~Texture();

		FRO_API Texture& operator=(Texture&& other) noexcept;

		FRO_API FRO_NODISCARD Implementation& getImplementation() const;

		FRO_API FRO_NODISCARD std::size_t getID() const;
		FRO_API FRO_NODISCARD int getWidth() const;
		FRO_API FRO_NODISCARD int getHeight() const;

	private:
		static IDGenerator sIDGenerator;

		Texture(Texture const&) = delete;

		Texture& operator=(Texture const&) = delete;

		ID mID{ sIDGenerator.get() };
		Reference<Renderer> mRenderer;
		std::unique_ptr<Implementation> mImplementation;
		int mWidth;
		int mHeight;
	};
}

#endif