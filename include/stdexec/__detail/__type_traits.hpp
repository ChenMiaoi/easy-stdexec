#ifndef STDEXEC___TYPE_TRAITS_HPP
#define STDEXEC___TYPE_TRAITS_HPP

#include "__config.hpp"

namespace stdexec {
  // A very simple std::declval replacement that doesn't handle void
  template <typename _Tp>
  _Tp&& __declval() noexcept;

  // __decay_t: An efficient implementation for std::decay
  namespace __tt {
    struct __decay_object {
      template <typename _Ty>
      static _Ty __g(_Ty const&);
      template <typename _Ty>
      using __f = decltype(__g(__declval<_Ty>()));
    };

    struct __decay_default {
      template <typename _Ty>
      static _Ty __g(_Ty);
      template <typename _Ty>
      using __f = decltype(__g(__declval<_Ty>()));
    };

    struct __decay_abominable {
      template <typename _Ty>
      using __f = _Ty;
    };

    struct __decay_void {
      template <typename _Ty>
      using __f = void;
    };

    template <typename _Ty>
    extern __decay_object __mdecay;

    template <typename _Ty, typename ... US>
    extern __decay_default __mdecay<_Ty(US...)>;

    template <typename _Ty, typename... Us>
    extern __decay_default __mdecay<_Ty(Us...) noexcept>;

    template <typename _Ty, typename... Us>
    extern __decay_default __mdecay<_Ty (&)(Us...)>;

    template <typename _Ty, typename... Us>
    extern __decay_default __mdecay<_Ty (&)(Us...) noexcept>;

    template <typename _Ty, typename... Us>
    extern __decay_abominable __mdecay<_Ty(Us...) const>;

    template <typename _Ty, typename... Us>
    extern __decay_abominable __mdecay<_Ty(Us...) const noexcept>;

    template <typename _Ty, typename... Us>
    extern __decay_abominable __mdecay<_Ty(Us...) const &>;

    template <typename _Ty, typename... Us>
    extern __decay_abominable __mdecay<_Ty(Us...) const & noexcept>;

    template <typename _Ty, typename... Us>
    extern __decay_abominable __mdecay<_Ty(Us...) const &&>;

    template <typename _Ty, typename... Us>
    extern __decay_abominable __mdecay<_Ty(Us...) const && noexcept>;

    template <typename _Ty>
    extern __decay_default __mdecay<_Ty[]>;

    template <typename _Ty, std::size_t N>
    extern __decay_default __mdecay<_Ty[N]>;

    template <typename _Ty, std::size_t N>
    extern __decay_default __mdecay<_Ty (&)[N]>;

    template <>
    inline __decay_void __mdecay<void>;

    template <>
    inline __decay_void __mdecay<void const>;
  } //! namespace __tt

  template <typename _Ty>
  using __decay_t =
    typename decltype(__tt::__mdecay<_Ty>)::template __f<_Ty>;
}

#endif //STDEXEC___TYPE_TRAITS_HPP
