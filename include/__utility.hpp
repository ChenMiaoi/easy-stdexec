#ifndef EASY_STDEXEC___UTILITY_HPP
#define EASY_STDEXEC___UTILITY_HPP

#include <type_traits>

namespace stdexec {
  /**
   * std::decay需要依赖于 `std::declval`，但是由于`declval`无法处理`void`类型 \n
   * 因此我们需要实现一个可以处理的declval用于我们自实现的 `decay`
   */
  template <typename _Tp>
  _Tp &&__declval() noexcept;

  namespace __tt {
    /**
     * 使用`__decay_object`来实现对对象的`decay`方法 \n
     * 能够得到以下类型：
     * \code{.cc}
     * __f = const _Ty& -> _Ty&& -> _Ty
     * \endcode
     */
    struct __decay_object {
      template <typename _Ty>
      static _Ty __g(const _Ty &);

      template <typename _Ty>
      using __f = decltype(__g(__declval<_Ty>()));
    };

    /**
     * 使用`__decay_default`来实现对默认类型的`decay`方法\n
     * 能够得到以下类型：
     * \code{.cc}
     * __f = __Ty -> _Ty
     * \endcode
     */
    struct __decay_default {
      template <typename _Ty>
      static _Ty __g(_Ty);

      template <typename _Ty>
      using __f = decltype(__g(__declval<_Ty>()));
    };

    /**
     * 使用`__decay_abominable`来实现对const函数类型的`decay`方法\n
     * 去除`const`的行为是`恶劣的`
     *
     * 能够得到以下类型:
     */
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

    /**
     * 针对于默认类型的函数生效：
     * \code{.cc}
     * int...(int, double, char...);
     * \endcode
     * @tparam _Ty 可以是int, double, char...
     * @tparam US 可以是int, double, char...
     */
    template <typename _Ty, typename ... US>
    extern __decay_default __mdecay<_Ty(US...)>;

    /**
     * 针对于`noexcept`默认类型的函数生效：
     * \code{.cc}
     * int...(int, double, char...) noexcept;
     * \endcode
     * @tparam _Ty 可以是int, double, char...
     * @tparam US 可以是int, double, char...
     */
    template <typename _Ty, typename ... US>
    extern __decay_default __mdecay<_Ty(US...) noexcept>;

    /**
     * 针对于默认类型的函数的引用生效：
     * \code{.cc}
     * // 对于_Ty(&)(US...)而言，其等效于_Ty(*)(US...)
     * // 使用方法和函数指针一致
     * using func_type = int(&)(double, char);
     *
     * int func(double, char) {...};
     *
     * func_type func_ref = func;
     * // 另一种写法
     * int (&func_ref)(double, char) = func;
     * int (*func_ptr)(double, char) = func;
     * \endcode
     * @tparam _Ty 可以是int, double, char...
     * @tparam US 可以是int, double, char...
     */
    template <typename _Ty, typename ... US>
    extern __decay_default __mdecay<_Ty(&)(US...)>;

    /**
     * 针对于`noexcept`的默认类型的函数的引用生效
     *
     * \code{.cc}
     * using func_type = int(&)(double, char) noexcept;
     *
     * int func(double, char) noexcept {...};
     *
     * int (&func_ref)(double, char) = func;
     * \endcode
     * @tparam _Ty 可以是int, double, char...
     * @tparam US 可以是int, double, char...
     */
    template <typename _Ty, typename ... US>
    extern __decay_default __mdecay<_Ty(&)(US...) noexcept>;

    /**
     * 针对于`const`的默认类型的成员函数生效
     *
     * \code{.cc}
     * int...(int, double, char...) const;
     * \endcode
     *
     * @tparam _Ty 可以是int, double, char...
     * @tparam US 可以是int, double, char...
     */
    template <typename _Ty, typename ... US>
    extern __decay_abominable __mdecay<_Ty(US...) const>;

    /**
     * 针对于`const noexcept`的默认类型的成员函数生效
     *
     * \code{.cc}
     * int...(int, double, char...) const noexcept;
     * \endcode
     *
     * @tparam _Ty 可以是int, double, char...
     * @tparam US 可以是int, double, char...
     */
    template <typename _Ty, typename ... US>
    extern __decay_abominable __mdecay<_Ty(US...) const noexcept>;

    /**
     * 针对于`const&`(常量引用)的默认类型的成员函数生效
     *
     * \code{.cc}
     * int...(int, double, char...) const&;
     * \endcode
     *
     * @tparam _Ty 可以是int, double, char...
     * @tparam US 可以是int, double, char...
     */
    template <typename _Ty, typename ... US>
    extern __decay_abominable __mdecay<_Ty(US...) const &>;

    /**
     * 针对于`const& noexcept`(常量引用)的默认类型的成员函数生效
     *
     * \code{.cc}
     * int...(int, double, char...) const& noexcept;
     * \endcode
     *
     * @tparam _Ty 可以是int, double, char...
     * @tparam US 可以是int, double, char...
     */
    template <typename _Ty, typename ... US>
    extern __decay_abominable __mdecay<_Ty(US...) const &noexcept>;

    /**
     * 针对于`const&&`(右值引用)的默认类型的成员函数生效
     *
     * \code{.cc}
     * int...(int, double, char...) const &&;
     * \endcode
     *
     * @tparam _Ty 可以是int, double, char...
     * @tparam US 可以是int, double, char...
     */
    template <typename _Ty, typename ... US>
    extern __decay_abominable __mdecay<_Ty(US...) const &&>;

    /**
     * 针对于`const&& noexcept`(右值引用)的默认类型的成员函数生效
     *
     * \code{.cc}
     * int...(int, double, char...) const && noexcept;
     * \endcode
     *
     * @tparam _Ty 可以是int, double, char...
     * @tparam US 可以是int, double, char...
     */
    template <typename _Ty, typename ... US>
    extern __decay_abominable __mdecay<_Ty(US...) const &&noexcept>;

    /**
     * 针对于指针形式的一维数组生效
     *
     * @tparam _Ty 可以是int, double, char...
     */
    template <typename _Ty>
    extern __decay_default __mdecay<_Ty[]>;

    /**
     * 针对于通常意义上的一维数组生效
     *
     * @tparam _Ty 可以是int, double, char...
     * @tparam N unsigned int
     */
    template <typename _Ty, std::size_t N>
    extern __decay_default __mdecay<_Ty[N]>;

    /**
     * 针对于数组的引用生效
     *
     * @tparam _Ty 可以是int, double, char...
     * @tparam N unsigned int
     */
    template <typename _Ty, std::size_t N>
    extern __decay_default __mdecay<_Ty(&)[N]>;

    /**
     * 针对于`void`类型生效
     */
    template <>
    inline __decay_void __mdecay<void>;

    /**
     * 针对于`void const`类型生效
     */
    template <>
    inline __decay_void __mdecay<void const>;
  } //! namespace stdexec::__tt

  /**
   * 通过上面各种特化实现，使用模板元编程统一接口，封装出`__decay_t`
   */
  template <typename _Ty>
  using __decay_t =
    typename decltype(__tt::__mdecay<_Ty>)::template __f<_Ty>;

///////////////////////////////////////////////////////////////////////////////


} //! namesapce stdexec

#endif //EASY_STDEXEC___UTILITY_HPP
