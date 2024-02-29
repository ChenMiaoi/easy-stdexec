#ifndef EASY_STDEXEC_INSTRUSIVE_QUEUE_HPP
#define EASY_STDEXEC_INSTRUSIVE_QUEUE_HPP

#include <cassert>
#include <tuple>
#include <utility>

/// @tag ����ʽ�������
///
/// �����ⲿʵ��ֱ��Ӱ�������ڲ���Ա��������Ҫͨ������ӿڻ�̳й�ϵ
namespace example {
  /**
   * ��������ʽ����
   *
   * @tparam Next ����ָ��Item������һ��Ԫ�ص�ָ���Ա��ʵ����Ϊ Item::*Next
   */
  template <auto Next>
  class intrusive_queue;

  /**
   * ����ʽ���е�ʵ�֣��ػ�Next��Ҳ����Ҫ��Item����ӵ��Item* Next��Ա
   *
   * @tparam Item ������Ԫ�ص�����
   * @tparam Next ��һ��Ԫ�ص�ָ��
   */
  template <typename Item, Item* Item::*Next>
  class intrusive_queue<Next> {
  public:
    intrusive_queue() noexcept = default;

    /**
     * @tag �ƶ�����
     *
     * ��Ҫʹ��`std::exchange`��ԭ�ӵĽ��ƶ�������Ϊnullptr������`other`��ֵת����`this`
     *
     * \code{.cc}
     *
     * _head(std::exchange(other._head, nullptr));
     * // �ȼ��� ==>
     * _head = std::move(other._head);
     * other._head = nullptr
     * // ������ԭ�ӵ�
     *
     * \endcode
     *
     * @param other ��һ������ʽ����
     */
    intrusive_queue(intrusive_queue&& other) noexcept
      : _head(std::exchange(other._head, nullptr))
      , _tail(std::exchange(other._tail, nullptr)) {}

    /**
     * �ִ���`operator =`����ʹ��`const &`ֱ��ʹ����ʱֵ���н���
     *
     * @param other ��ʱ������`other`����
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
     * ��Ԫ������ת��������һ���µ�`intrusive_queue`
     *
     * @param list ԭʼ����
     * @return ��ת���`intrusive_queue`
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
     * ��һ������׷�ӵ���ǰ���е�β��
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
     * ��һ�����в��뵽��ǰ���е�ͷ��
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
