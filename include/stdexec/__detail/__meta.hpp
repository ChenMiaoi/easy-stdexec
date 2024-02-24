#ifndef EASY_STDEXEC___META_HPP
#define EASY_STDEXEC___META_HPP

#include <cstddef>
#include <cassert>
#include <exception>
#include <type_traits>

#include <utility>
#include "__config.hpp"
#include "__concepts.hpp"
#include "__type_traits.hpp"

namespace stdexec {
  struct __none_such {};

  /**
   * �����ڼ����const bool����
   * */
  template <bool _Bp>
  using __mbool = std::bool_constant<_Bp>;

  using __msuccess = int;

  template <typename _What, typename ... _With>
  struct _WARNING {};

  template <typename _What, typename ... _With>
  struct _ERROR_ {
    _ERROR_ operator, (__msuccess) const noexcept;
  };

  // TODO
//  template <__mstring _What>
//  struct _WHAT_ {};

  template <typename _What, typename ... _With>
  using __mexception = _ERROR_<_What, _With...>;

  /**
   * �ⲿ����`__ok_v`���˴���ʾһ�������`__ok_v`������Ϊ`success`״̬
   * */
  template <typename>
  extern __msuccess __ok_v;

  /**
   * �ػ�`__ok_v`����ģ��Ϊ<_What, _With...>������µ�����Ϊ`_ERROR_`
   * */
  template <typename _What, typename ... _With>
  extern _ERROR_<_What, _With...> __ok_v<__mexception<_What, _With...>>;

  /**
   * `__ok_t`ֻ���һ������µ�`__ok_v`��Ч����˺�Ϊ`__msuccess`
   * */
  template <typename _Ty>
  using __ok_t = decltype(__ok_v<_Ty>);

  template <typename ... _Ts>
  using __disp = decltype((__msuccess(), ..., __ok_t<_Ts>()));
  
  template <typename _Arg>
  concept __ok = __same_as<__ok_t<_Arg>, __msuccess>;

  /**
   * `_OK`Լ��ÿһ��������������`__msuccess`��
   * */
  template <typename ... _Args>
  concept _OK = (__ok<_Args> && ...);

  template <bool _AllOK>
  struct __i;

  /**
   * ͨ��`_OK`����Լ����`_Args`��������`__msuccess`��Ȼ���Ϊ`_Fn<_Args...>`
   * */
  template <template <typename ...> typename _Fn, typename ... _Args>
  using __meval = typename __i<_OK<_Args...>>::template __g<_Fn, _Args...>;

  /**
   * ͨ��`_OK`����Լ����`_Args`��������`__msuccess`��
   * ͬʱ��_Fn�ڴ˴��ǽṹ�壬������`_Fn::__f`�����ܵõ�`_Fn::__f<_Args...>`
   * */
  template <typename _Fn, typename ... _Args>
  using __minvoke = typename __i<_OK<_Fn>>::template __h<_Fn, _Args...>;

  template <bool _AllOK>
  struct __i {
    /**
     * ��_Fn��һ�ַ�װ����Ϊ_Fn<_Args...>
     * */
    template <template <typename ...> typename _Fn, typename ... _Args>
    using __g = _Fn<_Args...>;

    /**
     * �����_Fn�������ƣ��ṹ���������`_Fn::__f`����`_Args`����`__msuccess`������`_Fn::__f<_Args...>`
     * */
    template <typename _Fn, typename ... _Args>
    using __h = __meval<_Fn::template __f, _Args...>;
  };

  template <>
  struct __i<false> {
    template <template <typename ...> typename, typename ... _Args>
    using __g = __disp<_Args...>;

    template <typename _Fn, typename ...>
    using __h = _Fn;
  };

  template <template <typename ...> typename _Fn>
  struct __q {
    template <typename ... _Args>
    using __f = __meval<_Fn, _Args...>;
  };

  /**
   * `__declval<_Fun>()`���_Fun&&����
   * __declval<_As>()...���������б�_As&&...
   * ���յõ� _Fun&&(_As&&...)��Ȼ��ó�����÷�������
   * */
  template <typename _Fun, typename ... _As>
  using __call_result_t = decltype(__declval<_Fun>()(__declval<_As>()...));
}

#endif //EASY_STDEXEC___META_HPP
