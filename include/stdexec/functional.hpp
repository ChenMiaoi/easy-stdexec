#ifndef STDEXEC_FUNCTIONAL_HPP
#define STDEXEC_FUNCTIONAL_HPP

#include "__detail/__type_traits.hpp"

namespace stdexec {
  /**
   * 在下方注明了，std::invoke在编译期计算的开销是巨大的，但是我们需要在编译期实现，因此就需要自己实现一个simple invoke
   * 此处用到了`调用分发技术`，通过给出的参数和符号不同，在编译期时就能选择对应的调用路径
   * */
  namespace __tag_invoke {
    // NOT TO SPEC: Don't require tag_invocable to subsume invocable.
    // std::invoke is more expensive at compile time than necessary,
    // and results in diagnostics that are more verbose than necessary.
    void tag_invoke();

    /**
     * 通过`tag_invocable`约束`__tag`是可被调用的，
     * 也就是说，如果是一个类，就必须实现`operator()`，规定了一种统一的调用方式
     * 最终通过`tag_invoke`接口进行调用
     * */
    template <typename _Tag, typename ... _Args>
    concept tag_invocable =
      requires(_Tag __tag, _Args&& ... __args) {
        tag_invoke((_Tag&&)__tag, (_Args&&)__args...);
      };

    /**
     * 在`tag_invocable`的基础上，约束了返回值类型
     * */
    template <typename _Ret, typename _Tag, typename ... _Args>
    concept __tag_invocable_r =
      requires(_Tag __tag, _Args&& ... __args) {
        { static_cast<_Ret>(tag_invoke((_Tag&&)__tag, (_Args&&)__args...)) };
      };

    /**
     * 在`tag_invocable`的基础上，约束了调用是`noexcept`的
     * */
    template <typename _Tag, typename ... _Args>
    concept nothrow_tag_invocable =
      tag_invocable<_Tag, _Args...> &&
      requires(_Tag __tag, _Args&& ... __args) {
        { tag_invoke((_Tag&&)__tag, (_Args&&)__args...) } noexcept;
      };

    /**
     * 获取`tag_invoke`的返回值类型
     * */
    template <typename _Tag, typename ... _Args>
    using tag_invoke_result_t = decltype(tag_invoke(__declval<_Tag>(), __declval<_Args>()...));

    template <typename _Tag, typename ... _Args>
    struct tag_invoke_result {};

    /**
     * 如果满足`tag_invocable`，那么就能够获取该调用的返回值类型
     * */
    template <typename _Tag, typename ... _Args>
      requires tag_invocable<_Tag, _Args...>
      struct tag_invoke_result<_Tag, _Args...> {
        using type = tag_invoke_result_t<_Tag, _Args...>;
      };

    /**
     * 通过`operator ()`实现通用调用接口
     * 需要满足`tag_invocable`条件
     * */
    struct tag_invoke_t {
      template <typename _Tag, typename ... _Args>
        requires tag_invocable<_Tag, _Args...>
        [[clang::always_inline]]
        constexpr auto operator() (_Tag __tag, _Args&&... __args) const
        noexcept(nothrow_tag_invocable<_Tag, _Args...>) -> tag_invoke_result_t<_Tag, _Args...> {
          return tag_invoke((_Tag&&)__tag, (_Args&&)__args...);
        }
    };
  } //! namespace __tag_invoke

  using __tag_invoke::tag_invoke_t;

  namespace __ti {
    /**
     * 统一的调用接口：`tag_invoke(_Tag, _Args...)`
     * */
    inline constexpr tag_invoke_t tag_invoke{};
  }

  using namespace __ti;

  template <auto& _Tag>
  using tag_t = __decay_t<decltype(_Tag)>;

  using __tag_invoke::tag_invocable;
  using __tag_invoke::__tag_invocable_r;
  using __tag_invoke::nothrow_tag_invocable;
  using __tag_invoke::tag_invoke_result;
  using __tag_invoke::tag_invoke_result_t;
}

#endif //! STDEXEC_FUNCTIONAL_HPP
