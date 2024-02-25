#ifndef EASY_STDEXEC___UTILITY_HPP
#define EASY_STDEXEC___UTILITY_HPP

#include <type_traits>

namespace stdexec {
  /**
   * std::decay��Ҫ������ `std::declval`����������`declval`�޷�����`void`���� \n
   * ���������Ҫʵ��һ�����Դ����declval����������ʵ�ֵ� `decay`
   */
  template <typename _Tp>
  _Tp &&__declval() noexcept;

  namespace __tt {
    /**
     * ʹ��`__decay_object`��ʵ�ֶԶ����`decay`���� \n
     * �ܹ��õ��������ͣ�
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
     * ʹ��`__decay_default`��ʵ�ֶ�Ĭ�����͵�`decay`����\n
     * �ܹ��õ��������ͣ�
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
     * ʹ��`__decay_abominable`��ʵ�ֶ�const�������͵�`decay`����\n
     * ȥ��`const`����Ϊ��`���ӵ�`
     *
     * �ܹ��õ���������:
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
     * �����Ĭ�����͵ĺ�����Ч��
     * \code{.cc}
     * int...(int, double, char...);
     * \endcode
     * @tparam _Ty ������int, double, char...
     * @tparam US ������int, double, char...
     */
    template <typename _Ty, typename ... US>
    extern __decay_default __mdecay<_Ty(US...)>;

    /**
     * �����`noexcept`Ĭ�����͵ĺ�����Ч��
     * \code{.cc}
     * int...(int, double, char...) noexcept;
     * \endcode
     * @tparam _Ty ������int, double, char...
     * @tparam US ������int, double, char...
     */
    template <typename _Ty, typename ... US>
    extern __decay_default __mdecay<_Ty(US...) noexcept>;

    /**
     * �����Ĭ�����͵ĺ�����������Ч��
     * \code{.cc}
     * // ����_Ty(&)(US...)���ԣ����Ч��_Ty(*)(US...)
     * // ʹ�÷����ͺ���ָ��һ��
     * using func_type = int(&)(double, char);
     *
     * int func(double, char) {...};
     *
     * func_type func_ref = func;
     * // ��һ��д��
     * int (&func_ref)(double, char) = func;
     * int (*func_ptr)(double, char) = func;
     * \endcode
     * @tparam _Ty ������int, double, char...
     * @tparam US ������int, double, char...
     */
    template <typename _Ty, typename ... US>
    extern __decay_default __mdecay<_Ty(&)(US...)>;

    /**
     * �����`noexcept`��Ĭ�����͵ĺ�����������Ч
     *
     * \code{.cc}
     * using func_type = int(&)(double, char) noexcept;
     *
     * int func(double, char) noexcept {...};
     *
     * int (&func_ref)(double, char) = func;
     * \endcode
     * @tparam _Ty ������int, double, char...
     * @tparam US ������int, double, char...
     */
    template <typename _Ty, typename ... US>
    extern __decay_default __mdecay<_Ty(&)(US...) noexcept>;

    /**
     * �����`const`��Ĭ�����͵ĳ�Ա������Ч
     *
     * \code{.cc}
     * int...(int, double, char...) const;
     * \endcode
     *
     * @tparam _Ty ������int, double, char...
     * @tparam US ������int, double, char...
     */
    template <typename _Ty, typename ... US>
    extern __decay_abominable __mdecay<_Ty(US...) const>;

    /**
     * �����`const noexcept`��Ĭ�����͵ĳ�Ա������Ч
     *
     * \code{.cc}
     * int...(int, double, char...) const noexcept;
     * \endcode
     *
     * @tparam _Ty ������int, double, char...
     * @tparam US ������int, double, char...
     */
    template <typename _Ty, typename ... US>
    extern __decay_abominable __mdecay<_Ty(US...) const noexcept>;

    /**
     * �����`const&`(��������)��Ĭ�����͵ĳ�Ա������Ч
     *
     * \code{.cc}
     * int...(int, double, char...) const&;
     * \endcode
     *
     * @tparam _Ty ������int, double, char...
     * @tparam US ������int, double, char...
     */
    template <typename _Ty, typename ... US>
    extern __decay_abominable __mdecay<_Ty(US...) const &>;

    /**
     * �����`const& noexcept`(��������)��Ĭ�����͵ĳ�Ա������Ч
     *
     * \code{.cc}
     * int...(int, double, char...) const& noexcept;
     * \endcode
     *
     * @tparam _Ty ������int, double, char...
     * @tparam US ������int, double, char...
     */
    template <typename _Ty, typename ... US>
    extern __decay_abominable __mdecay<_Ty(US...) const &noexcept>;

    /**
     * �����`const&&`(��ֵ����)��Ĭ�����͵ĳ�Ա������Ч
     *
     * \code{.cc}
     * int...(int, double, char...) const &&;
     * \endcode
     *
     * @tparam _Ty ������int, double, char...
     * @tparam US ������int, double, char...
     */
    template <typename _Ty, typename ... US>
    extern __decay_abominable __mdecay<_Ty(US...) const &&>;

    /**
     * �����`const&& noexcept`(��ֵ����)��Ĭ�����͵ĳ�Ա������Ч
     *
     * \code{.cc}
     * int...(int, double, char...) const && noexcept;
     * \endcode
     *
     * @tparam _Ty ������int, double, char...
     * @tparam US ������int, double, char...
     */
    template <typename _Ty, typename ... US>
    extern __decay_abominable __mdecay<_Ty(US...) const &&noexcept>;

    /**
     * �����ָ����ʽ��һά������Ч
     *
     * @tparam _Ty ������int, double, char...
     */
    template <typename _Ty>
    extern __decay_default __mdecay<_Ty[]>;

    /**
     * �����ͨ�������ϵ�һά������Ч
     *
     * @tparam _Ty ������int, double, char...
     * @tparam N unsigned int
     */
    template <typename _Ty, std::size_t N>
    extern __decay_default __mdecay<_Ty[N]>;

    /**
     * ����������������Ч
     *
     * @tparam _Ty ������int, double, char...
     * @tparam N unsigned int
     */
    template <typename _Ty, std::size_t N>
    extern __decay_default __mdecay<_Ty(&)[N]>;

    /**
     * �����`void`������Ч
     */
    template <>
    inline __decay_void __mdecay<void>;

    /**
     * �����`void const`������Ч
     */
    template <>
    inline __decay_void __mdecay<void const>;
  } //! namespace stdexec::__tt

  /**
   * ͨ����������ػ�ʵ�֣�ʹ��ģ��Ԫ���ͳһ�ӿڣ���װ��`__decay_t`
   */
  template <typename _Ty>
  using __decay_t =
    typename decltype(__tt::__mdecay<_Ty>)::template __f<_Ty>;

///////////////////////////////////////////////////////////////////////////////


} //! namesapce stdexec

#endif //EASY_STDEXEC___UTILITY_HPP
