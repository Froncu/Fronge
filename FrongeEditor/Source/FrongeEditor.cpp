#include "FrongeEditor.hpp"

namespace fro
{
	std::unique_ptr<Application> createApplication()
	{
		return std::make_unique<FrongeEditor>();
	}

	void FrongeEditor::run()
	{
		while (true);
	}
}