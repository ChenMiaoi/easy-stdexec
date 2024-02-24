#ifndef STDEXEC___EXECUTION_FWD_HPP
#define STDEXEC___EXECUTION_FWD_HPP

#include "__meta.hpp"
#include "__config.hpp"
#include "__concepts.hpp"
#include "__type_traits.hpp"

namespace stdexec {
  /////////////////////////////////////////////////////////////////////////////
  namespace __receivers {
    struct set_value_t;
    struct set_error_t;
    struct set_stopped_t;
  }
  using __receivers::set_value_t;
  using __receivers::set_error_t;
  using __receivers::set_stopped_t;
  extern const set_value_t set_value;
  extern const set_error_t set_error;
  extern const set_stopped_t set_stopped;

  template <typename _Tag>
  concept __completion_tag = __one_of<_Tag, set_value_t, set_error_t, set_stopped_t>;

  struct receiver_t;
  extern const bool enable_recevier;

  /////////////////////////////////////////////////////////////////////////////
  namespace __env {
    struct get_env_t;

    struct empty_env {
      using __t  = empty_env;
      using __id = empty_env;
    };
  }
  using __env::empty_env;
  using __env::get_env_t;
  extern const get_env_t get_env;

  template <typename _EnvProvider>
  using env_of_t = __call_result_t<get_env_t, _EnvProvider>;

  /////////////////////////////////////////////////////////////////////////////
  namespace __as_awaitable {
    struct as_awaitable_t;
  }
  using __as_awaitable::as_awaitable_t;
  extern const as_awaitable_t as_awaitable;

  /////////////////////////////////////////////////////////////////////////////
}

#endif //STDEXEC___EXECUTION_FWD_HPP
