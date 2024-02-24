#ifndef STDEXEC___ENV_HPP
#define STDEXEC___ENV_HPP

#include "__config.hpp"
#include "__concepts.hpp"
#include "__execution_fwd.hpp"

#include "../concepts.hpp"
#include "../functional.hpp"

#include <concepts>
#include <exception>
#include <type_traits>

namespace stdexec {
  namespace __env {
    struct get_env_t {
      template <typename _EnvProvider>
        requires tag_invocable<get_env_t, const _EnvProvider&>
          [[clang::always_inline]]
          constexpr auto operator() (const _EnvProvider& __env_provider) const noexcept
          -> tag_invoke_result_t<get_env_t, const _EnvProvider&> {
            // TODO
          }
    };

    // To be kept in sync with the
    template <class _Env>
    struct __promise {
      template <typename _Ty>
      _Ty&& await_transform(_Ty&& __value) noexcept {
        return (_Ty&&)__value;
      }

      template <typename _Ty>
        requires tag_invocable<as_awaitable_t, _Ty, __promise&>
      auto await_transform(_Ty&& __value)
        noexcept(nothrow_tag_invocable<as_awaitable_t, _Ty, __promise&>)
        -> tag_invoke_result_t<as_awaitable_t, _Ty, __promise&> {
        return tag_invoke(as_awaitable, (_Ty&&)__value, *this);
      }

      template <same_as<get_env_t> _Tag>
      friend auto tag_invoke(_Tag, const __promise&) noexcept -> const _Env& {
        std::terminate();
      }
    };
  } //! namespace stdexec::__env
} //! namespace stdexec

#endif //STDEXEC___ENV_HPP
