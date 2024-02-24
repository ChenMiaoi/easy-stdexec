#ifndef EASY_STDEXEC___DOMAIN_HPP
#define EASY_STDEXEC___DOMAIN_HPP

#include "__env.hpp"
#include "__meta.hpp"
#include "__config.hpp"
#include "__concepts.hpp"
#include "__execution_fwd.hpp"

#include "../functional.hpp"

namespace stdexec {
  struct default_domain;
  struct dependent_domain;

  namespace __domain {
    template <typename _Tag>
    using __legacy_c11n_for = typename _Tag::__legacy_customization_t;

    template <typename _Tag, typename ... _Args>
    using __legacy_c11_fn =
      __make_dispatcher<__legacy_c11n_for<_Tag>, __none_such, _Args...>;
  } //! namespace __domain
} //! namespace stdexec

#endif //EASY_STDEXEC___DOMAIN_HPP
