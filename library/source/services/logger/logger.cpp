#include "logger.hpp"
#include "utility/exception.hpp"

namespace fro
{
   Logger::~Logger()
   {
      {
         std::lock_guard const lock{ mutex_ };
         run_thread_ = false;
      }

      condition_.notify_one();
   }

   void Logger::register_engine_source_root(std::filesystem::path user_root)
   {
      register_source_root(std::move(user_root), ENGINE_COMPILE_SOURCE_PATH.data());
   }

   void Logger::register_source_root(std::filesystem::path user_root, std::filesystem::path compile_root)
   {
      if (not std::filesystem::exists(user_root))
         exception("the specified user source root must exist");

      if (compile_root.empty())
         exception("the specified compile root cannot be empty");

      source_roots_.emplace_back(std::move(user_root), std::move(compile_root));
   }

   std::stacktrace_entry Logger::location(std::stacktrace::size_type stack_trace_depth)
   {
      // TODO: remove this when MinGW works with std::stacktrace
      if constexpr (MINGW)
         return {};

      stack_trace_depth += 2;
      auto const stack_trace{ std::stacktrace::current(stack_trace_depth, 1) };
      return stack_trace[0];
   }

   void Logger::log(Payload const& payload)
   {
      std::ostream* output_stream;
      switch (payload.type)
      {
         case Type::INFO:
            output_stream = &std::clog;
            break;

         case Type::WARNING:
            [[fallthrough]];

         case Type::ERROR:
            output_stream = &std::cerr;
            break;

         default:
            output_stream = &std::cout;
      }

      std::string_view esc_sequence;
      switch (payload.type)
      {
         case Type::INFO:
            esc_sequence = payload.engine_level ? "1;2;36;40" : "1;36;40";
            break;

         case Type::WARNING:
            esc_sequence = payload.engine_level ? "1;2;33;40" : "1;33;40";
            break;

         case Type::ERROR:
            esc_sequence = payload.engine_level ? "1;2;31;40" : "1;31;40";
            break;
      }

      // TODO: this works only if there is enough debug information present
      std::filesystem::path path{ payload.location.source_file() };
      for (auto const& [user_root, compile_root] : source_roots_)
         if (std::ranges::mismatch(compile_root, path).in1 == compile_root.end())
         {
            path = user_root / std::filesystem::relative(path, compile_root);
            break;
         }

      std::string source_file_location;
      if (std::filesystem::exists(path))
         source_file_location = std::format("{}({})", path.lexically_normal().string(), payload.location.source_line());
      else
         source_file_location = "source file not found";

      std::time_t const now{ std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()) };
      std::tm local_time;
      localtime_s(&local_time, &now);

      std::ostringstream time_stream;
      time_stream << std::put_time(&local_time, "%H:%M:%S");

      // TODO: remove this when MinGW works with std::println
      if constexpr (MINGW)
         *output_stream << std::format(
            "\033[{}m>> {}\n[{}] {:6}: {}\033[0m",
            esc_sequence,
            source_file_location,
            time_stream.str(),
            payload.engine_level ? "FRONGE" : "APP",
            payload.message);
      else
         std::println(*output_stream,
            "\033[{}m>> {}\n[{}] {:6}: {}\033[0m",
            esc_sequence,
            source_file_location,
            time_stream.str(),
            payload.engine_level ? "FRONGE" : "APP",
            payload.message);
   }

   void Logger::log_once(Payload const& payload)
   {
      if (payload.location and not stacktrace_entries_.insert(payload.location).second)
         return;

      return log(payload);
   }
}