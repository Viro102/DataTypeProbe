#include <stdexcept>
#include <vector>

// Custom data structures
struct CustomContainer {
  using value_type = int;
  using reference = int &;

  void push_back(int value) { data.push_back(value); }
  void pop_back() { data.pop_back(); }
  size_t size() const { return data.size(); }
  // bool empty() const { return data.empty(); }
  void clear() { data.clear(); }
  int &front() { return data.front(); }
  int &back() { return data.back(); }
  int &at(size_t index) {
    if (index >= data.size())
      throw std::out_of_range("Index out of range");
    return data.at(index);
  }

private:
  std::vector<int> data;
};

// Here user of the library defines their Data structure to test
using MyTypes = std::tuple<std::vector<int>, CustomContainer>;
#define LIST_TEST MyTypes
#include "../include/tester.h"

int main(int argc, char *argv[]) {
  int result = data_type_probe::run_all_tests(argc, argv);

  return result;
}
