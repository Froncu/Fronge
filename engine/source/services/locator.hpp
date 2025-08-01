// Inspired by Matias Devred's locator from his "gooey" engine
// (https://git.allpurposem.at/mat/gooey/src/branch/main/src/lib/services/locator.cppm)

#ifndef LOCATOR_HPP
#define LOCATOR_HPP

#include "core.hpp"
#include "froch.hpp"
#include "utility/type_index.hpp"
#include "utility/unique_pointer.hpp"
#include "utility/void_deleter.hpp"

namespace fro
{
   class Locator final
   {
      public:
         template <typename Service, std::derived_from<Service> Provider, typename... Arguments>
            requires std::constructible_from<Provider, Arguments...>
         static Service& set(Arguments&&... arguments)
         {
            UniquePointer<void> new_provider{ new Provider{ std::forward<Arguments>(arguments)... }, void_deleter<Provider> };
            UniquePointer<void>& current_provider{ find<Service>() };

            if constexpr (std::movable<Service>)
               if (current_provider)
                  *static_cast<Service*>(new_provider.get()) = std::move(*static_cast<Service*>(current_provider.get()));

            current_provider = std::move(new_provider);
            return *static_cast<Service*>(current_provider.get());
         }

         static void remove_providers()
         {
            services_.clear();
         }

         template <typename Service>
         static void remove_provider()
         {
            services_.erase(type_index<Service>());
         }

         template <std::default_initializable Service>
         [[nodiscard]] static Service& get()
         {
            return *static_cast<Service*>(internal_get<Service>().get());
         }

         template <typename Service>
         [[nodiscard]] static std::optional<Service&> get()
         {
            Service* service{ static_cast<Service*>(internal_get<Service>().get()) };
            if (not service)
               return std::nullopt;

            return *service;
         }

         Locator() = delete;
         Locator(Locator const&) = delete;
         Locator(Locator&&) = delete;

         ~Locator() = delete;

         Locator& operator=(Locator const&) = delete;
         Locator& operator=(Locator&&) = delete;

      private:
         template <typename Service>
         [[nodiscard]] static UniquePointer<void>& find()
         {
            return services_[type_index<Service>()];
         }

         template <typename Service>
         [[nodiscard]] static UniquePointer<void>& internal_get()
         {
            UniquePointer<void>& service{ find<Service>() };
            if constexpr (std::default_initializable<Service>)
               if (not service)
                  service = UniquePointer<void>{ new Service{}, void_deleter<Service> };

            return service;
         }

         FRO_API static std::unordered_map<std::type_index, UniquePointer<void>> services_;
   };
}

#endif