#ifndef AISDI_LINEAR_LINKEDLIST_H
#define AISDI_LINEAR_LINKEDLIST_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <utility>
namespace aisdi
{

template <typename Type>
class LinkedList
{
  struct Node
  {
    Node() : next(nullptr), prev(nullptr) {}
    Node(const Type &elem) : elem(elem), next(nullptr), prev(nullptr){};
    Node(const Type &elem, Node *prev, Node *next = nullptr) : elem(elem), next(next), prev(prev){};

    /**
     * @brief inserts itself between two other nodes, assumes that
     *        other nodes are connected
     * 
     *             ----                        ->  ----  <----
     *            |this|                       |  |this|-     |
     *             ----                        |   ----  |    |
     *                                         |  |      |    |
     *                                         |  |      |    |
     *                                         |  \/     \/   |
     *        ----   --->  -----              ----         -----
     *       |left|       |right|   =====>   |left|       |right|
     *        ----   <---  -----              ----         -----
     * 
     * @param left 
     * @param right 
     */
    void insertInBetween(Node *left, Node *right)
    {
      left->next = this;
      this->prev = left;
      right->prev = this;
      this->next = right;
    }

    void connectWith(Node *other)
    {
      next = other;
      other->prev = this;
    }

    void disconnect(){
      if(!next)
        throw std::out_of_range("Trying to remove static data");

      auto left = prev, right = next;
      prev = nullptr;
      next = nullptr;
      left->connectWith(right);
    }

    Type elem;
    Node *next;
    Node *prev;
  };

public:
  using difference_type = std::ptrdiff_t;
  using size_type = std::size_t;
  using value_type = Type;
  using pointer = Type *;
  using reference = Type &;
  using const_pointer = const Type *;
  using const_reference = const Type &;

  class ConstIterator;
  class Iterator;
  using iterator = Iterator;
  using const_iterator = ConstIterator;

  LinkedList() : _head(new Node), _tail(new Node), _size(0)
  {
    _head->next = _tail;
    _tail->prev = _head;
  }

  LinkedList(std::initializer_list<Type> l) : LinkedList()
  {
    for (const auto &elem : l)
    {
      append(elem);
    }
  }

  LinkedList(const LinkedList &other) : LinkedList()
  {
    for (const auto &elem : other)
    {
      append(elem);
    }
  }

  LinkedList(LinkedList &&other) : _head(other._head), _tail(other._tail), _size(other._size)
  {
    other._head = nullptr;
    other._tail = nullptr;
  }

  ~LinkedList()
  {
    deleteNodesFrom(_head, nullptr);
  }

  LinkedList &operator=(const LinkedList &other)
  {
    if (this == &other)
      return *this;

    deleteNodesFrom(_head->next, _tail);
    _head->connectWith(_tail);
     _size = 0;

    for (const auto &item : other)
      append(item);

    return *this;
  }

  LinkedList &operator=(LinkedList &&other)
  {
    if (this == &other)
      return *this;

    deleteNodesFrom(_head->next, _tail);
    _head->connectWith(_tail);

    std::swap(_head, other._head);
    std::swap(_tail, other._tail);
    _size = other._size;

    return *this;
  }

  bool isEmpty() const
  {
    return _size == 0;
  }

  size_type getSize() const
  {
    return _size;
  }

  Type &operator[](int pos)//to delete
  {
    if (!_head)
      throw std::runtime_error("Referencing deleted object");
    int i = 0;
    for (auto it = _head->next; it && it != _tail; it = it->next, ++i)
    {
      if (pos == i)
        return it->elem;
    }
    throw std::out_of_range("Index out of range0");
  }

  void append(const Type &item)
  {
    Node *newElem = new Node(item);
    newElem->insertInBetween(_tail->prev, _tail);
    ++_size;
  }

  void prepend(const Type &item)
  {
    Node *newElem = new Node(item);
    newElem->insertInBetween(_head, _head->next);
    ++_size;
  }

  void insert(const const_iterator &insertPosition, const Type &item)
  {
    Node *newElem = new Node(item);

      auto it = iterator(insertPosition);//we need to non const iterator
      auto right = it.node();
      auto left = right->prev;

      newElem->insertInBetween(left, right);
      _size++;
    
  }

  Type popFirst()
  {
    if (_size == 0)
      throw std::out_of_range("Popped empty list");

    auto elementToPop = _head->next;
    auto returnValue = elementToPop->elem;
    elementToPop->disconnect();

    delete elementToPop;

    --_size;

    return returnValue;
  }

  Type popLast()
  {
    if (_size == 0)
      throw std::out_of_range("Popped empty list");

    auto elementToPop = _tail->prev;
    auto returnValue = elementToPop->elem;

    elementToPop->disconnect();

    delete elementToPop;

    --_size;

    return returnValue;
  }

  void erase(const const_iterator &possition)
  {
    if (_size == 0)
      throw std::out_of_range("Popped empty list");

    auto nodeToErase = iterator(possition).node();//we need to non const iterator
    nodeToErase->disconnect();
    delete nodeToErase;

    --_size;
  }

