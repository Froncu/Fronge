#pragma once

#include "Renderable.h"

#include <string>

namespace fro
{
	class Sprite final : public Renderable
	{
		fro_GENERATED_RENDERABLE_BODY(Sprite)

	public:
		void setFileName(const std::string& fileName);

	private:
		std::string m_FileName{};
	};
}