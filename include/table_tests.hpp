#pragma once
#include "concepts/table_concepts.hpp"
#include <catch2/catch_session.hpp>
#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators_random.hpp>
#include <concepts/generic_concepts.hpp>
#include <concepts/list_concepts.hpp>
#include <iostream>
#include <utility.hpp>

TEMPLATE_LIST_TEST_CASE("Table ADT tests", "[table]", TESTS_LIST) {
  // These lines will be done b4 each section
  std::cout << "Testing type: " << type_name<TestType>() << std::endl;

  TestType test_type;

  if constexpr (table_concepts::has_insert_pair<TestType>) {
    SECTION("Insertion") {
      std::pair<int, int> pair{1, 3};

      REQUIRE_NOTHROW(test_type.insert(pair));
    }
  }

  if constexpr (table_concepts::has_find<TestType>) {
    SECTION("Access") {
      std::pair<int, int> pair{1, 3};

      REQUIRE_NOTHROW(test_type.insert(pair));
      REQUIRE(test_type.contains(1));
      REQUIRE(test_type.at(1) == 3);
    }
  }
}