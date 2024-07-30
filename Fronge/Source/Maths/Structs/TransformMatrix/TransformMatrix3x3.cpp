#include "froch.hpp"

#include "TransformMatrix3x3.hpp"
#include "Maths/MathFunctions.hpp"

namespace fro
{
	TransformMatrix3x3::TransformMatrix3x3(Matrix3x3<double> transformation)
		: mTransformation{ std::move(transformation) }
		, mIsRotationDirty{ true }
		, mIsScaleDirty{ true }
	{
	}

	TransformMatrix3x3 TransformMatrix3x3::operator*(TransformMatrix3x3 const& other) const
	{
		return getTransformation() * other.getTransformation();
	}

	TransformMatrix3x3& TransformMatrix3x3::operator*=(TransformMatrix3x3 const& other)
	{
		return *this = other * *this;
	}

	bool TransformMatrix3x3::operator==(TransformMatrix3x3 const& other) const
	{
		return getTransformation() == other.getTransformation();
	}

	TransformMatrix3x3::operator Matrix3x3<double>const& () const
	{
		return getTransformation();
	}

	void TransformMatrix3x3::translate(Vector2<double> const translation)
	{
		mTransformation[0][2] += translation.x;
		mTransformation[1][2] += translation.y;
	}

	void TransformMatrix3x3::rotate(double const rotation)
	{
		setRotation(getRotation() + rotation);
	}

	void TransformMatrix3x3::scale(Vector2<double> const scale)
	{
		Vector2<double> const currentScale{ getScale() };
		setScale({ currentScale.x * scale.x, currentScale.y * scale.y });
	}

	void TransformMatrix3x3::setTranslation(Vector2<double> const translation)
	{
		mTransformation[0][2] = translation.x;
		mTransformation[1][2] = translation.y;
	}

	void TransformMatrix3x3::setRotation(double const rotation)
	{
		if (mIsScaleDirty)
		{
			calculateScale();
			mIsScaleDirty = false;
		}

		mRotation = rotation;
		mIsRotationDirty = false;

		mAreTrigonometricValuesDirty = true;
		mIsTransformationDirty = true;
	}

	void TransformMatrix3x3::setScale(Vector2<double> const scale)
	{
		if (mIsRotationDirty)
		{
			calculateRotation();
			mIsRotationDirty = false;
		}

		mScale = scale;
		mIsScaleDirty = false;

		mIsTransformationDirty = true;
	}

	Vector2<double> TransformMatrix3x3::getTranslation() const
	{
		return { mTransformation[0][2], mTransformation[1][2] };
	}

	double TransformMatrix3x3::getRotation() const
	{
		if (mIsRotationDirty)
		{
			calculateRotation();
			mIsRotationDirty = false;
		}

		return mRotation;
	}

	Vector2<double> TransformMatrix3x3::getScale() const
	{
		if (mIsScaleDirty)
		{
			calculateScale();
			mIsScaleDirty = false;
		}

		return mScale;
	}

	TransformMatrix3x3 TransformMatrix3x3::getInverse() const
	{
		return mTransformation.getInverse();
	}

	TransformMatrix3x3& TransformMatrix3x3::inverse()
	{
		return *this = getInverse();
	}

	Matrix3x3<double> const& TransformMatrix3x3::getTransformation() const
	{
		if (mIsTransformationDirty)
		{
			if (mAreTrigonometricValuesDirty)
			{
				mRotationCosine = std::cos(mRotation);
				mRotationSine = std::sin(mRotation);

				mAreTrigonometricValuesDirty = false;
			}

			mTransformation[0][0] = mScale.x * mRotationCosine;
			mTransformation[0][1] = -mScale.y * mRotationSine;
			mTransformation[1][0] = mScale.x * mRotationSine;
			mTransformation[1][1] = mScale.y * mRotationCosine;

			mIsTransformationDirty = false;
		}

		return mTransformation;
	}

	void TransformMatrix3x3::calculateRotation() const
	{
		Matrix3x3<double> const& transformation{ getTransformation() };
		mRotation = std::atan2(transformation[1][0], transformation[0][0]);
	}

	void TransformMatrix3x3::calculateScale() const
	{
		Matrix3x3<double> const& transformation{ getTransformation() };
		mScale.x = math::getMagnitude(Vector2{ transformation[0][0], transformation[1][0] });
		mScale.y = math::getMagnitude(Vector2{ transformation[0][1], transformation[1][1] });
	}
}