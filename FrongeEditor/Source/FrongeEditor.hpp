#if not defined FRO_FRONGE_EDITOR
#define FRO_FRONGE_EDITOR

#include "Fronge.hpp"

namespace fro
{
	class FrongeEditor final : public Application
	{
	public:
		FrongeEditor() = default;
		FrongeEditor(FrongeEditor const&) = default;
		FrongeEditor(FrongeEditor&&) noexcept = default;

		virtual ~FrongeEditor() override = default;

		FrongeEditor& operator=(FrongeEditor const&) = default;
		FrongeEditor& operator=(FrongeEditor&&) noexcept = default;

		virtual void run() override;
	};
}

#endif