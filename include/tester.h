#pragma once
#include <catch2/catch_session.hpp>
#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>
#include <concepts>
#include <cstddef>

// Concepts to check if the container has specific methods
template <typename T>
concept SupportsPushBack = requires(T structure, typename T::value_type value) {
  { structure.push_back(value) };
};

template <typename T>
concept SupportsPop = requires(T structure) {
  { structure.pop() };
};

template <typename T>
concept SupportsTop = requires(T structure) {
  { structure.top() } -> std::convertible_to<typename T::value_type>;
};

template <typename T>
concept SupportsFront = requires(T structure) {
  { structure.front() } -> std::convertible_to<typename T::value_type>;
};

template <typename T>
concept SupportsEmpty = requires(T structure) {
  { structure.empty() } -> std::convertible_to<bool>;
};

template <typename T>
concept SupportsSize = requires(T structure) {
  { structure.size() } -> std::convertible_to<size_t>;
};

template <typename T> struct data_type_probe {
  int run_all_tests(T &container, int argc, char *argv[]) {
    Catch::Session session;

    // Apply any command line arguments to Catch2
    int return_code = session.applyCommandLine(argc, argv);

    // Indicates a command line error
    if (return_code != 0) {
      return return_code;
    }

    // Run the Catch2 session, which will run the test cases
    int result = session.run();

    return result;
  }

  int run_all_tests(T &container) {
    Catch::Session session;

    // Run the Catch2 session, which will run the test cases
    int result = session.run();

    return result;
  }
};

TEST_CASE("Data structure tests", "[test]") {
  SECTION("test") {
    std::vector<int> test = {1, 3, 4};
    REQUIRE(test.size() == 3);
  }
}

TEMPLATE_LIST_TEST_CASE("Data structure tests", "[container]", MyTypes) {
  TestType test_type;

  if constexpr (SupportsSize<TestType> && SupportsEmpty<TestType>) {
    SECTION("Empty") {
      REQUIRE(test_type.empty());
      REQUIRE(test_type.size() == 0);
    }
  }

  SECTION("Push back and access") {
    test_type.clear();

    test_type.push_back(1);
    test_type.push_back(2);
    test_type.push_back(3);

    REQUIRE(test_type.size() == 3);
    REQUIRE(test_type.front() == 1);
    REQUIRE(test_type.back() == 3);
    REQUIRE(test_type.at(1) == 2);
  }

  SECTION("Pop back") {
    test_type.clear();

    test_type.push_back(1);
    test_type.push_back(2);
    test_type.pop_back();

    REQUIRE(test_type.size() == 1);
    REQUIRE(test_type.back() == 1);
  }

  if constexpr (SupportsEmpty<TestType>) {
    SECTION("Clear") {
      test_type.push_back(1);
      test_type.push_back(2);
      test_type.clear();

      REQUIRE(test_type.empty());
      REQUIRE(test_type.size() == 0);
    }
  }
  SECTION("Out of range") {
    test_type.clear();

    test_type.push_back(1);

    REQUIRE_THROWS_AS(test_type.at(1), std::out_of_range);
  }
}
