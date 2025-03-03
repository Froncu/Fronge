#ifndef VOID_DELETER_HPP
#define VOID_DELETER_HPP

namespace fro
{
   template <typename T>
   static void void_deleter(void* value)
   {
      delete static_cast<T*>(value);
   }
}

#endif