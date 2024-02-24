#ifndef EASY_STDEXEC_CONCEPTS_HPP
#define EASY_STDEXEC_CONCEPTS_HPP

#if __has_include(<version>)
#include <version>
#else
#error "EASY_STEXEC less than C++17 is not supported."
#endif

#if __has_include(<concepts>) && __cpp_lib_concepts >= 202002
#include <concepts>
#define STDEXEC_HAS_STD_CONCEPT_HEADER() 1
#else
#error "EASY_STEXEC less than C++20 is not supported."
#endif

#include <type_traits>

namespace stdexec {
  /**
   * 在C++中，标准库并未对`__is_same`做出任何规定 \n
   *
   * `__is_same`的实现依赖于编译器扩展
   *
   * \code{.cpp}
   * template &lt typename _Ap, typename _Bp >
   * concept same_as = __is_same(_Ap, _Bp);
   * \endcode
   *
   * 因此，我们自行实现`__is_same`即可
   * */
  template <typename _Ap, typename _Bp>
  inline constexpr bool __same_as_v = false;

  template <typename _Ap>
  inline constexpr bool __same_as_v<_Ap, _Ap> = true;

  template <typename _Ap, typename _Bp>
  concept __same_as = __same_as_v<_Ap, _Bp>;

//////////////////////////////////////////////////////////////////////////////

  namespace stdexec::__std_concepts {
    template <typename _Ap, typename _Bp>
    concept same_as = __same_as<_Ap, _Bp> && __same_as<_Bp, _Ap>;

  #if STDEXEC_HAS_STD_CONCEPT_HEADER()
    using std::derived_from;
    using std::convertible_to;
    using std::equality_comparable;
  #endif
  } //! namespace stdexec::__std_concepts

  using namespace stdexec::__std_concepts;

//////////////////////////////////////////////////////////////////////////////

  // todo invocable

//////////////////////////////////////////////////////////////////////////////

  template <typename _Ty, typename ... _As>
  concept __one_of = (__same_as<_Ty, _As> || ...);

  template <typename _Ty, typename ... _As>
  concept __none_of = (!(__same_as<_Ty, _As>) && ...);

  template <typename, template <typename ...> typename>
  constexpr bool __is_instance_of_ = false;
  template <typename ... _As, template <typename ...> typename _Ty>
  constexpr bool __is_instance_of_<_Ty<_As...>, _Ty> = true;

  template <typename _Ay, template <typename ...> typename _Ty>
  concept __is_instance_of = __is_instance_of_<_Ay, _Ty>;
}

#endif //EASY_STDEXEC_CONCEPTS_HPP
