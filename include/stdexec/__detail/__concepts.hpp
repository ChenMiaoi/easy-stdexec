#ifndef STDEXEC___CONCEPTS_HPP
#define STDEXEC___CONCEPTS_HPP

#include "__config.hpp"
#include "__type_traits.hpp"

namespace stdexec {
  template <typename ...>
  struct __types;

  template <typename ... _Ts>
  concept __typename = requires { typename __types<_Ts...>; };

#if STDEXEC_GUNG()
  template <typename _Ap, typename _Bp>
  concept __same_as = __is_same(_Ap, _Bp);
#else
  template <typename _Ap, typename _Bp>
  inline constexpr bool __same_as_v = false;

  template <typename _Ap>
  inline constexpr bool __same_as_v<_Ap, _Ap> = true;

  template <typename _Ap, typename _Bp>
  concept __same_as = __same_as_v<_Ap, _Bp>;
#endif

  // Handy concepts
  template <typename _Ty, typename _Up>
  concept __decays_to = __same_as<__decay_t<_Ty>, _Up>;

  template <typename _Ty, typename _Up>
  concept __not_decays_to = !__decays_to<_Ty, _Up>;

  template <bool _TrueOrFalse>
  concept __satisfies = _TrueOrFalse;

  template <typename ...>
  concept __true = true;

  template <typename _Cp>
  concept __class = __true<int _Cp::*> && (!__same_as<const _Cp, _Cp>);

  template <typename _Ty, typename ... _As>
  concept __one_of = (__same_as<_Ty, _As> || ...);

  template <typename _Ty, typename ... _As>
  concept __all_of = (__same_as<_Ty, _As> && ...);

  template <typename _Ty, typename ... _As>
  concept __none_of = (!(__same_as<_Ty, _As>) && ...);

  template <typename, template <typename ...> typename>
  constexpr bool __is_instance_of_ = false;
  template <typename ... _As, template <typename ...> typename _Ty>
  constexpr bool __is_instance_of_<_Ty<_As...>, _Ty> = true;

  template <typename _Ay, template <typename ...> typename _Ty>
  concept __is_instance_of = __is_instance_of_<_Ay, _Ty>;

  template <typename _Ay, template <typename ...> typename _Ty>
  concept __is_not_instance_of = !__is_instance_of<_Ay, _Ty>;
}

#endif //STDEXEC___CONCEPTS_HPP
