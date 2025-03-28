#ifndef VECTOR2_HPP
#define VECTOR2_HPP

#include "arithmetic.hpp"
#include "froch.hpp"
#include "resulting_component.hpp"
#include "utility/exception.hpp"

namespace fro
{
   template <Arithmetic Component>
   struct Vector2 final
   {
      template <Arithmetic OtherComponent>
      [[nodiscard]] explicit operator Vector2<OtherComponent>() const
      {
         return { static_cast<OtherComponent>(x), static_cast<OtherComponent>(y) };
      }

      [[nodiscard]] Component& operator[](std::size_t const index)
      {
         switch (index)
         {
            case 0:
               return x;

            case 1:
               return y;

            default:
               exception("index {}} is outside the [0, 1] range!", index);
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
               exception("index {}} is outside the [0, 1] range!", index);
         }
      }

      template <Arithmetic OtherComponent>
      [[nodiscard]] Vector2<ResultingComponent<Component, OtherComponent>> operator+(Vector2<OtherComponent> const& vector) const
      {
         return { x + vector.x, y + vector.y };
      }

      template <Arithmetic OtherComponent>
         requires std::same_as<ResultingComponent<Component, OtherComponent>, Component>
      Vector2& operator+=(Vector2<OtherComponent> const& vector)
      {
         return *this = *this + vector;
      }

      [[nodiscard]] Vector2 operator-() const
      {
         return { -x, -y };
      }

      template <Arithmetic OtherComponent>
      [[nodiscard]] Vector2<ResultingComponent<Component, OtherComponent>> operator-(Vector2<OtherComponent> const& vector) const
      {
         return { x - vector.x, y - vector.y };
      }

      template <Arithmetic OtherComponent>
         requires std::same_as<ResultingComponent<Component, OtherComponent>, Component>
      Vector2& operator-=(Vector2<OtherComponent> const& vector)
      {
         return *this = *this - vector;
      }

      template <Arithmetic OtherComponent>
      [[nodiscard]] Vector2<ResultingComponent<Component, OtherComponent>> operator*(Vector2<OtherComponent> const& vector) const
      {
         return { x * vector.x + y * vector.y };
      }

      template <Arithmetic Multiplier>
      [[nodiscard]] Vector2<ResultingComponent<Component, Multiplier>> operator*(Multiplier const multiplier) const
      {
         return { x * multiplier, y * multiplier };
      }

      template <Arithmetic Multiplier>
         requires std::same_as<ResultingComponent<Component, Multiplier>, Component>
      Vector2& operator*=(Multiplier const multiplier)
      {
         return *this = *this * multiplier;
      }

      template <Arithmetic Divider>
      [[nodiscard]] Vector2<ResultingComponent<Component, Divider>> operator/(Divider const divider) const
      {
         return { x / divider, y / divider };
      }

      template <Arithmetic Divider>
         requires std::same_as<ResultingComponent<Component, Divider>, Component>
      Vector2& operator/=(Divider const divider)
      {
         return *this = *this / divider;
      }

      template <Arithmetic OtherComponent>
      [[nodiscard]] bool operator==(Vector2<OtherComponent> const& vector) const
      {
         return x == vector.x and y == vector.y;
      }

      [[nodiscard]] auto magnitude() const
      {
         return std::sqrt(x * x + y * y);
      }

      [[nodiscard]] auto normalized() const
      {
         auto const magnitude{ this->magnitude() };
         return Vector2<ResultingComponent<Component, decltype(magnitude)>>{ x / magnitude, y / magnitude };
      }

      Vector2& normalize()
         requires std::same_as<Vector2, decltype(normalized())>
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