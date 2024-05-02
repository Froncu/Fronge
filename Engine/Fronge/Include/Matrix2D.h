#if not defined fro_TRANSFORMATION_H
#define fro_TRANSFORMATION_H

#include "Defines.hpp"

#include <glm.hpp>

namespace fro
{
	class Matrix2D final
	{
	public:
		Matrix2D(glm::mat3x3 const& transformation);
		Matrix2D() = default;
		Matrix2D(Matrix2D const&) = default;
		Matrix2D(Matrix2D&&) noexcept = default;

		~Matrix2D() = default;

		Matrix2D& operator=(Matrix2D const&) = default;
		Matrix2D& operator=(Matrix2D&&) noexcept = default;
		Matrix2D& operator*=(Matrix2D const& other);
		Matrix2D& operator/=(Matrix2D const& other);

		void setTransformation(glm::mat3x3 const& transformation);
		void setTranslation(glm::vec2 const& translation);
		void setRotation(float const rotation);
		void setScale(glm::vec2 const& scale);

		glm::mat3x3 const& getTransformation() const;
		glm::vec2 getTranslation() const;
		float getRotation() const;
		glm::vec2 const& getScale() const;

	private:
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

	Matrix2D operator*(Matrix2D const& matrix1, Matrix2D const& matrix2);
	Matrix2D operator/(Matrix2D const& matrix1, Matrix2D const& matrix2);
}

#endif