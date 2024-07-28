#if not defined RENDERER_HPP
#define RENDERER_HPP

#include "froch.hpp"

#include "Core.hpp"
#include "Events/Systems/EventListener.hpp"
#include "Maths/Structs/Matrix3x3.hpp"
#include "Maths/Structs/Rectangle.hpp"
#include "Reference/Reference.hpp"
#include "Idenitifer/IDGenerator.hpp"
#include "Utility/CustomUniquePointer.hpp"

namespace fro
{
	class Surface;
	class Texture;
	class Window;

	class Renderer final : public Referencable
	{
		class Implementation;

	public:
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

		EventListener<Vector2<int> const> mOnWindowResizeEvent;
		ID mID{ sIDGenerator.get() };
		Reference<Window> mWindow;
		Vector2<int> mResolution;
		ScalingMode mScalingMode;
		CustomUniquePointer<Implementation> mImplementation;
	};
}

#endif