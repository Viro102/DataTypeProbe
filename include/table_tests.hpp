#pragma once
#include <catch2/catch_session.hpp>
#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators_random.hpp>
#include <concepts/generic_concepts.hpp>
#include <concepts/list_concepts.hpp>
#include <concepts/table_concepts.hpp>
#include <iostream>
#include <utility.hpp>

TEMPLATE_LIST_TEST_CASE("Table ADT tests", "[table]", TESTS_LIST) {
  // These lines will be done b4 each section
  std::cout << "Testing type: " << type_name<TestType>() << std::endl;

  TestType test_type;

  if constexpr (table_concepts::has_insert_pair<TestType>) {
    SECTION("Basic Insertion") {
      std::pair<int, int> pair{1, 3};
      REQUIRE_NOTHROW(test_type.insert(pair));
      REQUIRE(test_type.size() == 1);
    }

    SECTION("Multiple Insertions") {
      std::vector<std::pair<int, int>> pairs = {
          {1, 3}, {2, 6}, {3, 9}, {4, 12}};
      for (const auto &pair : pairs) {
        REQUIRE_NOTHROW(test_type.insert(pair));
      }
      REQUIRE(test_type.size() == pairs.size());
    }

    SECTION("Duplicate Key Insertion") {
      std::pair<int, int> pair1{1, 3};
      std::pair<int, int> pair2{1, 4};
      REQUIRE_NOTHROW(test_type.insert(pair1));
      auto insert_result =
          test_type.insert(pair2);   // std::map returns pair<iterator, bool>
      REQUIRE(test_type.at(1) == 3); // Original value should remain
      REQUIRE(test_type.size() == 1);
    }
  }

  if constexpr (table_concepts::has_find<TestType>) {
    SECTION("Basic Access") {
      std::pair<int, int> pair{1, 3};
      REQUIRE_NOTHROW(test_type.insert(pair));
      REQUIRE(test_type.contains(1));
      REQUIRE(test_type.at(1) == 3);
    }

    SECTION("Access Non-existent Key") {
      REQUIRE_FALSE(test_type.contains(999));
      REQUIRE_THROWS(test_type.at(999));
    }

    SECTION("Access After Multiple Operations") {
      std::pair<int, int> pair1{1, 3};
      std::pair<int, int> pair2{2, 6};
      REQUIRE_NOTHROW(test_type.insert(pair1));
      REQUIRE_NOTHROW(test_type.insert(pair2));
      REQUIRE(test_type.contains(1));
      REQUIRE(test_type.contains(2));
      REQUIRE(test_type.at(1) == 3);
      REQUIRE(test_type.at(2) == 6);
    }
  }

  if constexpr (table_concepts::has_erase<TestType>) {
    SECTION("Basic Removal") {
      std::pair<int, int> pair{1, 3};
      REQUIRE_NOTHROW(test_type.insert(pair));
      REQUIRE(test_type.contains(1));
      REQUIRE_NOTHROW(test_type.erase(1));
      REQUIRE_FALSE(test_type.contains(1));
      REQUIRE(test_type.size() == 0);
    }

    SECTION("Remove Non-existent Key") {
      // std::map::erase returns count of removed elements (0) and doesn't throw
      REQUIRE_NOTHROW(test_type.erase(999));
      REQUIRE(test_type.size() == 0);
    }

    SECTION("Remove After Multiple Operations") {
      std::vector<std::pair<int, int>> pairs = {{1, 3}, {2, 6}, {3, 9}};
      for (const auto &pair : pairs) {
        REQUIRE_NOTHROW(test_type.insert(pair));
      }
      REQUIRE_NOTHROW(test_type.erase(2));
      REQUIRE_FALSE(test_type.contains(2));
      REQUIRE(test_type.contains(1));
      REQUIRE(test_type.contains(3));
      REQUIRE(test_type.size() == 2);
    }
  }

  if constexpr (generic_concepts::has_clear<TestType> &&
                table_concepts::has_contains<TestType>) {
    SECTION("Clear Empty Table") {
      REQUIRE_NOTHROW(test_type.clear());
      REQUIRE(test_type.size() == 0);
    }

    SECTION("Clear Non-empty Table") {
      std::vector<std::pair<int, int>> pairs = {
          {1, 3}, {2, 6}, {3, 9}, {4, 12}};
      for (const auto &pair : pairs) {
        REQUIRE_NOTHROW(test_type.insert(pair));
      }
      REQUIRE_NOTHROW(test_type.clear());
      REQUIRE(test_type.size() == 0);
      for (const auto &pair : pairs) {
        REQUIRE_FALSE(test_type.contains(pair.first));
      }
    }
  }

  if constexpr (table_concepts::has_iterator<TestType>) {
    SECTION("Iterator Test") {
      std::vector<std::pair<int, int>> pairs = {{1, 3}, {2, 6}, {3, 9}};
      for (const auto &pair : pairs) {
        REQUIRE_NOTHROW(test_type.insert(pair));
      }

      std::vector<std::pair<int, int>> found_pairs;
      for (const auto &pair : test_type) {
        found_pairs.push_back(pair);
      }

      REQUIRE(found_pairs.size() == pairs.size());
      for (const auto &pair : pairs) {
        REQUIRE(std::find(found_pairs.begin(), found_pairs.end(), pair) !=
                found_pairs.end());
      }
    }
  }

  SECTION("Edge Cases") {
    if constexpr (table_concepts::has_insert_pair<TestType>) {
      SECTION("Insert After Clear") {
        std::pair<int, int> pair{1, 3};
        REQUIRE_NOTHROW(test_type.insert(pair));
        REQUIRE_NOTHROW(test_type.clear());
        REQUIRE_NOTHROW(test_type.insert(pair));
        REQUIRE(test_type.contains(1));
        REQUIRE(test_type.at(1) == 3);
      }
    }

    if constexpr (generic_concepts::has_capacity<TestType>) {
      SECTION("Capacity Tests") {
        size_t initial_capacity = test_type.capacity();
        std::vector<std::pair<int, int>> pairs;
        for (int i = 0; i < 1000; ++i) {
          pairs.emplace_back(i, i * 2);
        }
        for (const auto &pair : pairs) {
          REQUIRE_NOTHROW(test_type.insert(pair));
        }
        REQUIRE(test_type.capacity() >= pairs.size());
        REQUIRE(test_type.capacity() >= initial_capacity);
      }
    }
  }

  //   SECTION("Stress Test") {
  //     if constexpr (table_concepts::has_insert_pair<TestType> &&
  //                   table_concepts::has_erase<TestType>) {
  //       const int NUM_OPERATIONS = 1000;
  //       std::vector<int> keys;

  //       // Insert operations
  //       for (int i = 0; i < NUM_OPERATIONS; ++i) {
  //         int key = i;
  //         int value = i * 2;
  //         REQUIRE_NOTHROW(test_type.insert({key, value}));
  //         keys.push_back(key);
  //       }

  //       // Random access and removal
  //       auto rng = Catch::Generators::random(0, NUM_OPERATIONS - 1);
  //       for (int i = 0; i < NUM_OPERATIONS / 2; ++i) {
  //         int index = rng.get();
  //         int key = keys[index];
  //         REQUIRE_NOTHROW(test_type.erase(key));
  //         keys.erase(keys.begin() + index);
  //       }

  //       // Verify remaining elements
  //       for (int key : keys) {
  //         REQUIRE(test_type.contains(key));
  //         REQUIRE(test_type.at(key) == key * 2);
  //       }
  //     }
  //   }
}
