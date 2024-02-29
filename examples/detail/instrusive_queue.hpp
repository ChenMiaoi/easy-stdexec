#ifndef EASY_STDEXEC_INSTRUSIVE_QUEUE_HPP
#define EASY_STDEXEC_INSTRUSIVE_QUEUE_HPP

#include <cassert>
#include <tuple>
#include <utility>

/// @tag 侵入式设计理念
///
/// 允许外部实体直接影响或操作内部成员，而不需要通过额外接口或继承关系
namespace example {
  /**
   * 声明侵入式队列
   *
   * @tparam Next 用于指定Item类中下一个元素的指针成员，实际上为 Item::*Next
   */
  template <auto Next>
  class intrusive_queue;

  /**
   * 侵入式队列的实现，特化Next，也就是要求Item必须拥有Item* Next成员
   *
   * @tparam Item 队列中元素的类型
   * @tparam Next 下一个元素的指针
   */
  template <typename Item, Item* Item::*Next>
  class intrusive_queue<Next> {
  public:
    intrusive_queue() noexcept = default;

    /**
     * @tag 移动构造
     *
     * 需要使用`std::exchange`来原子的将移动的设置为nullptr，并将`other`的值转交给`this`
     *
     * \code{.cc}
     *
     * _head(std::exchange(other._head, nullptr));
     * // 等价于 ==>
     * _head = std::move(other._head);
     * other._head = nullptr
     * // 过程是原子的
     *
     * \endcode
     *
     * @param other 另一个侵入式队列
     */
    intrusive_queue(intrusive_queue&& other) noexcept
      : _head(std::exchange(other._head, nullptr))
      , _tail(std::exchange(other._tail, nullptr)) {}

    /**
     * 现代化`operator =`，不使用`const &`直接使用临时值进行交换
     *
     * @param other 临时拷贝的`other`数据
     * @return this
     */
    intrusive_queue& operator= (intrusive_queue other) noexcept {
      std::swap(_head, other._head);
      std::swap(_tail, other._tail);
      return *this;
    }

    ~intrusive_queue() {
      assert(empty());
    }

  public:
    /**
     * 将元素链表反转，并返回一个新的`intrusive_queue`
     *
     * @param list 原始数据
     * @return 反转后的`intrusive_queue`
     */
    static intrusive_queue make_reversed(Item* list) noexcept {
      Item* new_head = nullptr;
      Item* new_tail = list;
      while (list != nullptr) {
        Item* next = list->*Next;
        list->*Next = new_head;
        new_head = list;
        list = next;
      }

      intrusive_queue result;
      result._head = new_head;
      result._tail = new_tail;

      return result;
    }

    [[nodiscard]] bool empty() const noexcept {
      return _head == nullptr;
    }

    [[nodiscard]] Item* pop_front() noexcept {
      assert(!empty());

      Item* item = std::exchange(_head, _head->*Next);
      if (_head == nullptr) {
        _tail = nullptr;
      }

      return item;
    }

    void push_front(Item* item) noexcept {
      assert(item != nullptr);

      item->*Next = _head;
      _head = item;
      if (_tail == nullptr) {
        _tail = item;
      }
    }

    void push_back(Item* item) noexcept {
      assert(item != nullptr);

      item->*Next = nullptr;
      if (_tail == nullptr) {
        _head = item;
      } else {
        _tail->*Next = item;
      }
      _tail = item;
    }

    /**
     * 将一个队列追加到当前队列的尾部
     *
     * @param other
     */
    void append(intrusive_queue other) noexcept {
      if (other.empty()) return;

      auto* other_head = std::exchange(other._head, nullptr);
      if (empty()) {
        _head = other_head;
      } else {
        _tail->*Next = other_head;
      }
      _tail = std::exchange(other._tail, nullptr);
    }

    /**
     * 将一个队列插入到当前队列的头部
     *
     * @param other
     */
    void prepend(intrusive_queue other) noexcept {
      if (other.empty()) return;

      other._tail->*Next = _head;
      _head = other._head;
      if (_tail == nullptr) {
        _tail = other._tail;
      }

      other._tail = nullptr;
      other._head = nullptr;
    }

  private:
    Item* _head = nullptr;
    Item* _tail = nullptr;
  };
}

#endif //EASY_STDEXEC_INSTRUSIVE_QUEUE_HPP
