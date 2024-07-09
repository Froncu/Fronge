#if not defined TEXTURE_HPP
#define TEXTURE_HPP

#include "froch.hpp"

#include "Core.hpp"
#include "Reference/Reference.hpp"

namespace fro
{
	class Renderer;

	class Texture final : public Referencable
	{
		class Implementation;

	public:
		struct Descriptor final
		{
			Reference<Renderer> renderer;
			std::string filePath;
		};

		FRO_API Texture(Descriptor descriptor);
		FRO_API Texture(Texture const& other);
		FRO_API Texture(Texture&& other) noexcept;

		FRO_API ~Texture();

		FRO_API Texture& operator=(Texture const& other);
		FRO_API Texture& operator=(Texture&& other) noexcept;

		FRO_API FRO_NODISCARD Implementation& getImplementation() const;

		FRO_API FRO_NODISCARD std::string_view getFilePath() const;
		FRO_API FRO_NODISCARD int getWidth() const;
		FRO_API FRO_NODISCARD int getHeight() const;

	private:
		Descriptor mDescriptor;

		std::unique_ptr<Implementation> mImplementation;
		int mWidth;
		int mHeight;
	};
}

#endif