#include <map>
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
  T &front() { return data.front(); }
  T &back() { return data.back(); }
  T &at(size_t index) {
    if (index >= data.size())
      throw std::out_of_range("Index out of range");
    return data.at(index);
  }

private:
  std::vector<T> data;
};

// Here user of the library defines their Data structure to test
// TODO: test case for each ADT in separate files, user can include them as
// needed
// using MyTypes =
//     std::tuple<std::vector<int>, custom_vec<int>, std::map<int, int>>;
using MyTypes = std::tuple<std::map<int, int>>;
#define TESTS_LIST MyTypes
// #include <list_tests.hpp>
#include <table_tests.hpp>

int main(int argc, char *argv[]) {
  int result = data_type_probe::run_tests(argc, argv);

  return result;
}
