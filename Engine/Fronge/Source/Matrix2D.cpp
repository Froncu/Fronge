#include "Matrix2D.h"

#pragma region Constructors/Destructor
fro::Matrix2D::Matrix2D(glm::mat3x3 const& transformation)
	: m_Transformation{ transformation }
	, m_IsRotationDirty{ true }
	, m_IsScaleDirty{ true }
{
}
#pragma endregion Constructors/Destructor



#pragma region Operators
fro::Matrix2D& fro::Matrix2D::operator*=(Matrix2D const& other)
{
	*this = *this * other;

	return *this;
}

fro::Matrix2D& fro::Matrix2D::operator/=(Matrix2D const& other)
{
	*this = *this / other;

	return *this;
}

fro::Matrix2D fro::operator*(Matrix2D const& matrix1, Matrix2D const& matrix2)
{
	return matrix1.getTransformation() * matrix2.getTransformation();
}

fro::Matrix2D fro::operator/(Matrix2D const& matrix1, Matrix2D const& matrix2)
{
	return matrix1.getTransformation() * glm::inverse(matrix2.getTransformation());
}
#pragma endregion Operators



#pragma region PublicMethods
void fro::Matrix2D::setTransformation(glm::mat3x3 const& transformation)
{
	m_Transformation = transformation;
	m_IsTransformationDirty = false;

	m_IsRotationDirty = true;
	m_IsScaleDirty = true;
}

void fro::Matrix2D::setTranslation(glm::vec2 const& translation)
{
	m_Transformation[0][2] = translation.x;
	m_Transformation[1][2] = translation.y;
}

void fro::Matrix2D::setRotation(float const rotation)
{
	m_Rotation = rotation;
	m_IsRotationDirty = false;

	m_AreTrigonometricValuesDirty = true;
	m_IsTransformationDirty = true;
}

void fro::Matrix2D::setScale(glm::vec2 const& scale)
{
	m_Scale = scale;
	m_IsScaleDirty = false;

	m_IsTransformationDirty = true;
}

glm::mat3x3 const& fro::Matrix2D::getTransformation() const
{
	if (m_IsTransformationDirty)
	{
		if (m_AreTrigonometricValuesDirty)
		{
			m_RotationCosine = glm::cos(getRotation());
			m_RotationSine = glm::sin(getRotation());

			m_AreTrigonometricValuesDirty = false;
		}

		m_Transformation[0][0] = getScale().x * m_RotationCosine;
		m_Transformation[0][1] = -getScale().y * m_RotationSine;
		m_Transformation[1][0] = getScale().x * m_RotationSine;
		m_Transformation[1][1] = getScale().y * m_RotationCosine;

		m_IsTransformationDirty = false;
	}

	return m_Transformation;
}

glm::vec2 fro::Matrix2D::getTranslation() const
{
	return { m_Transformation[0][2], m_Transformation[1][2] };
}

float fro::Matrix2D::getRotation() const
{
	if (m_IsRotationDirty)
	{
		m_Rotation = glm::atan(getTransformation()[1][0], getTransformation()[0][0]);

		m_IsRotationDirty = false;
	}

	return m_Rotation;
}

glm::vec2 const& fro::Matrix2D::getScale() const
{
	if (m_IsScaleDirty)
	{
		m_Scale.x = glm::sqrt(getTransformation()[0][0] * getTransformation()[0][0] + getTransformation()[1][0] * getTransformation()[1][0]);
		m_Scale.y = glm::sqrt(getTransformation()[0][1] * getTransformation()[0][1] + getTransformation()[1][1] * getTransformation()[1][1]);

		m_IsScaleDirty = false;
	}

	return m_Scale;
}
#pragma endregion PublicMethods