#pragma once
#include <cstddef>

template <typename T> class list_interface {
public:
  virtual ~list_interface() = default;

  // Insertion methods
  virtual void push_back(const T &value) = 0;
  virtual void push_front(const T &value) = 0;

  // Deletion methods
  virtual void pop_back() = 0;
  virtual void pop_front() = 0;

  // Element access
  virtual T &front() = 0;
  virtual const T &front() const = 0;
  virtual T &back() = 0;
  virtual const T &back() const = 0;
  virtual T &at(std::size_t index) = 0;
  virtual const T &at(std::size_t index) const = 0;

  // Capacity methods
  virtual std::size_t size() const = 0;
  virtual bool empty() const = 0;
  virtual void clear() = 0;
};