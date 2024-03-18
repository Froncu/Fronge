#pragma once

#include "Renderable.h"

#include <string>

fro_GENERATED_RENDERABLE_BODY
(
	Sprite,

	public:
		void setFileName(const std::string& fileName);

	private:
		std::string m_FileName{};
)