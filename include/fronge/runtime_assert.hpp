#ifndef RUNTIME_ASSERT_HPP
#define RUNTIME_ASSERT_HPP

#include "fronge/constants.hpp"
#include "fronge/locator.hpp"
#include "fronge/logger.hpp"

namespace fro
{
   template <typename Message>
   constexpr void runtime_assert(bool const condition, Message&& message,
      std::source_location location = std::source_location::current())
   {
      if constexpr (DEBUG)
      {
         if (condition)
            return;

         Locator::get<Logger>().error(std::forward<Message>(message), false, std::move(location));
         std::abort();
      }
   }
}

#endif