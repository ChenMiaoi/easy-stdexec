#ifndef EASY_STDEXEC_COROUTINE_HPP
#define EASY_STDEXEC_COROUTINE_HPP

#include "concepts.hpp"

#include <version>

#if __cpp_impl_coroutine >= 201902 && __cpp_lib_coroutine >= 201902
#include <coroutine>
namespace __coro = std;
#else
#error "EASY_STDEXEC less than C++20 is not supported."
#endif

namespace stdexec {
  template <typename _Promise, typename _Awaitater>
  decltype(auto) __await_suspend(_Awaitater& __await) {
    if constexpr (!same_as<_Promise, void>) {
      return __await.await_suspend(__coro::coroutine_handle<_Promise>());
    }
  }

  template <typename _Tp>
  concept __await_suspend_result =
    __one_of<_Tp, void, bool> || __is_instance_of<_Tp, __coro::coroutine_handle>;

  template <typename _Awaiter, typename _Promise = void>
  concept __awaiter =
    requires (_Awaiter& __await) {
      __await.await_ready() ? 1 : 0;
      { __await_suspend<_Promise>(__await) } -> __await_suspend_result;
      __await.await_resume();
    };

  template <typename _Awaitable>
  decltype(auto) __get_awaiter(_Awaitable&& __await, void*) {
    if constexpr (requires { ((_Awaitable&&)__await).operator co_await (); }) {
      return ((_Awaitable&&)__await).operator co_await ();
    } else if constexpr (requires { operator co_await ((_Awaitable&&)__await); }) {
      return operator co_await ((_Awaitable&&)__await);
    } else {
      return (_Awaitable&&)__await;
    }
  }

  template <typename _Awaitable, typename _Promise>
  decltype(auto) __get_awaiter(_Awaitable&& __await, _Promise* __promise)
    requires requires { __promise->await_transform((_Awaitable&&)__await);} {
    if constexpr (requires { __promise->await_transform((_Awaitable&&)__await).operator co_await (); }) {
      return __promise->await_transform((_Awaitable&&)__await).operator co_await ();
    } else if constexpr (requires { operator co_await (__promise->await_transform((_Awaitable&&)__await)); }) {
      return operator co_await (__promise->await_transform((_Awaitable&&)__await));
    } else {
      return __promise->await_transform((_Awaitable&&)__await);
    }
  }

  template <typename _Awaitable, typename _Promise = void>
  concept __awaitable =
    requires (_Awaitable&& __await, _Promise* __promise) {
      { __get_awaiter((_Awaitable&&)__await, __promise) } -> __awaiter<_Promise>;
    };

  template <typename _Ty>
  _Ty& __as_lvalue(_Ty&&);

  template <typename _Awaitable, typename _Promise = void>
    requires __awaitable<_Awaitable, _Promise>
      using __await_result_t = decltype(__as_lvalue(
        __get_awaiter(std::declval<_Awaitable>(), (_Promise*)nullptr)
      ).await_resume());
} //! namespace stdexec

#endif //EASY_STDEXEC_COROUTINE_HPP
