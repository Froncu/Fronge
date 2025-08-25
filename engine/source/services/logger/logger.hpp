#ifndef LOGGER_HPP
#define LOGGER_HPP

#include "constants.hpp"
#include "core.hpp"
#include "froch.hpp"

namespace fro
{
   class Logger final
   {
      protected:
         enum class Type
         {
            INFO,
            WARNING,
            ERROR
         };

      private:
         struct Payload final
         {
            Type type;
            bool engine_level;
            std::stacktrace_entry location;
            std::string message;
         };

         struct LogInfo final
         {
            bool once;
            Payload payload;
         };

      public:
         Logger() = default;
         Logger(Logger const&) = delete;
         Logger(Logger&&) = delete;

         FRO_API ~Logger();

         Logger& operator=(Logger const&) = delete;
         Logger& operator=(Logger&&) = delete;

         template <typename... Arguments>
         void info(std::format_string<Arguments...> const format, Arguments&&... arguments)
         {
            {
               std::lock_guard const lock{ mutex_ };
               log_queue_.push({
                  .once{ false },
                  .payload{
                     .type{ Type::INFO },
                     .engine_level{ ENGINE },
                     .location{ location() },
                     .message{ std::format(format, std::forward<Arguments>(arguments)...) }
                  }
               });
            }

            condition_.notify_one();
         }

         template <typename Message>
         void info(Message&& message)
         {
            {
               std::lock_guard const lock{ mutex_ };
               log_queue_.push({
                  .once{ false },
                  .payload{
                     .type{ Type::INFO },
                     .engine_level{ ENGINE },
                     .location{ location() },
                     .message{ std::format("{}", message) }
                  }
               });
            }

            condition_.notify_one();
         }

         template <typename... Arguments>
         void info_once(std::format_string<Arguments...> const format, Arguments&&... arguments)
         {
            {
               std::lock_guard const lock{ mutex_ };
               log_queue_.push({
                  .once{ true },
                  .payload{
                     .type{ Type::INFO },
                     .engine_level{ ENGINE },
                     .location{ location() },
                     .message{ std::format(format, std::forward<Arguments>(arguments)...) }
                  }
               });
            }

            condition_.notify_one();
         }

         template <typename Message>
         void info_once(Message&& message)
         {
            {
               std::lock_guard const lock{ mutex_ };
               log_queue_.push({
                  .once{ true },
                  .payload{
                     .type{ Type::INFO },
                     .engine_level{ ENGINE },
                     .location{ location() },
                     .message{ std::format("{}", message) }
                  }
               });
            }

            condition_.notify_one();
         }

         template <typename... Arguments>
         void warning(std::format_string<Arguments...> const format, Arguments&&... arguments)
         {
            {
               std::lock_guard const lock{ mutex_ };
               log_queue_.push({
                  .once{ false },
                  .payload{
                     .type{ Type::WARNING },
                     .engine_level{ ENGINE },
                     .location{ location() },
                     .message{ std::format(format, std::forward<Arguments>(arguments)...) }
                  }
               });
            }

            condition_.notify_one();
         }

         template <typename Message>
         void warning(Message&& message)
         {
            {
               std::lock_guard const lock{ mutex_ };
               log_queue_.push({
                  .once{ false },
                  .payload{
                     .type{ Type::WARNING },
                     .engine_level{ ENGINE },
                     .location{ location() },
                     .message{ std::format("{}", message) }
                  }
               });
            }

            condition_.notify_one();
         }

         template <typename... Arguments>
         void warning_once(std::format_string<Arguments...> const format, Arguments&&... arguments)
         {
            {
               std::lock_guard const lock{ mutex_ };
               log_queue_.push({
                  .once{ true },
                  .payload{
                     .type{ Type::WARNING },
                     .engine_level{ ENGINE },
                     .location{ location() },
                     .message{ std::format(format, std::forward<Arguments>(arguments)...) }
                  }
               });
            }

            condition_.notify_one();
         }

         template <typename Message>
         void warning_once(Message&& message)
         {
            {
               std::lock_guard const lock{ mutex_ };
               log_queue_.push({
                  .once{ true },
                  .payload{
                     .type{ Type::WARNING },
                     .engine_level{ ENGINE },
                     .location{ location() },
                     .message{ std::format("{}", message) }
                  }
               });
            }

            condition_.notify_one();
         }

         template <typename... Arguments>
         void error(std::format_string<Arguments...> const format, Arguments&&... arguments)
         {
            {
               std::lock_guard const lock{ mutex_ };
               log_queue_.push({
                  .once{ false },
                  .payload{
                     .type{ Type::ERROR },
                     .engine_level{ ENGINE },
                     .location{ location() },
                     .message{ std::format(format, std::forward<Arguments>(arguments)...) }
                  }
               });
            }

            condition_.notify_one();
         }

         template <typename Message>
         void error(Message&& message)
         {
            {
               std::lock_guard const lock{ mutex_ };
               log_queue_.push({
                  .once{ false },
                  .payload{
                     .type{ Type::ERROR },
                     .engine_level{ ENGINE },
                     .location{ location() },
                     .message{ std::format("{}", message) }
                  }
               });
            }

            condition_.notify_one();
         }

         template <typename... Arguments>
         void error_once(std::format_string<Arguments...> const format, Arguments&&... arguments)
         {
            {
               std::lock_guard const lock{ mutex_ };
               log_queue_.push({
                  .once{ true },
                  .payload{
                     .type{ Type::ERROR },
                     .engine_level{ ENGINE },
                     .location{ location() },
                     .message{ std::format(format, std::forward<Arguments>(arguments)...) }
                  }
               });
            }

            condition_.notify_one();
         }

         template <typename Message>
         void error_once(Message&& message)
         {
            {
               std::lock_guard const lock{ mutex_ };
               log_queue_.push({
                  .once{ true },
                  .payload{
                     .type{ Type::ERROR },
                     .engine_level{ ENGINE },
                     .location{ location() },
                     .message{ std::format("{}", message) }
                  }
               });
            }

            condition_.notify_one();
         }

      private:
         FRO_API [[nodiscard]] static std::stacktrace_entry location();

         FRO_API static void log(Payload const& payload);
         FRO_API void log_once(Payload const& payload);

         std::unordered_set<std::stacktrace_entry> stacktrace_entries_{};

         bool run_thread_{ true };
         std::queue<LogInfo> log_queue_{};

         std::mutex mutex_{};
         std::condition_variable condition_{};
         std::jthread thread_{
            [this]
            {
               while (true)
               {
                  LogInfo log_info;

                  {
                     std::unique_lock lock{ mutex_ };
                     condition_.wait(lock,
                        [this]
                        {
                           return not run_thread_ or not log_queue_.empty();
                        });

                     if (not run_thread_)
                        break;

                     log_info = std::move(log_queue_.front());
                     log_queue_.pop();
                  }

                  if (log_info.once)
                     log_once(log_info.payload);
                  else
                     log(log_info.payload);
               }
            }
         };
   };
}

#endif