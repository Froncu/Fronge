#if not defined fro_TRANSFORMATION_H
#define fro_TRANSFORMATION_H

#include "Defines.hpp"

#include <glm.hpp>

namespace fro
{
	class TransformationMatrix2D final
	{
	public:
		TransformationMatrix2D(glm::mat3x3 const& transformation);
		TransformationMatrix2D() = default;
		TransformationMatrix2D(TransformationMatrix2D const&) = default;
		TransformationMatrix2D(TransformationMatrix2D&&) noexcept = default;

		~TransformationMatrix2D() = default;

		TransformationMatrix2D& operator=(TransformationMatrix2D const&) = default;
		TransformationMatrix2D& operator=(TransformationMatrix2D&&) noexcept = default;
		TransformationMatrix2D& operator*=(TransformationMatrix2D const& other);
		TransformationMatrix2D& operator/=(TransformationMatrix2D const& other);

		void transform(glm::mat3x3 const& transformation);
		void translate(glm::vec2 const& translation);
		void rotate(float const rotation);
		void scale(glm::vec2 const& scale);

		void setTransformation(glm::mat3x3 const& transformation);
		void setTranslation(glm::vec2 const& translation);
		void setRotation(float const rotation);
		void setScale(glm::vec2 const& scale);

		glm::mat3x3 const& getTransformation() const;
		glm::vec2 getTranslation() const;
		float getRotation() const;
		glm::vec2 const& getScale() const;

	private:
		void calculateRotation() const;
		void calculateScale() const;

		mutable glm::mat3x3 m_Transformation
		{
			1.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 1.0f
		};

		mutable glm::vec2 m_Scale{ 1.0f, 1.0f };
		mutable float m_Rotation{};
		mutable float m_RotationCosine{ glm::cos(m_Rotation) };
		mutable float m_RotationSine{ glm::sin(m_Rotation) };

		mutable bool m_IsTransformationDirty{};
		mutable bool m_AreTrigonometricValuesDirty{};
		mutable bool m_IsRotationDirty{};
		mutable bool m_IsScaleDirty{};
	};

	TransformationMatrix2D operator*(TransformationMatrix2D const& matrix1, TransformationMatrix2D const& matrix2);
	TransformationMatrix2D operator/(TransformationMatrix2D const& matrix1, TransformationMatrix2D const& matrix2);
}

#endif