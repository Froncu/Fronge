#if not defined TRANSFORM_MATRIX3X3_HPP
#define TRANSFORM_MATRIX3X3_HPP

#include "Maths/Structs/Matrix3x3.hpp"
#include "Maths/Structs/Vector2.hpp"

namespace fro
{
	class TransformMatrix3x3 final
	{
	public:
		TransformMatrix3x3() = default;
		FRO_API TransformMatrix3x3(Matrix3x3<double> transformation);
		TransformMatrix3x3(TransformMatrix3x3 const&) = default;
		TransformMatrix3x3(TransformMatrix3x3&&) noexcept = default;

		~TransformMatrix3x3() = default;

		TransformMatrix3x3& operator=(TransformMatrix3x3 const&) = default;
		TransformMatrix3x3& operator=(TransformMatrix3x3&&) noexcept = default;
		FRO_API FRO_NODISCARD TransformMatrix3x3 operator*(TransformMatrix3x3 const& other);
		FRO_API TransformMatrix3x3& operator*=(TransformMatrix3x3 const& other);
		FRO_API FRO_NODISCARD operator Matrix3x3<double> const& () const;

		FRO_API void translate(Vector2<double> const translation);
		FRO_API void rotate(double const rotation);
		FRO_API void scale(Vector2<double> const scale);

		FRO_API void setTranslation(Vector2<double> const translation);
		FRO_API void setRotation(double const rotation);
		FRO_API void setScale(Vector2<double> const scale);

		FRO_API FRO_NODISCARD Vector2<double> getTranslation() const;
		FRO_API FRO_NODISCARD double getRotation() const;
		FRO_API FRO_NODISCARD Vector2<double> getScale() const;

	private:
		FRO_NODISCARD Matrix3x3<double> const& getTransformation() const;
		FRO_NODISCARD void calculateRotation() const;
		FRO_NODISCARD void calculateScale() const;

		mutable Matrix3x3<double> mTransformation{ true };

		mutable Vector2<double> mScale{ 1.0, 1.0 };
		mutable double mRotation{};
		mutable double mRotationCosine{};
		mutable double mRotationSine{};

		mutable bool mIsTransformationDirty{};
		mutable bool mAreTrigonometricValuesDirty{ true };
		mutable bool mIsRotationDirty{};
		mutable bool mIsScaleDirty{};
	};
}

#endif