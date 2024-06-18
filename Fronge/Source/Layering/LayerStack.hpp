#if not defined LAYER_STACK_HPP
#define LAYER_STACK_HPP

#include "froch.hpp"

#include "Layer.hpp"

namespace fro
{
	class LayerStack final
	{
		using Layers = std::vector<std::unique_ptr<Layer>>;

	public:
		LayerStack() = default;
		LayerStack(LayerStack const&) = default;
		LayerStack(LayerStack&&) noexcept = default;

		~LayerStack() = default;

		LayerStack& operator=(LayerStack const&) = default;
		LayerStack& operator=(LayerStack&&) noexcept = default;

		FRO_API void pushLayer(std::unique_ptr<Layer>&& layer);
		FRO_API void pushOverlay(std::unique_ptr<Layer>&& overlay);

		// NOTE: no FRO_NODISCARD; the user might want to actually delete the layer

		FRO_API std::unique_ptr<Layer> popLayer();
		FRO_API std::unique_ptr<Layer> popOverlay();

		FRO_API Layers::iterator begin();
		FRO_API Layers::const_iterator begin() const;
		FRO_API Layers::iterator end();
		FRO_API Layers::const_iterator end() const;

		EventListener<Event&> mOnEvent
		{
			[this](auto&& event)
			{
				auto layer{ mLayers.end() };
				while (layer not_eq mLayers.begin())
				{
					--layer;
					if (layer->get()->onEvent(event))
						return true;
				}

				return false;
			}
		};

	private:
		Layers mLayers{};
		std::size_t mOverlayCount{};
	};
}

#endif