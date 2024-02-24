#ifndef STDEXEC_EXECUTION_HPP
#define STDEXEC_EXECUTION_HPP

#include "__detail/__env.hpp"
#include "__detail/__meta.hpp"
#include "__detail/__config.hpp"
#include "__detail/__execution_fwd.hpp"

#include "concepts.hpp"
#include "coroutine.hpp"
#include "functional.hpp"

#include <concepts>

STDEXEC_PRAGMA_PUSH()

namespace stdexec {
  /////////////////////////////////////////////////////////////////////////////
//  template <typename _Sender, typename _Scheduler, typename _Tag = set_value_t>
//  concept __completes_on =
//    __decays_to<__call_result_t<get_completion_scheduler_t<>>>

//  template <typename _Sender, typename _Scheduler, typename _Env>
//  concept __starts_on =
//    __decays_to<__call_result_t<get_sche>

  /////////////////////////////////////////////////////////////////////////////
  // [execution.receivers]
  namespace __receivers {
    struct set_value_t {
      template <typename _Fn, typename ... _Args>
      using __f = __minvoke<_Fn, _Args...>;

      template <typename _Receiver, typename ... _As>
        requires tag_invocable<set_value_t, _Receiver, _As...>
        [[clang::always_inline]]
        void operator() (_Receiver&& __rcvr, _As&&... __as) const noexcept {
          static_assert(nothrow_tag_invocable<set_value_t, _Receiver, _As...>);
          (void )tag_invoke(set_value_t{}, (_Receiver&&)__rcvr, (_As&&)__as...);
        }
    };

    struct set_error_t {
      template <typename _Fn, typename ... _Args>
        requires(sizeof ... (_Args) == 1)
          using __f = __minvoke<_Fn, _Args...>;

      template <typename _Receiver, typename _Error>
        requires tag_invocable<set_error_t, _Receiver, _Error>
        [[clang::always_inline]]
        void operator() (_Receiver&& __rcvr, _Error&& __err) const noexcept {
          static_assert(nothrow_tag_invocable<set_error_t, _Receiver, _Error>);
          (void) tag_invoke(set_error_t{}, (_Receiver&&)__rcvr, (_Error&&)__err);
        }
    };

    struct set_stopped_t {
      template <typename _Fn, typename ... _Args>
        requires(sizeof ... (_Args) == 1)
          using __f = __minvoke<_Fn, _Args...>;

      template <typename _Receiver>
        requires tag_invocable<set_stopped_t, _Receiver>
          [[clang::always_inline]]
          void operator() (_Receiver&& __rcvr) const noexcept {
            static_assert(nothrow_tag_invocable<set_stopped_t, _Receiver>);
            (void) tag_invoke(set_stopped_t{}, (_Receiver&&)__rcvr);
          }
    };
  } //! namespace __receivers
  using __receivers::set_value_t;
  using __receivers::set_error_t;
  using __receivers::set_stopped_t;
  inline constexpr set_value_t set_value {};
  inline constexpr set_error_t set_error {};
  inline constexpr set_stopped_t set_stopped {};

  /////////////////////////////////////////////////////////////////////////////
  // completion_signatures
  namespace __compl_sigs {
    template <same_as<set_value_t> _Tag, typename _Ty = __q<__types>, typename ... _Args>
    __types<__minvoke<_Ty, _Args...>> __test(_Tag(*)(_Args...));
    template <same_as<set_error_t> _Tag, typename _Ty = __q<__types>, typename _Error>
    __types<__minvoke<_Ty, _Error>> __test(_Tag(*)(_Error));
    template <same_as<set_stopped_t> _Tag, typename _Ty = __q<__types>>
    __types<__minvoke<_Ty>> __test(_Tag(*)());
    template <typename, typename = void>
    __types<> __test(...);
    template <typename _Tag, typename _Ty = void, typename ... _Args>
    void __test(_Tag(*)(_Args...) noexcept) = delete;

    template <typename _Sig>
    concept __completion_signature = __typename<decltype(__compl_sigs::__test((_Sig*) nullptr))>;

    template <typename _Sig, typename _Tag, typename _Ty = __q<__types>>
    using __signal_args_t = decltype(__compl_sigs::__test<_Tag, _Ty>((_Sig*) nullptr));
  }
  using __compl_sigs::__completion_signature;

  template <__compl_sigs::__completion_signature... _Sigs>
  struct completion_signatures {};

  template <typename _Completion>
  concept __valid_completion_signatures =
    __ok<_Completion> && __is_instance_of<_Completion, completion_signatures>;

  template <typename _Completions>
  using __invalid_completion_signatures_t =
    __mbool<!__valid_completion_signatures<_Completions>>;

  /////////////////////////////////////////////////////////////////////////////
  // [execution.sndtraits]
  namespace __get_completion_signatures {
    // TODO
    template <typename _Sender, typename _Env>
    using __tfx_sender =
      ;
    struct get_completion_signatures_t {
      // TODO
    };
  } //! namespace __detail

  using __get_completion_signatures::get_completion_signatures_t;
  inline constexpr get_completion_signatures_t get_completion_signatures {};

  /////////////////////////////////////////////////////////////////////////////
  // [execution.senders]
  struct sender_t {
    using sender_concept = sender_t;
  };

  namespace __detail {
    template <typename _Sender>
    concept __enable_sender =
      derived_from<typename _Sender::sender_concept, sender_t>
      || requires { typename _Sender::is_sender; }
      || __awaitable<_Sender, __env::__promise<empty_env>>;
  } //! namespace __detail

  template <typename _Sender>
  inline constexpr bool enable_sender = __detail::__enable_sender<_Sender>;

  template <typename _Sender, typename _Env = empty_env>
  concept sender =
    enable_sender<__decay_t<_Sender>>;
    // TODO

  template <typename _Sender, typename _Env = empty_env>
  concept sender_in =
    sender<_Sender> &&
    requires(_Sender&& __sndr, _Env&& __env) {
      {
        get_completion_signatures((_Sender&&)__sndr, (_Env&&)__env)
      } -> __valid_completion_signatures;
    };

  namespace __as_awaitable {
    template <typename _Sender, typename _Promise>
    concept __awaitable_sender =
      sender_in<_Sender, env_of_t<_Promise&>> ;

    struct __unspecified {
      __unspecified get_return_object() noexcept;
      __unspecified initial_suspend() noexcept;
      __unspecified final_suspend() noexcept;
      void unhandle_exception() noexcept;
      void return_void() noexcept;
      __coro::coroutine_handle<> unhandle_stopped() noexcept;
    };

    struct as_awaitable_t {
      template <typename _Tp, typename _Promise>
      static constexpr auto __select_impl_() noexcept {
        if constexpr (tag_invocable<as_awaitable_t, _Tp, _Promise&>) {
          using _Result = tag_invoke_result_t<as_awaitable_t, _Tp, _Promise&>;
          constexpr bool _Nothrow = nothrow_tag_invocable<as_awaitable_t, _Tp, _Promise&>;
          return static_cast<_Result(*)() noexcept(_Nothrow)>(nullptr);
        } else if constexpr (__awaitable<_Tp, __unspecified>) {
          return static_cast<_Tp&&(*)() noexcept>(nullptr);
        } else {
          return static_cast<_Tp&&(*)() noexcept>(nullptr);
        }
      }
    };
  } //! namespace __as_awaitable
} //! namespace stdexec

STDEXEC_PRAGMA_POP()

#endif //STDEXEC_EXECUTION_HPP
