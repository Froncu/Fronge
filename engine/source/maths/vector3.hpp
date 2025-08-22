#ifndef VECTOR3_HPP
#define VECTOR3_HPP

#include "arithmetic.hpp"
#include "froch.hpp"
#include "resulting_component.hpp"
#include "utility/exception.hpp"

namespace fro
{
   template <Arithmetic Component>
   struct Vector3 final
   {
      template <Arithmetic OtherComponent>
      [[nodiscard]] explicit operator Vector3<OtherComponent>() const
      {
         return { static_cast<OtherComponent>(x), static_cast<OtherComponent>(y), static_cast<OtherComponent>(z) };
      }

      [[nodiscard]] Component& operator[](std::size_t const index)
      {
         switch (index)
         {
            case 0:
               return x;

            case 1:
               return y;

            case 2:
               return z;

            default:
               exception("index {} is outside the [0, 2] range!", index);
         }
      }

      [[nodiscard]] Component operator[](std::size_t const index) const
      {
         switch (index)
         {
            case 0:
               return x;

            case 1:
               return y;

            case 2:
               return z;

            default:
               exception("index {} is outside the [0, 2] range!", index);
         }
      }

      template <Arithmetic OtherComponent>
      [[nodiscard]] Vector3<ResultingComponent<Component, OtherComponent>> operator+(
         Vector3<OtherComponent> const& vector) const
      {
         return { x + vector.x, y + vector.y, z + vector.z };
      }

      template <Arithmetic OtherComponent>
         requires std::same_as<ResultingComponent<Component, OtherComponent>, Component>
      Vector3& operator+=(Vector3<OtherComponent> const& vector)
      {
         return *this = *this + vector;
      }

      [[nodiscard]] Vector3 operator-() const
      {
         return { -x, -y, -z };
      }

      template <Arithmetic OtherComponent>
      [[nodiscard]] Vector3<ResultingComponent<Component, OtherComponent>> operator-(
         Vector3<OtherComponent> const& vector) const
      {
         return { x - vector.x, y - vector.y, z - vector.z };
      }

      template <Arithmetic OtherComponent>
         requires std::same_as<ResultingComponent<Component, OtherComponent>, Component>
      Vector3& operator-=(Vector3<OtherComponent> const& vector)
      {
         return *this = *this - vector;
      }

      template <Arithmetic OtherComponent>
      [[nodiscard]] ResultingComponent<Component, OtherComponent> operator*(Vector3<OtherComponent> const& vector) const
      {
         return x * vector.x + y * vector.y + z * vector.z;
      }

      template <Arithmetic Multiplier>
      [[nodiscard]] Vector3<ResultingComponent<Component, Multiplier>> operator*(Multiplier const multiplier) const
      {
         return { x * multiplier, y * multiplier, z * multiplier };
      }

      template <Arithmetic Multiplier>
         requires std::same_as<ResultingComponent<Component, Multiplier>, Component>
      Vector3& operator*=(Multiplier const multiplier)
      {
         return *this = *this * multiplier;
      }

      template <Arithmetic Divider>
      [[nodiscard]] Vector3<ResultingComponent<Component, Divider>> operator/(Divider const divider) const
      {
         return { x / divider, y / divider, z / divider };
      }

      template <Arithmetic Divider>
         requires std::same_as<ResultingComponent<Component, Divider>, Component>
      Vector3& operator/=(Divider const divider)
      {
         return *this = *this / divider;
      }

      template <Arithmetic OtherComponent>
      [[nodiscard]] bool operator==(Vector3<OtherComponent> const& vector) const
      {
         return x == vector.x and y == vector.y and z == vector.z;
      }

      [[nodiscard]] auto magnitude_squared() const
      {
         return x * x + y * y + z * z;
      }

      [[nodiscard]] auto magnitude() const
      {
         return std::sqrt(magnitude_squared());
      }

      [[nodiscard]] auto normalized() const
      {
         auto const magnitude{ this->magnitude() };
         return Vector3<ResultingComponent<Component, decltype(magnitude)>>{ x / magnitude, y / magnitude, z / magnitude };
      }

      Vector3& normalize()
         requires std::same_as<Vector3, decltype(normalized())>
      {
         return *this = normalized();
      }

      template <Arithmetic OtherComponent>
      [[nodiscard]] Vector3<ResultingComponent<Component, OtherComponent>> crossed(Vector3<OtherComponent> const& other) const
      {
         return {
            y * other.z - z * other.y,
            z * other.x - x * other.z,
            x * other.y - y * other.x
         };
      }

      template <Arithmetic OtherComponent>
         requires std::same_as<Vector3, decltype(crossed())>
      Vector3& cross(Vector3<OtherComponent> const& other)
      {
         return *this = crossed(other);
      }

      Component x;
      Component y;
      Component z;
   };
}

#endif