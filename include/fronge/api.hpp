#ifndef API_HPP
#define API_HPP

#ifdef FRO_SHARED
   #ifdef _MSC_VER
      #ifdef FRO_ENGINE
         #define FRO_API __declspec(dllexport)
      #else
         #define FRO_API __declspec(dllimport)
      #endif
   #else
      #ifdef FRO_ENGINE
         #define FRO_API __attribute__((visibility("default")))
      #else
         #define FRO_API
      #endif
   #endif
#else
   #define FRO_API
#endif

#endif