#if not defined fro_TEXT_H
#define fro_TEXT_H

#include "GameObject.h"
#include "Renderable.h"
#include "Typenames.hpp"

#include <string>

struct SDL_Texture;

namespace fro
{
	template<typename TextType>
	concept StringAssignable =
		std::is_assignable_v<std::string, TextType>;

	template<typename TextType>
	concept StringConvertible = requires(TextType text)
	{
		{ std::to_string(text) };
	};

	class Text final : public Renderable
	{
	public:
		Text(Reference<GameObject> const parentingGameObject);

		virtual ~Text() override = default;

		virtual void render() const override;

		void setFont(std::string fontName, int const fontSize);

		template<typename TextType>
			requires StringConvertible<TextType> or StringAssignable<TextType>
		void setText(TextType&& text)
		{
			if constexpr (StringConvertible<TextType>)
				m_Text = std::to_string(text);
			else
				m_Text = std::forward<TextType>(text);

			updateTexture();
		}

	private:
		Text(Text const&) = delete;
		Text(Text&&) noexcept = delete;

		Text& operator=(Text const&) = delete;
		Text& operator=(Text&&) noexcept = delete;

		void updateTexture();

		Reference<Transform> m_Transform{ m_ParentingGameObject.get().forceGetComponent<Transform>() };

		CustomUniquePointer<SDL_Texture> m_pTexture{};

		std::string m_Text{};
		std::string m_FontName{};
		int m_FontSize{};
	};
}

#endif