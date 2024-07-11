#if not defined RENDERER_HPP
#define RENDERER_HPP

#include "froch.hpp"

#include "Core.hpp"
#include "Events/Systems/EventListener.hpp"
#include "Maths/MathStructs.hpp"
#include "Reference/Reference.hpp"
#include "Utility.hpp"

namespace fro
{
	class Texture;
	class Window;

	class Renderer final : public Referencable
	{
		class Implementation;

	public:
		enum class ScalingMode
		{
			none,
			aspectRatio,
			fill
		};

		FRO_API Renderer(Reference<Window> const window,
			Vector2<int> const viewPortSize = {},
			ScalingMode const scalingMode = ScalingMode::fill);

		FRO_API ~Renderer();

		FRO_API FRO_NODISCARD Implementation& getImplementation() const;

		FRO_API void clear(float red, float green, float blue) const;
		FRO_API void present() const;
		FRO_API void renderTexture(Texture const& texture,
			Matrix3x3<double> const& transform = true,
			Rectangle<int> sourceRectangle = {}) const;

		FRO_NODISCARD Vector2<int> getViewportSize() const;

		FRO_API void setResolution(int const width, int const height);
		FRO_API void setScalingMode(ScalingMode const scalingMode);

	private:
		Renderer(Renderer const&) = delete;
		Renderer(Renderer&&) noexcept = delete;

		Renderer& operator=(Renderer const&) = delete;
		Renderer& operator=(Renderer&&) noexcept = delete;

		EventListener<> mOnWindowResizeEvent;

		Reference<Window> mWindow;
		Vector2<int> mViewportSize;
		ScalingMode mScalingMode;

		CustomUniquePointer<Implementation> mImplementation;
	};
}

#endif