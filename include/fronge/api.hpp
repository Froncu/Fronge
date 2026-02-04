#ifndef API_HPP
#define API_HPP

#ifdef FRO_SHARED
   #ifdef _MSC_VER
      #ifdef FRO_ENGINE_LEVEL
         #define FRO_API __declspec(dllexport)
      #else
         #define FRO_API __declspec(dllimport)
      #endif
      #define FRO_UDT
   #else
      #define FRO_API __attribute__((__visibility__("default")))
      #define FRO_UDT __attribute__((__visibility__("default")))
   #endif
#else
   #define FRO_API
   #define FRO_UDT
#endif

#endif