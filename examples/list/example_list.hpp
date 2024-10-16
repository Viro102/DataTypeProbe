#pragma once
#include "../../include/list/list_interface.hpp"
#include <vector>

template <typename T> class ExampleList : public ListInterface<T> {
private:
  std::vector<T> data;

public:
  void push_back(const T &value) override { data.push_back(value); }

  void push_front(const T &value) override { data.insert(data.begin(), value); }

  void pop_back() override {
    if (!empty()) {
      data.pop_back();
    }
  }

  void pop_front() override {
    if (!empty()) {
      data.erase(data.begin());
    }
  }

  T &front() override { return data.front(); }

  const T &front() const override { return data.front(); }

  T &back() override { return data.back(); }

  const T &back() const override { return data.back(); }

  std::size_t size() const override { return data.size(); }

  bool empty() const override { return data.empty(); }

  void clear() override { data.clear(); }

  T &at(std::size_t index) override { return data.at(index); }

  const T &at(std::size_t index) const override { return data.at(index); }
};