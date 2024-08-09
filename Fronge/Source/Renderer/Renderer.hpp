#if not defined RENDERER_HPP
#define RENDERER_HPP

#include "froch.hpp"

#include "Core.hpp"
#include "Events/Systems/EventListener.hpp"
#include "Maths/Structs/Matrix3x3.hpp"
#include "Maths/Structs/Chain.hpp"
#include "Maths/Structs/Circle.hpp"
#include "Maths/Structs/Polygon.hpp"
#include "Maths/Structs/Rectangle.hpp"
#include "Maths/Structs/Vector2.hpp"
#include "Reference/Reference.hpp"
#include "Idenitifer/IDGenerator.hpp"
#include "Utility/CustomUniquePointer.hpp"
#include "Window/Window.hpp"

namespace fro
{
	class Surface;
	class Texture;

	class Renderer final : public Referencable
	{
	public:
		class Implementation;

		enum class ScalingMode
		{
			NONE,
			ASPECT_RATIO,
			FILL
		};

		FRO_API Renderer(Window& window,
			Vector2<int> const resolution = {},
			ScalingMode const scalingMode = ScalingMode::ASPECT_RATIO);

		FRO_API ~Renderer();

		FRO_API FRO_NODISCARD Implementation& getImplementation() const;

		FRO_API void clear() const;
		FRO_API void present() const;

		FRO_API void renderTexture(Texture const& texture,
			Matrix3x3<double> const& transform = true,
			Rectangle<int> sourceRectangle = {}) const;

		FRO_API void renderCircle(Circle<double> const& circle, bool const fill,
			double const red, double const green, double const blue, double const alpha) const;

		FRO_API void renderPolygon(Polygon<double> const& polygon, bool const fill,
			double const red, double const green, double const blue, double const alpha) const;

		FRO_API void renderChain(Chain<double> const& chain, double const thickness,
			double const red, double const green, double const blue, double const alpha) const;

		FRO_API FRO_NODISCARD ID const& getID() const;
		FRO_API FRO_NODISCARD Vector2<int> getResolution() const;
		FRO_API FRO_NODISCARD Reference<Window> getWindow() const;

		FRO_API void setResolution(Vector2<int> const resolution);
		FRO_API void setScalingMode(ScalingMode const scalingMode);

	private:
		static IDGenerator sIDGenerator;

		Renderer(Renderer const&) = delete;
		Renderer(Renderer&&) noexcept = delete;

		Renderer& operator=(Renderer const&) = delete;
		Renderer& operator=(Renderer&&) noexcept = delete;

		void updateViewPort(Vector2<int> const windowSize,
			Vector2<int> const resolution,
			ScalingMode const scalingMode) const;

		ID mID{ sIDGenerator.get() };
		Reference<Window> mWindow;

		EventListener<Vector2<int> const> mOnWindowResizeEvent
		{
			[this](Vector2<int> const& size)
			{
				updateViewPort(size, mResolution, mScalingMode);
				return true;
			}, mWindow->mResizeEvent
		};

		Vector2<int> mResolution;
		ScalingMode mScalingMode;
		CustomUniquePointer<Implementation> mImplementation;
	};
}

#endif