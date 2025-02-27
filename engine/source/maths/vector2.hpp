#ifndef VECTOR2_HPP
#define VECTOR2_HPP

#include "froch.hpp"
#include "maths/arithmetic.hpp"
#include "utility/exception.hpp"

namespace fro
{
   template <Arithmetic Component>
   struct Vector2 final
   {
      [[nodiscard]] Component& operator[](std::size_t const index)
      {
         switch (index)
         {
            case 0:
               return x;

            case 1:
               return y;

            default:
               exception("index {} outside [0, 1] range!", index);
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

            default:
               exception("index {} outside [0, 1] range!", index);
         }
      }

      template <Arithmetic OtherComponent>
      [[nodiscard]] constexpr auto operator+(Vector2<OtherComponent> const& vector) const
      {
         return Vector2
         {
            x + vector.x,
            y + vector.y
         };
      }

      template <Arithmetic OtherComponent>
      constexpr auto operator+=(Vector2<OtherComponent> const& vector)
      {
         return *this = *this + vector;
      }

      [[nodiscard]] constexpr Vector2 operator-() const
      {
         return { -x, -y };
      }

      template <Arithmetic OtherComponent>
      [[nodiscard]] constexpr auto operator-(Vector2<OtherComponent> const& vector) const
      {
         return Vector2
         {
            x - vector.x,
            y - vector.y
         };
      }

      template <Arithmetic OtherComponent>
      constexpr auto operator-=(Vector2<OtherComponent> const& vector)
      {
         return *this = *this - vector;
      }

      template <Arithmetic OtherComponent>
      [[nodiscard]] constexpr auto operator*(Vector2<OtherComponent> const& vector) const
      {
         return
            x * vector.x +
            y * vector.y;
      }

      template <Arithmetic Multiplier>
      [[nodiscard]] constexpr auto operator*(Multiplier const multiplier) const
      {
         return Vector2{ x * multiplier, y * multiplier };
      }

      template <Arithmetic Multiplier>
      [[nodiscard]] constexpr auto operator*=(Multiplier const multiplier) const
      {
         return *this = *this * multiplier;
      }

      template <Arithmetic Divider>
      [[nodiscard]] constexpr auto operator/(Divider const divider) const
      {
         return Vector2{ x / divider, y / divider };
      }

      template <Arithmetic Divider>
      [[nodiscard]] constexpr auto operator/=(Divider const divider) const
      {
         return *this = *this / divider;
      }

      template <Arithmetic OtherComponent>
      [[nodiscard]] constexpr bool operator==(Vector2<OtherComponent> const& vector) const
      {
         return
            x == vector.x and
            y == vector.y;
      }

      [[nodiscard]] auto magnitude() const
      {
         return std::sqrt(x * x + y * y);
      }

      [[nodiscard]] Vector2 normalized() const
      {
         auto const magnitude{ magnitude() };
         return { x / magnitude, y / magnitude };
      }

      Vector2& normalize()
      {
         return *this = normalized();
      }

      template <Arithmetic OtherType>
      [[nodiscard]] auto cross(Vector2<OtherType> const& vector) const
      {
         return x * vector.y - y * vector.x;
      }

      Component x;
      Component y;
   };
}

#endif