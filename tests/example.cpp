#include <cstddef>
#include <map>
#include <queue>
#include <stack>
#include <stdexcept>
#include <vector>

// Custom data structure for testing (user's structure)
template <typename T> struct custom_vec {
  using value_type = T;
  using reference = T &;

  custom_vec() : data(){};

  custom_vec(std::initializer_list<T> init_list) : data(init_list) {}

  void push_back(T value) { data.push_back(value); }
  void pop_back() { data.pop_back(); }
  size_t size() const { return data.size(); }
  bool empty() const { return data.empty(); }
  void clear() { data.clear(); }
  void remove(size_t index) {
    if (index < size()) {
      data.erase(data.begin() + index);
    } else {
      throw std::out_of_range("out_of_range");
    }
  }
  void insert(size_t index, T value) {
    if (index <= size()) {
      data.insert(data.begin() + index, value);
    } else {
      throw std::out_of_range("out_of_range");
    }
  }

  T &front() {
    if (!data.empty()) {
      return data.front();
    } else {
      throw std::out_of_range("Index out of range");
    }
  }
  T &back() {
    if (!data.empty()) {
      return data.back();
    } else {
      throw std::out_of_range("Index out of range");
    }
  }
  T &at(size_t index) { return data.at(index); }

  // DEBUG
  std::vector<T> &vector() { return data; }

private:
  std::vector<T> data;
};

// Here user of the library defines their Data structure to test
// Test cases for each ADT are in separate files, user can include them as
// needed
// LIST ADT
// using MyTypes = std::tuple<custom_vec<int>, std::vector<int>>;
// TABLE ADT
// using MyTypes = std::tuple<std::map<int, int>>;
// STACK ADT
// using MyTypes = std::tuple<std::stack<int>>;
// QUEUE ADT
using MyTypes = std::tuple<std::queue<int>>;
// PRIORITY QUEUE ADT
// using MyTypes = std::tuple<std::priority_queue<int>>;
#define TESTS_LIST MyTypes
// #include <list_tests.hpp>
// #include <table_tests.hpp>
// #include <stack_tests.hpp>
#include <queue_tests.hpp>
// #include <priority_queue_tests.hpp>

int main(int argc, char *argv[]) {
  int result = data_type_probe::run_tests(argc, argv);

  return result;
}
