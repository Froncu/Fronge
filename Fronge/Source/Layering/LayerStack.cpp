#include "froch.hpp"

#include "LayerStack.hpp"

namespace fro
{
	void LayerStack::pushLayer(std::unique_ptr<Layer>&& layer)
	{
		auto pushedLayer{ mLayers.emplace(mLayers.end() - mOverlayCount, std::move(layer)) };

		pushedLayer->get()->onAttach();
	}

	void LayerStack::pushOverlay(std::unique_ptr<Layer>&& overlay)
	{
		auto const& pushedOverlay{ mLayers.emplace_back(std::move(overlay)) };
		++mOverlayCount;

		pushedOverlay->onAttach();
	}

	std::unique_ptr<Layer> LayerStack::popLayer()
	{
		std::size_t const index{ mLayers.size() - mOverlayCount - 1 };
		FRO_ASSERT(index >= 0, "no layer to pop!");

		auto layer{ std::move(mLayers[index]) };
		mLayers.erase(mLayers.begin() + index);

		layer->onDetach();
		return layer;
	}

	std::unique_ptr<Layer> LayerStack::popOverlay()
	{
		FRO_ASSERT(mOverlayCount, "no overlay to pop!");

		auto layer{ std::move(mLayers.back()) };
		mLayers.pop_back();
		--mOverlayCount;

		layer->onDetach();
		return layer;
	}

	LayerStack::Layers::iterator LayerStack::begin()
	{
		return mLayers.begin();
	}

	LayerStack::Layers::const_iterator LayerStack::begin() const
	{
		return mLayers.begin();
	}

	LayerStack::Layers::iterator LayerStack::end()
	{
		return mLayers.end();
	}

	LayerStack::Layers::const_iterator LayerStack::end() const
	{
		return mLayers.end();
	}
}