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
   * 编译期计算的const bool类型
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
   * 外部声明`__ok_v`，此处表示一般情况下`__ok_v`的类型为`success`状态
   * */
  template <typename>
  extern __msuccess __ok_v;

  /**
   * 特化`__ok_v`，在模板为<_What, _With...>的情况下的类型为`_ERROR_`
   * */
  template <typename _What, typename ... _With>
  extern _ERROR_<_What, _With...> __ok_v<__mexception<_What, _With...>>;

  /**
   * `__ok_t`只会对一般情况下的`__ok_v`起效，因此恒为`__msuccess`
   * */
  template <typename _Ty>
  using __ok_t = decltype(__ok_v<_Ty>);

  template <typename ... _Ts>
  using __disp = decltype((__msuccess(), ..., __ok_t<_Ts>()));
  
  template <typename _Arg>
  concept __ok = __same_as<__ok_t<_Arg>, __msuccess>;

  /**
   * `_OK`约束每一个参数都必须是`__msuccess`的
   * */
  template <typename ... _Args>
  concept _OK = (__ok<_Args> && ...);

  template <bool _AllOK>
  struct __i;

  /**
   * 通过`_OK`做出约束，`_Args`必须满足`__msuccess`，然后变为`_Fn<_Args...>`
   * */
  template <template <typename ...> typename _Fn, typename ... _Args>
  using __meval = typename __i<_OK<_Args...>>::template __g<_Fn, _Args...>;

  /**
   * 通过`_OK`做出约束，`_Args`必须满足`__msuccess`，
   * 同时，_Fn在此处是结构体，其满足`_Fn::__f`，就能得到`_Fn::__f<_Args...>`
   * */
  template <typename _Fn, typename ... _Args>
  using __minvoke = typename __i<_OK<_Fn>>::template __h<_Fn, _Args...>;

  template <bool _AllOK>
  struct __i {
    /**
     * 对_Fn的一种封装，变为_Fn<_Args...>
     * */
    template <template <typename ...> typename _Fn, typename ... _Args>
    using __g = _Fn<_Args...>;

    /**
     * 这里对_Fn做出限制，结构体必须满足`_Fn::__f`，且`_Args`满足`__msuccess`才能有`_Fn::__f<_Args...>`
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
   * `__declval<_Fun>()`获得_Fun&&类型
   * __declval<_As>()...获得其参数列表_As&&...
   * 最终得到 _Fun&&(_As&&...)，然后得出其调用返回类型
   * */
  template <typename _Fun, typename ... _As>
  using __call_result_t = decltype(__declval<_Fun>()(__declval<_As>()...));
}

#endif //EASY_STDEXEC___META_HPP
