#ifndef STDEXEC_FUNCTIONAL_HPP
#define STDEXEC_FUNCTIONAL_HPP

#include "__detail/__type_traits.hpp"

namespace stdexec {
  /**
   * ���·�ע���ˣ�std::invoke�ڱ����ڼ���Ŀ����Ǿ޴�ģ�����������Ҫ�ڱ�����ʵ�֣���˾���Ҫ�Լ�ʵ��һ��simple invoke
   * �˴��õ���`���÷ַ�����`��ͨ�������Ĳ����ͷ��Ų�ͬ���ڱ�����ʱ����ѡ���Ӧ�ĵ���·��
   * */
  namespace __tag_invoke {
    // NOT TO SPEC: Don't require tag_invocable to subsume invocable.
    // std::invoke is more expensive at compile time than necessary,
    // and results in diagnostics that are more verbose than necessary.
    void tag_invoke();

    /**
     * ͨ��`tag_invocable`Լ��`__tag`�ǿɱ����õģ�
     * Ҳ����˵�������һ���࣬�ͱ���ʵ��`operator()`���涨��һ��ͳһ�ĵ��÷�ʽ
     * ����ͨ��`tag_invoke`�ӿڽ��е���
     * */
    template <typename _Tag, typename ... _Args>
    concept tag_invocable =
      requires(_Tag __tag, _Args&& ... __args) {
        tag_invoke((_Tag&&)__tag, (_Args&&)__args...);
      };

    /**
     * ��`tag_invocable`�Ļ����ϣ�Լ���˷���ֵ����
     * */
    template <typename _Ret, typename _Tag, typename ... _Args>
    concept __tag_invocable_r =
      requires(_Tag __tag, _Args&& ... __args) {
        { static_cast<_Ret>(tag_invoke((_Tag&&)__tag, (_Args&&)__args...)) };
      };

    /**
     * ��`tag_invocable`�Ļ����ϣ�Լ���˵�����`noexcept`��
     * */
    template <typename _Tag, typename ... _Args>
    concept nothrow_tag_invocable =
      tag_invocable<_Tag, _Args...> &&
      requires(_Tag __tag, _Args&& ... __args) {
        { tag_invoke((_Tag&&)__tag, (_Args&&)__args...) } noexcept;
      };

    /**
     * ��ȡ`tag_invoke`�ķ���ֵ����
     * */
    template <typename _Tag, typename ... _Args>
    using tag_invoke_result_t = decltype(tag_invoke(__declval<_Tag>(), __declval<_Args>()...));

    template <typename _Tag, typename ... _Args>
    struct tag_invoke_result {};

    /**
     * �������`tag_invocable`����ô���ܹ���ȡ�õ��õķ���ֵ����
     * */
    template <typename _Tag, typename ... _Args>
      requires tag_invocable<_Tag, _Args...>
      struct tag_invoke_result<_Tag, _Args...> {
        using type = tag_invoke_result_t<_Tag, _Args...>;
      };

    /**
     * ͨ��`operator ()`ʵ��ͨ�õ��ýӿ�
     * ��Ҫ����`tag_invocable`����
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
     * ͳһ�ĵ��ýӿڣ�`tag_invoke(_Tag, _Args...)`
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
