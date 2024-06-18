#include "TransformationMatrix2D.h"

#pragma region Constructors/Destructor
fro::TransformationMatrix2D::TransformationMatrix2D(glm::mat3x3 const& transformation)
	: m_Transformation{ transformation }
	, m_IsRotationDirty{ true }
	, m_IsScaleDirty{ true }
{
}
#pragma endregion Constructors/Destructor



#pragma region Operators
fro::TransformationMatrix2D& fro::TransformationMatrix2D::operator*=(TransformationMatrix2D const& other)
{
	*this = *this * other;

	return *this;
}

fro::TransformationMatrix2D& fro::TransformationMatrix2D::operator/=(TransformationMatrix2D const& other)
{
	*this = *this / other;

	return *this;
}

fro::TransformationMatrix2D fro::operator*(TransformationMatrix2D const& matrix1, TransformationMatrix2D const& matrix2)
{
	return matrix1.getTransformation() * matrix2.getTransformation();
}

fro::TransformationMatrix2D fro::operator/(TransformationMatrix2D const& matrix1, TransformationMatrix2D const& matrix2)
{
	return matrix1.getTransformation() * glm::inverse(matrix2.getTransformation());
}
#pragma endregion Operators



#pragma region PublicMethods
void fro::TransformationMatrix2D::transform(glm::mat3x3 const& transformation)
{
	m_Transformation *= transformation;
	m_IsTransformationDirty = false;

	m_IsRotationDirty = true;
	m_IsScaleDirty = true;
}

void fro::TransformationMatrix2D::translate(glm::vec2 const& translation)
{
	m_Transformation[0][2] += translation.x;
	m_Transformation[1][2] += translation.y;
}

void fro::TransformationMatrix2D::rotate(float const rotation)
{
	setRotation(getRotation() + rotation);
}

void fro::TransformationMatrix2D::scale(glm::vec2 const& scale)
{
	setScale(getScale() * scale);
}

void fro::TransformationMatrix2D::setTransformation(glm::mat3x3 const& transformation)
{
	m_Transformation = transformation;
	m_IsTransformationDirty = false;

	m_IsRotationDirty = true;
	m_IsScaleDirty = true;
}

void fro::TransformationMatrix2D::setTranslation(glm::vec2 const& translation)
{
	m_Transformation[0][2] = translation.x;
	m_Transformation[1][2] = translation.y;
}

void fro::TransformationMatrix2D::setRotation(float const rotation)
{
	if (m_IsScaleDirty)
	{
		calculateScale();
		m_IsScaleDirty = false;
	}

	m_Rotation = rotation;
	m_IsRotationDirty = false;

	m_AreTrigonometricValuesDirty = true;
	m_IsTransformationDirty = true;
}

void fro::TransformationMatrix2D::setScale(glm::vec2 const& scale)
{
	if (m_IsRotationDirty)
	{
		calculateRotation();
		m_IsRotationDirty = false;
	}

	m_Scale = scale;
	m_IsScaleDirty = false;

	m_IsTransformationDirty = true;
}

glm::mat3x3 const& fro::TransformationMatrix2D::getTransformation() const
{
	if (m_IsTransformationDirty)
	{
		if (m_AreTrigonometricValuesDirty)
		{
			float const rotation{ getRotation() };
			m_RotationCosine = glm::cos(rotation);
			m_RotationSine = glm::sin(rotation);

			m_AreTrigonometricValuesDirty = false;
		}

		glm::vec2 const& scale{ getScale() };
		m_Transformation[0][0] = scale.x * m_RotationCosine;
		m_Transformation[0][1] = -scale.y * m_RotationSine;
		m_Transformation[1][0] = scale.x * m_RotationSine;
		m_Transformation[1][1] = scale.y * m_RotationCosine;

		m_IsTransformationDirty = false;
	}

	return m_Transformation;
}

glm::vec2 fro::TransformationMatrix2D::getTranslation() const
{
	return { m_Transformation[0][2], m_Transformation[1][2] };
}

float fro::TransformationMatrix2D::getRotation() const
{
	if (m_IsRotationDirty)
	{
		calculateRotation();
		m_IsRotationDirty = false;
	}

	return m_Rotation;
}

glm::vec2 const& fro::TransformationMatrix2D::getScale() const
{
	if (m_IsScaleDirty)
	{
		calculateScale();
		m_IsScaleDirty = false;
	}

	return m_Scale;
}
#pragma endregion PublicMethods



#pragma region PrivateMethods
void fro::TransformationMatrix2D::calculateRotation() const
{
	glm::mat3x3 const& transformation{ getTransformation() };
	m_Rotation = glm::atan(transformation[1][0], transformation[0][0]);
}

void fro::TransformationMatrix2D::calculateScale() const
{
	glm::mat3x3 const& transformation{ getTransformation() };
	m_Scale.x = glm::sqrt(transformation[0][0] * transformation[0][0] + transformation[1][0] * transformation[1][0]);
	m_Scale.y = glm::sqrt(transformation[0][1] * transformation[0][1] + transformation[1][1] * transformation[1][1]);
}
#pragma endregion PrivateMethods