#if not defined RENDERER_HPP
#define RENDERER_HPP

#include "froch.hpp"

#include "Core.hpp"
#include "Events/Systems/EventListener.hpp"
#include "Maths/MathStructs.hpp"
#include "Reference/Reference.hpp"
#include "Resources/Texture.hpp"
#include "Utility.hpp"

namespace fro
{
	class Window;

	class Renderer final : public Referencable
	{
		friend class Texture;
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

		FRO_API void clear(float const red, float const green, float const blue) const;
		FRO_API void present() const;
		FRO_API void renderTexture(Texture const& texture,
			Matrix3x3<double> const& transform,
			Rectangle<int> sourceRectangle = {}) const;

		FRO_NODISCARD Vector2<int> getViewportSize() const;

		void setResolution(int const width, int const height);
		void setScalingMode(ScalingMode const scalingMode);

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