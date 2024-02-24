#ifndef STDEXEC_COROUTINE_HPP
#define STDEXEC_COROUTINE_HPP

#include "concepts.hpp"

#include "__detail/__config.hpp"
#include "__detail/__concepts.hpp"

#include <version>
#if __cpp_impl_coroutine >= 201902 && __cpp_lib_coroutine >= 201902
#include <coroutine>
namespace __coro = std;
#endif

namespace stdexec {
#if !STDEXEC_STD_NO_COROUTINES_
  // Define some concepts and utilities for working with awaitable
  template <typename _Tp>
  concept __await_suspend_result =
    __one_of<_Tp, void, bool> || __is_instance_of<_Tp, __coro::coroutine_handle>;

  template <typename _Awaitable, typename _Promise>
  concept __with_await_suspend =
    same_as<_Promise, void> ||
    requires(_Awaitable& __awaiter, __coro::coroutine_handle<_Promise> __h) {
      { __awaiter.await_suspend(__h) } -> __await_suspend_result;
    };

  template <typename _Awaiter, typename _Promise = void>
  concept __awaiter = //
    requires(_Awaiter& __awaiter) {
      __awaiter.await_ready() ? 1 : 0;
      __awaiter.await_resume();
    } && __with_await_suspend<_Awaiter, _Promise>;

  template <typename _Awaitable>
  decltype(auto) __get_awaiter(_Awaitable&& __awaitable, void*) {
    if constexpr (requires { ((_Awaitable&&)__awaitable).operator co_await(); }) {
      return ((_Awaitable&&)__awaitable).operator co_await();
    } else if constexpr (requires { operator co_await((_Awaitable&&)__awaitable); }) {
      return operator co_await((_Awaitable&&)__awaitable);
    } else {
      return (_Awaitable&&)__awaitable;
    }
  }

  template <typename _Awaitable, typename _Promise>
  decltype(auto) __get_awaiter(_Awaitable&& __awaitable, _Promise* __promise)
    requires requires { __promise->await_transform((_Awaitable&&)__awaitable); } {
    if constexpr (requires { __promise->await_transform((_Awaitable&&)__awaitable).operator co_await(); }) {
      return __promise->await_transform((_Awaitable&&)__awaitable).operator co_await();
    } else if constexpr (requires { operator co_await(__promise->await_transform((_Awaitable&&)__awaitable)); }) {
      return operator co_await(__promise->await_transform((_Awaitable&&)__awaitable));
    } else {
      return __promise->await_transform((_Awaitable&&)__awaitable);
    }
  }

  template <typename _Awaitable, typename _Promise = void>
  concept __awaitable =
    requires(_Awaitable&& __awaitable, _Promise* __promise) {
      { stdexec::__get_awaiter((_Awaitable&&)__awaitable, __promise) } -> __awaiter<_Promise>;
    };

  template <typename _Ty>
  _Ty& __as_lvalue(_Ty&&);

  template <typename _Awaitable, typename _Promise = void>
    requires __awaitable<_Awaitable, _Promise>
      using __await_result_t =
    decltype(stdexec::__as_lvalue(
      stdexec::__get_awaiter(std::declval<_Awaitable>(), (_Promise*)nullptr)
    ).await_resume());
#else
  template <typename _Awaitable, typename _Promise = void>
  concept __awaitable = false;

  template <typename _Awaitable, typename _Promise = void>
    requires __awaitable<_Awaitable, _Promise>
      using __await_result_t = void;
#endif
}

#endif //STDEXEC_COROUTINE_HPP
