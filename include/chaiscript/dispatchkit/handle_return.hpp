// This file is distributed under the BSD License.
// See "license.txt" for details.
// Copyright 2009-2012, Jonathan Turner (jonathan@emptycrate.com)
// Copyright 2009-2015, Jason Turner (jason@emptycrate.com)
// http://www.chaiscript.com

#ifndef CHAISCRIPT_HANDLE_RETURN_HPP_
#define CHAISCRIPT_HANDLE_RETURN_HPP_

#include <functional>
#include <memory>
#include <type_traits>

#include "boxed_number.hpp"
#include "boxed_value.hpp"

namespace chaiscript {
class Boxed_Number;
}  // namespace chaiscript

namespace chaiscript
{
  namespace dk
  {
    template<class T, class U> class Proxy_Fun_Callable_Impl;
    template<class T> class Assignable_Proxy_Fun_Impl;

    namespace det
    {
      /**
       * Used internally for handling a return value from a Proxy_Function call
       */
      template<typename Ret>
        struct Handle_Ret
        {
          template<typename T,
                   typename = typename std::enable_if<std::is_pod<typename std::decay<T>::type>::value>::type>
          static Boxed_Value handle(T r)
          {
            return Boxed_Value(std::move(r), true);
          }

          template<typename T,
                   typename = typename std::enable_if<!std::is_pod<typename std::decay<T>::type>::value>::type>
          static Boxed_Value handle(T &&r)
          {
            return Boxed_Value(std::make_shared<T>(std::forward<T>(r)), true);
          }
        };

      template<typename Ret>
        struct Handle_Ret<const std::function<Ret> &>
        {
          static Boxed_Value handle(const std::function<Ret> &f) {
            return Boxed_Value(
                chaiscript::make_shared<dk::Proxy_Fun_B, dk::Proxy_Fun_Callable_Impl<Ret, std::function<Ret>>>(f)
              );
          }
        };

      template<typename Ret>
        struct Handle_Ret<std::function<Ret>>
        {
          static Boxed_Value handle(const std::function<Ret> &f) {
            return Boxed_Value(
                chaiscript::make_shared<dk::Proxy_Fun_B, dk::Proxy_Fun_Callable_Impl<Ret, std::function<Ret>>>(f)
              );
          }
        };

      template<typename Ret>
        struct Handle_Ret<const std::shared_ptr<std::function<Ret>>>
        {
          static Boxed_Value handle(const std::shared_ptr<std::function<Ret>> &f) {
            return Boxed_Value(
                chaiscript::make_shared<dk::Proxy_Fun_B, dk::Assignable_Proxy_Fun_Impl<Ret>>(std::ref(*f),f)
                );
          }
        };

      template<typename Ret>
        struct Handle_Ret<const std::shared_ptr<std::function<Ret>> &>
        {
          static Boxed_Value handle(const std::shared_ptr<std::function<Ret>> &f) {
            return Boxed_Value(
                chaiscript::make_shared<dk::Proxy_Fun_B, dk::Assignable_Proxy_Fun_Impl<Ret>>(std::ref(*f),f)
              );
          }
        };

      template<typename Ret>
        struct Handle_Ret<std::shared_ptr<std::function<Ret>>>
        {
          static Boxed_Value handle(const std::shared_ptr<std::function<Ret>> &f) {
            return Boxed_Value(
                chaiscript::make_shared<dk::Proxy_Fun_B, dk::Assignable_Proxy_Fun_Impl<Ret>>(std::ref(*f),f)
              );
          }
        };

      template<typename Ret>
        struct Handle_Ret<std::function<Ret> &>
        {
          static Boxed_Value handle(std::function<Ret> &f) {
            return Boxed_Value(
                chaiscript::make_shared<dk::Proxy_Fun_B, dk::Assignable_Proxy_Fun_Impl<Ret>>(std::ref(f),
                  std::shared_ptr<std::function<Ret>>())
              );
          }

          static Boxed_Value handle(const std::function<Ret> &f) {
            return Boxed_Value(
                chaiscript::make_shared<dk::Proxy_Fun_B, dk::Proxy_Fun_Callable_Impl<Ret, std::function<Ret>>>(f)
              );
          }
        };

      template<typename Ret>
        struct Handle_Ret<Ret *>
        {
          static Boxed_Value handle(Ret *p)
          {
            return Boxed_Value(p, true);
          }
        };

      template<typename Ret>
        struct Handle_Ret<const Ret *>
        {
          static Boxed_Value handle(const Ret *p)
          {
            return Boxed_Value(p, true);
          }
        };

      template<typename Ret>
        struct Handle_Ret<std::shared_ptr<Ret> &>
        {
          static Boxed_Value handle(const std::shared_ptr<Ret> &r)
          {
            return Boxed_Value(r, true);
          }
        };

      template<typename Ret>
        struct Handle_Ret<std::shared_ptr<Ret> >
        {
          static Boxed_Value handle(const std::shared_ptr<Ret> &r)
          {
            return Boxed_Value(r, true);
          }
        };

      template<typename Ret>
        struct Handle_Ret<const std::shared_ptr<Ret> &>
        {
          static Boxed_Value handle(const std::shared_ptr<Ret> &r)
          {
            return Boxed_Value(r, true);
          }
        };

      template<typename Ret>
        struct Handle_Ret<const Ret &>
        {
          static Boxed_Value handle(const Ret &r)
          {
            return Boxed_Value(std::cref(r), true);
          }
        };


      /**
       * Used internally for handling a return value from a Proxy_Function call
       */
      template<typename Ret>
        struct Handle_Ret<Ret &>
        {
          static Boxed_Value handle(Ret &r)
          {
            return Boxed_Value(std::ref(r));
          }

          static Boxed_Value handle(const Ret &r)
          {
            return Boxed_Value(std::cref(r));
          }
        };

      /**
       * Used internally for handling a return value from a Proxy_Function call
       */
      template<>
        struct Handle_Ret<Boxed_Value>
        {
          static Boxed_Value handle(const Boxed_Value &r)
          {
            return r;
          }
        };

      /**
       * Used internally for handling a return value from a Proxy_Function call
       */
      template<>
        struct Handle_Ret<const Boxed_Value>
        {
          static Boxed_Value handle(const Boxed_Value &r)
          {
            return r;
          }
        };

      /**
       * Used internally for handling a return value from a Proxy_Function call
       */
      template<>
        struct Handle_Ret<Boxed_Value &>
        {
          static Boxed_Value handle(const Boxed_Value &r)
          {
            return r;
          }
        };

      /**
       * Used internally for handling a return value from a Proxy_Function call
       */
      template<>
        struct Handle_Ret<const Boxed_Value &>
        {
          static Boxed_Value handle(const Boxed_Value &r)
          {
            return r;
          }
        };

      /**
       * Used internally for handling a return value from a Proxy_Function call
       */
      template<>
        struct Handle_Ret<Boxed_Number>
        {
          static Boxed_Value handle(const Boxed_Number &r)
          {
            return r.bv;
          }
        };

      /**
       * Used internally for handling a return value from a Proxy_Function call
       */
      template<>
        struct Handle_Ret<const Boxed_Number>
        {
          static Boxed_Value handle(const Boxed_Number &r)
          {
            return r.bv;
          }
        };


      /**
       * Used internally for handling a return value from a Proxy_Function call
       */
      template<>
        struct Handle_Ret<void>
        {
          static Boxed_Value handle()
          {
            return Boxed_Value(Boxed_Value::Void_Type());
          }
        };
    }
  }
}

#endif
