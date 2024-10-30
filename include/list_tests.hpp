#pragma once
#include <catch2/catch_session.hpp>
#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators_random.hpp>
#include <concepts/generic_concepts.hpp>
#include <concepts/list_concepts.hpp>
#include <iostream>
#include <utility.hpp>

TEMPLATE_LIST_TEST_CASE("List access operations", "[list][access]",
                        TESTS_LIST) {
  std::cout << "Testing type: " + std::string(type_name<TestType>())
            << std::endl;

  TestType test_type{5, 4, 10, 41};
  REQUIRE(test_type.size() == 4);

  if constexpr (list_concepts::has_at<TestType>) {
    SECTION("Access") {
      REQUIRE(test_type.at(0) == 5);
      REQUIRE(test_type.at(1) == 4);
      REQUIRE(test_type.at(2) == 10);
      REQUIRE(test_type.at(3) == 41);
    }
  }

  if constexpr (list_concepts::has_at<TestType>) {
    SECTION("Out of range") {
      REQUIRE_THROWS_AS(test_type.at(test_type.size()), std::out_of_range);
    }
  }

  if constexpr (list_concepts::has_front<TestType>) {
    SECTION("Front") { REQUIRE(test_type.front() == 5); }
  }

  if constexpr (list_concepts::has_back<TestType>) {
    SECTION("Back") { REQUIRE(test_type.back() == 41); }
  }
}

TEMPLATE_LIST_TEST_CASE("List insert operations", "[list][modifier]",
                        TESTS_LIST) {
  std::cout << "Testing type: " + std::string(type_name<TestType>())
            << std::endl;

  TestType test_type{};

  if constexpr (list_concepts::has_push_back<TestType>) {
    SECTION("Randomize values") {
      const int size = 10;

      for (int i = 0; i < size; ++i) {
        int random_value = Catch::Generators::random(-100, 100).get();
        REQUIRE_NOTHROW(test_type.push_back(random_value));
      }
    }
  }
}

TEMPLATE_LIST_TEST_CASE("List default construction", "[list][construction]",
                        TESTS_LIST) {
  std::cout << "Testing type: " + std::string(type_name<TestType>())
            << std::endl;
  TestType test_type;

  if constexpr (generic_concepts::has_size<TestType> &&
                generic_concepts::has_empty<TestType>) {
    REQUIRE(test_type.size() == 0);
    REQUIRE(test_type.empty());
  }
}

TEMPLATE_LIST_TEST_CASE("List capacity operations", "[list][capacity]",
                        TESTS_LIST) {
  std::cout << "Testing type: " + std::string(type_name<TestType>())
            << std::endl;
  TestType test_type;

  if constexpr (generic_concepts::has_size<TestType> &&
                generic_concepts::has_empty<TestType> &&
                list_concepts::has_push_back<TestType>) {
    REQUIRE(test_type.size() == 0);
    REQUIRE(test_type.empty());

    test_type.push_back(3);
    REQUIRE(test_type.size() == 1);
    REQUIRE_FALSE(test_type.empty());
  }
}
