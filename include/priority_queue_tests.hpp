#pragma once
#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>
#include <concepts/generic_concepts.hpp>
#include <concepts/priority_queue_concepts.hpp>
#include <concepts/queue_concepts.hpp>
#include <iostream>
#include <utility.hpp>

TEMPLATE_LIST_TEST_CASE("Priority Queue basic operations",
                        "[priority_queue][basic]", TESTS_LIST) {
  std::cout << "Testing type: " << type_name<TestType>() << std::endl;
  TestType test_pq;

  if constexpr (priority_queue_concepts::has_push<TestType> &&
                priority_queue_concepts::has_top<TestType> &&
                queue_concepts::has_pop<TestType> &&
                generic_concepts::has_size<TestType>) {

    SECTION("Push and Top Operations") {
      // By default, std::priority_queue is a max heap
      test_pq.push(1); // lowest value
      test_pq.push(3); // highest value
      test_pq.push(2); // medium value

      REQUIRE(test_pq.size() == 3);
      REQUIRE(test_pq.top() == 3); // Highest value first

      REQUIRE(test_pq.pop());      // Remove highest
      REQUIRE(test_pq.top() == 2); // Next highest

      REQUIRE(test_pq.pop());      // Remove medium
      REQUIRE(test_pq.top() == 1); // Lowest left

      REQUIRE(test_pq.pop()); // Remove last item
      REQUIRE(test_pq.size() == 0);
    }

    SECTION("Order Verification") {
      std::vector<int> values = {4, 1, 3, 2, 5};
      for (int val : values) {
        test_pq.push(val);
      }

      // Values should come out in descending order (max heap)
      REQUIRE(test_pq.pop() == 5);
      REQUIRE(test_pq.pop() == 4);
      REQUIRE(test_pq.pop() == 3);
      REQUIRE(test_pq.pop() == 2);
      REQUIRE(test_pq.pop() == 1);
    }
  }
}

TEMPLATE_LIST_TEST_CASE("Priority Queue advanced operations",
                        "[priority_queue][advanced]", TESTS_LIST) {
  std::cout << "Testing type: " << type_name<TestType>() << std::endl;
  TestType test_pq;

  if constexpr (generic_concepts::has_clear<TestType> &&
                generic_concepts::has_empty<TestType>) {
    SECTION("Clear Operation") {
      test_pq.push(1);
      test_pq.push(2);
      test_pq.push(3);

      REQUIRE_FALSE(test_pq.empty());
      REQUIRE_NOTHROW(test_pq.clear());
      REQUIRE(test_pq.empty());
      REQUIRE(test_pq.size() == 0);

      // Verify queue is still usable after clear
      REQUIRE_NOTHROW(test_pq.push(42));
      REQUIRE(test_pq.size() == 1);
    }
  }

  if constexpr (generic_concepts::has_capacity<TestType>) {
    SECTION("Capacity Management") {
      const size_t LARGE_SIZE = 1000;
      size_t initial_capacity = test_pq.capacity();

      for (size_t i = 0; i < LARGE_SIZE; ++i) {
        test_pq.push(static_cast<int>(i));
      }

      REQUIRE(test_pq.capacity() >= LARGE_SIZE);
      REQUIRE(test_pq.capacity() >= initial_capacity);
    }
  }
}

TEMPLATE_LIST_TEST_CASE("Priority Queue edge cases", "[priority_queue][edge]",
                        TESTS_LIST) {
  std::cout << "Testing type: " << type_name<TestType>() << std::endl;
  TestType test_pq;

  SECTION("Empty Queue Operations") {
    REQUIRE(test_pq.empty());
    REQUIRE(test_pq.size() == 0);
  }

  if constexpr (std::is_copy_constructible_v<TestType> &&
                std::is_copy_assignable_v<TestType>) {
    SECTION("Copy Semantics") {
      test_pq.push(1);
      test_pq.push(3);
      test_pq.push(2);

      // Test copy constructor
      TestType copy_pq = test_pq;
      REQUIRE(copy_pq.size() == test_pq.size());

      // Verify independent copies
      test_pq.push(4);
      REQUIRE(copy_pq.size() != test_pq.size());

      // Verify order maintained in copy
      REQUIRE(copy_pq.top() == 3);    // Highest
      REQUIRE_NOTHROW(copy_pq.pop()); // Highest
      REQUIRE(copy_pq.top() == 2);    // Medium
      REQUIRE_NOTHROW(copy_pq.pop()); // Highest
      REQUIRE(copy_pq.top() == 1);    // Lowest
    }
  }
}

TEMPLATE_LIST_TEST_CASE("Priority Queue stress tests",
                        "[priority_queue][stress]", TESTS_LIST) {
  std::cout << "Testing type: " << type_name<TestType>() << std::endl;
  TestType test_pq;

  if constexpr (priority_queue_concepts::has_push<TestType> &&
                priority_queue_concepts::has_top<TestType>) {
    SECTION("Large Number of Elements") {
      std::vector<int> values;
      for (int i = 0; i < 1000; ++i) {
        values.push_back(i);
        test_pq.push(i);
      }

      // Sort in descending order for max heap comparison
      std::sort(values.begin(), values.end(), std::greater<int>());

      // Verify elements come out in correct order
      for (int expected : values) {
        REQUIRE(test_pq.top() == expected);
        REQUIRE_NOTHROW(test_pq.pop());
      }

      REQUIRE(test_pq.empty());
    }
  }
}