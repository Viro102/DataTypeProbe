#pragma once
#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>
#include <concepts/generic_concepts.hpp>
#include <concepts/queue_concepts.hpp>
#include <iostream>
#include <stdexcept>
#include <utility.hpp>

TEMPLATE_LIST_TEST_CASE("Queue ADT basic operations", "[queue][basic]",
                        TESTS_LIST) {
  std::cout << "Testing type: " << type_name<TestType>() << std::endl;
  TestType test_queue;

  if constexpr (queue_concepts::has_push<TestType> &&
                queue_concepts::has_pop<TestType> &&
                generic_concepts::has_size<TestType>) {
    SECTION("Push and Pop Operations (FIFO Verification)") {
      // Test pushing elements
      REQUIRE_NOTHROW(test_queue.push(1));
      REQUIRE(test_queue.size() == 1);

      REQUIRE_NOTHROW(test_queue.push(2));
      REQUIRE(test_queue.size() == 2);

      REQUIRE_NOTHROW(test_queue.push(3));
      REQUIRE(test_queue.size() == 3);

      // Test popping elements (FIFO order verification)
      REQUIRE(test_queue.pop() == 1);
      REQUIRE(test_queue.size() == 2);

      REQUIRE(test_queue.pop() == 2);
      REQUIRE(test_queue.size() == 1);

      REQUIRE(test_queue.pop() == 3);
      REQUIRE(test_queue.size() == 0);
    }

    SECTION("Empty Queue Operations") {
      REQUIRE_THROWS_AS(test_queue.pop(), std::out_of_range);
    }
  }

  if constexpr (queue_concepts::has_front_queue<TestType>) {
    SECTION("Front Access") {
      test_queue.push(42);
      REQUIRE(test_queue.front() == 42);
      REQUIRE(test_queue.size() == 1);

      test_queue.push(43);
      REQUIRE(test_queue.front() == 42); // Front should still be first element
      REQUIRE(test_queue.size() == 2);
    }
  }

  if constexpr (queue_concepts::has_back_queue<TestType>) {
    SECTION("Back Access") {
      test_queue.push(42);
      REQUIRE(test_queue.back() == 42);

      test_queue.push(43);
      REQUIRE(test_queue.back() == 43); // Back should be last element
      REQUIRE(test_queue.size() == 2);
    }
  }
}

TEMPLATE_LIST_TEST_CASE("Queue ADT advanced operations", "[queue][advanced]",
                        TESTS_LIST) {
  std::cout << "Testing type: " << type_name<TestType>() << std::endl;
  TestType test_queue;

  if constexpr (generic_concepts::has_clear<TestType> &&
                generic_concepts::has_empty<TestType>) {
    SECTION("Clear Operation") {
      // Fill queue with elements
      for (int i = 0; i < 5; ++i) {
        test_queue.push(i);
      }
      REQUIRE_FALSE(test_queue.empty());

      // Clear and verify
      REQUIRE_NOTHROW(test_queue.clear());
      REQUIRE(test_queue.empty());
      REQUIRE(test_queue.size() == 0);

      // Verify queue is still usable after clear
      REQUIRE_NOTHROW(test_queue.push(42));
      REQUIRE(test_queue.size() == 1);
    }
  }

  SECTION("Capacity Management") {
    if constexpr (generic_concepts::has_capacity<TestType>) {
      const size_t LARGE_SIZE = 1000;
      size_t initial_capacity = test_queue.capacity();

      // Test capacity growth
      for (size_t i = 0; i < LARGE_SIZE; ++i) {
        test_queue.push(static_cast<int>(i));
      }

      REQUIRE(test_queue.capacity() >= LARGE_SIZE);
      REQUIRE(test_queue.capacity() >= initial_capacity);
    }
  }
}

TEMPLATE_LIST_TEST_CASE("Queue ADT edge cases and robustness", "[queue][edge]",
                        TESTS_LIST) {
  std::cout << "Testing type: " << type_name<TestType>() << std::endl;
  TestType test_queue;

  if constexpr (queue_concepts::has_push<TestType> &&
                queue_concepts::has_pop<TestType>) {
    SECTION("Push-Pop Sequence") {
      // Test alternating push-pop operations
      for (int i = 0; i < 100; ++i) {
        REQUIRE_NOTHROW(test_queue.push(i));
        REQUIRE(test_queue.pop() == i);
        REQUIRE(test_queue.empty());
      }
    }

    SECTION("Multiple Push-Pop Cycles") {
      // Fill and empty queue multiple times
      for (int cycle = 0; cycle < 5; ++cycle) {
        // Fill queue
        for (int i = 0; i < 10; ++i) {
          test_queue.push(i);
        }

        // Empty queue and verify FIFO order
        for (int i = 0; i < 10; ++i) {
          REQUIRE(test_queue.pop() == i);
        }

        REQUIRE(test_queue.empty());
      }
    }
  }

  if constexpr (std::is_copy_constructible_v<TestType> &&
                std::is_copy_assignable_v<TestType>) {
    SECTION("Copy Semantics") {
      // Fill original queue
      for (int i = 0; i < 5; ++i) {
        test_queue.push(i);
      }

      // Test copy constructor
      TestType copy_queue = test_queue;
      REQUIRE(copy_queue.size() == test_queue.size());

      // Verify independent copies
      test_queue.push(42);
      REQUIRE(copy_queue.size() != test_queue.size());

      // Verify FIFO order maintained in copy
      for (int i = 0; i < 5; ++i) {
        REQUIRE(copy_queue.front() == i);
        REQUIRE_NOTHROW(copy_queue.pop());
      }
    }
  }
}