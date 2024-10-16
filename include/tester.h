#pragma once
#include <catch2/catch_session.hpp>
#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>
#include <concepts>
#include <cstddef>

#ifndef TEST_LIST
using Types = std::tuple<std::vector<int>>;
#define TEST_LIST Types
#endif

// Concepts to check if the container has specific methods
// TODO: make it general -> dont specify return type
// TODO: move this into separate files for each data type list_concepts.h,
// table_concepts.h ...?
template <typename T>
concept has_append = requires(T structure, typename T::value_type value) {
  { structure.push_back(value) };
};

template <typename T>
concept has_pop = requires(T structure) {
  { structure.pop() } -> std::convertible_to<typename T::value_type>;
};

template <typename T>
concept has_front = requires(T structure) {
  { structure.front() } -> std::convertible_to<typename T::value_type>;
};

template <typename T>
concept has_back = requires(T structure) {
  { structure.back() } -> std::convertible_to<typename T::value_type>;
};

template <typename T>
concept has_empty = requires(T structure) {
  { structure.empty() } -> std::convertible_to<bool>;
};

template <typename T>
concept has_size = requires(T structure) {
  { structure.size() } -> std::convertible_to<size_t>;
};

template <typename T>
concept has_clear = requires(T structure) {
  { structure.clear() };
};

template <typename T>
concept has_at = requires(T structure, size_t index) {
  { structure.at(index) } -> std::convertible_to<typename T::value_type>;
};

struct data_type_probe {
  static int run_all_tests(int argc, char *argv[]) {
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

  static int run_all_tests() {
    Catch::Session session;

    // Run the Catch2 session, which will run the test cases
    int result = session.run();

    return result;
  }
};

TEMPLATE_LIST_TEST_CASE("Pre-flight checks", "[pre-flight]", TEST_LIST) {
  TestType test_type;

  // TODO: refactor this to block other tests if this section fails
  // Check if data structure supports basic operations like:
  // empty(), clear(), size()
  SECTION("Pre-flight checks") {
    INFO("Make sure your data structure defines clear(), size(), and empty() "
         "methods.");
    REQUIRE((has_clear<TestType> && has_empty<TestType> && has_size<TestType>));
  }
}

TEMPLATE_LIST_TEST_CASE("Access operations", "[access]", TEST_LIST) {
  TestType test_type;

  if constexpr (has_append<TestType> && has_front<TestType> &&
                has_back<TestType> && has_at<TestType>) {
    SECTION("Push back and access") {
      test_type.push_back(1);
      test_type.push_back(2);
      test_type.push_back(3);

      // TODO: decouple tests for fewer methods
      REQUIRE(test_type.size() == 3);
      REQUIRE(test_type.front() == 1);
      REQUIRE(test_type.back() == 3);
      REQUIRE(test_type.at(1) == 2);
    }
  }
}

TEMPLATE_LIST_TEST_CASE("Data structure tests", "[full]", TEST_LIST) {
  TestType test_type;

  if constexpr (has_size<TestType> && has_empty<TestType>) {
    SECTION("Empty") {
      REQUIRE(test_type.empty());
      REQUIRE(test_type.size() == 0);
    }
  }

  if constexpr (has_append<TestType> && has_front<TestType> &&
                has_back<TestType> && has_size<TestType> &&
                has_front<TestType> && has_at<TestType>) {
    SECTION("Push back and access") {
      test_type.push_back(1);
      test_type.push_back(2);
      test_type.push_back(3);

      REQUIRE(test_type.size() == 3);
      REQUIRE(test_type.front() == 1);
      REQUIRE(test_type.back() == 3);
      REQUIRE(test_type.at(1) == 2);
    }
  }

  if constexpr (has_back<TestType> && has_pop<TestType> && has_back<TestType> &&
                has_size<TestType>)
    SECTION("Pop") {
      test_type.push_back(1);
      test_type.push_back(2);
      test_type.pop();

      REQUIRE(test_type.size() == 1);
      REQUIRE(test_type.back() == 1);
    }

  if constexpr (has_empty<TestType> && has_size<TestType> &&
                has_append<TestType> && has_clear<TestType>) {
    SECTION("Clear") {
      test_type.push_back(1);
      test_type.push_back(2);
      test_type.clear();

      REQUIRE(test_type.empty());
      REQUIRE(test_type.size() == 0);
    }
  }

  if constexpr (has_clear<TestType> && has_append<TestType> &&
                has_at<TestType>) {
    SECTION("Out of range") {
      test_type.push_back(1);

      REQUIRE_THROWS_AS(test_type.at(1), std::out_of_range);
    }
  }
}
