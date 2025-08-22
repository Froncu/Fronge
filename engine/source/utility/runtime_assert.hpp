#ifndef RUNTIME_ASSERT_HPP
#define RUNTIME_ASSERT_HPP

#include "constants.hpp"
#include "services/locator.hpp"
#include "services/logger/logger.hpp"

namespace fro
{
   template <typename... Arguments>
   void runtime_assert([[maybe_unused]] bool const condition,
      [[maybe_unused]] std::format_string<Arguments...> const format,
      [[maybe_unused]] Arguments&&... arguments)
   {
      if constexpr (DEBUG)
      {
         if (condition)
            return;

         Locator::get<Logger>().error(format, std::forward<Arguments>(arguments)...);
         std::abort();
      }
   }

   template <typename Message>
   void runtime_assert(bool const condition, Message&& message)
   {
      runtime_assert(condition, "{}", std::forward<Message>(message));
   }
}

#endif