  void erase(const const_iterator &firstIncluded, const const_iterator &lastExcluded)
  {
    auto itFirst = iterator(firstIncluded); //we need to non const iterator
    auto itLast = iterator(lastExcluded);

    auto first = itFirst.node(), last = itLast.node();
    first->prev->connectWith(last);

    _size -= deleteNodesFrom(first, last);
  }

  iterator       begin()        { return Iterator(_head->next); }
  iterator       end()          { return iterator(_tail); }
  const_iterator cbegin() const { return ConstIterator(_head->next); }
  const_iterator cend()   const { return const_iterator(_tail); }
  const_iterator begin()  const { return cbegin(); }
  const_iterator end()    const { return cend(); }

private:
  Node *_head;
  Node *_tail;
  size_type _size;

  static int deleteNodesFrom(Node *fromIncluded, Node *toExcluded)
  {
    int elementsDeleted = 0;
    auto it = fromIncluded;
    while (it && it != toExcluded)
    {
      auto next = it->next;
      delete it; elementsDeleted++;
      it = next;
    }
    return elementsDeleted;
  }

  // Node *findNode(const_iterator position)
  // {
  //   auto it = _head->next;
  //   while (it && it != _tail)
  //   {
  //     if (&(*position) == &(it->elem))
  //       return it;
  //   }
  //   return nullptr;
  // }
};

template <typename Type>
class LinkedList<Type>::ConstIterator
{
public:
  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = typename LinkedList::value_type;
  using difference_type = typename LinkedList::difference_type;
  using pointer = typename LinkedList::const_pointer;
  using reference = typename LinkedList::const_reference;

  explicit ConstIterator() : itr(nullptr)
  {
  }

  explicit ConstIterator(const Node *item) : itr(item) {}

  ConstIterator(const ConstIterator &other) : itr(other.itr) {}

  reference operator*() const
  {
    if(!itr)
      throw std::out_of_range("Dereferencing empty list");
    if (!(itr->next))
      throw std::out_of_range("Dereferencing end pointer");

    return itr->elem;
  }

  ConstIterator &operator++()
  {
    if (!itr || !(itr->next))
      throw std::out_of_range("Incrementing end pointer");

    itr = itr->next;
    return *this;
  }

  ConstIterator operator++(int)
  {
    if (!itr || !(itr->next))
      throw std::out_of_range("Incrementing end pointer");

    auto temp = *this;
    itr = itr->next;

    return temp;
  }

  ConstIterator &operator--()
  {
    if (!itr || !(itr->prev->prev))
      throw std::out_of_range("Decrementing end pointer");

    itr = itr->prev;
    return *this;
  }

  ConstIterator operator--(int)
  {
    if (!itr || !(itr->prev->prev))
      throw std::out_of_range("Decrementing end pointer");

    auto temp = *this;
    itr = itr->prev;

    return temp;
  }

  ConstIterator operator+(difference_type d) const
  {
    auto temp = itr;
    for (int i = 0; i < d; i++)
    {
      if (!temp || !(temp->next))
        throw std::out_of_range("Adding iterator pass the end");

      temp = temp->next;
    }

    return ConstIterator(temp);
  }

  ConstIterator operator-(difference_type d) const
  {
    auto temp = itr;
    for (int i = 0; i < d; i++)
    {
      if (!temp || !(temp->prev->prev))
        throw std::out_of_range("Substracting iterator pass the begining");

      temp = temp->prev;
    }

    return ConstIterator(temp);
  }

  bool operator==(const ConstIterator &other) const
  {
    return itr == other.itr;
  }

  bool operator!=(const ConstIterator &other) const
  {
    return !(*this == other);
  }

  const Node *node() const
  {
    return itr;
  }

private:
  const Node *itr;
};

template <typename Type>
class LinkedList<Type>::Iterator : public LinkedList<Type>::ConstIterator
{
public:
  using pointer = typename LinkedList::pointer;
  using reference = typename LinkedList::reference;

  explicit Iterator(const Node *item) : ConstIterator(item)
  {
  }

  Iterator(const ConstIterator &other)
      : ConstIterator(other)
  {
  }

  Iterator &operator++()
  {
    ConstIterator::operator++();
    return *this;
  }

  Iterator operator++(int)
  {
    auto result = *this;
    ConstIterator::operator++();
    return result;
  }

  Iterator &operator--()
  {
    ConstIterator::operator--();
    return *this;
  }

  Iterator operator--(int)
  {
    auto result = *this;
    ConstIterator::operator--();
    return result;
  }

  Iterator operator+(difference_type d) const
  {
    return ConstIterator::operator+(d);
  }

  Iterator operator-(difference_type d) const
  {
    return ConstIterator::operator-(d);
  }

  reference operator*() const
  {
    // ugly cast, yet reduces code duplication.
    return const_cast<reference>(ConstIterator::operator*());
  }

  Node *node()
  {
    return const_cast<Node *>(ConstIterator::node());
  }
};

} // namespace aisdi

#endif // AISDI_LINEAR_LINKEDLIST_H
