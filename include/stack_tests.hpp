#pragma once
#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>
#include <concepts/generic_concepts.hpp>
#include <concepts/stack_concepts.hpp>
#include <iostream>
#include <stdexcept>
#include <utility.hpp>

TEMPLATE_LIST_TEST_CASE("Stack ADT basic operations", "[stack][basic]",
                        TESTS_LIST) {
  std::cout << "Testing type: " << type_name<TestType>() << std::endl;
  TestType test_stack;

  if constexpr (stack_concepts::has_push<TestType> &&
                stack_concepts::has_pop<TestType> &&
                generic_concepts::has_size<TestType>) {
    SECTION("Push and Pop Operations") {
      // Test pushing elements
      REQUIRE_NOTHROW(test_stack.push(1));
      REQUIRE(test_stack.size() == 1);

      REQUIRE_NOTHROW(test_stack.push(2));
      REQUIRE(test_stack.size() == 2);

      // Test popping elements (LIFO order verification)
      REQUIRE(test_stack.pop() == 2);
      REQUIRE(test_stack.size() == 1);

      REQUIRE(test_stack.pop() == 1);
      REQUIRE(test_stack.size() == 0);
    }

    SECTION("Empty Stack Operations") {
      // Test pop on empty stack
      REQUIRE_THROWS_AS(test_stack.pop(), std::out_of_range);
    }
  }

  if constexpr (stack_concepts::has_peek<TestType>) {
    SECTION("Peek Operations") {
      REQUIRE_THROWS_AS(test_stack.peek(), std::out_of_range);

      test_stack.push(42);
      REQUIRE(test_stack.peek() == 42);
      REQUIRE(test_stack.size() == 1); // Verify peek doesn't remove element

      test_stack.push(43);
      REQUIRE(test_stack.peek() == 43);
      REQUIRE(test_stack.size() == 2);
    }
  }
}

TEMPLATE_LIST_TEST_CASE("Stack ADT advanced operations", "[stack][advanced]",
                        TESTS_LIST) {
  std::cout << "Testing type: " << type_name<TestType>() << std::endl;
  TestType test_stack;

  if constexpr (generic_concepts::has_clear<TestType> &&
                generic_concepts::has_empty<TestType>) {
    SECTION("Clear Operation") {
      // Fill stack with elements
      for (int i = 0; i < 5; ++i) {
        test_stack.push(i);
      }
      REQUIRE_FALSE(test_stack.empty());

      // Clear and verify
      REQUIRE_NOTHROW(test_stack.clear());
      REQUIRE(test_stack.empty());
      REQUIRE(test_stack.size() == 0);
    }
  }

  SECTION("Capacity Management") {
    if constexpr (generic_concepts::has_capacity<TestType>) {
      const size_t LARGE_SIZE = 1000;
      size_t initial_capacity = test_stack.capacity();

      // Test capacity growth
      for (size_t i = 0; i < LARGE_SIZE; ++i) {
        test_stack.push(static_cast<int>(i));
      }

      REQUIRE(test_stack.capacity() >= LARGE_SIZE);
      REQUIRE(test_stack.capacity() >= initial_capacity);
    }
  }
}

TEMPLATE_LIST_TEST_CASE("Stack ADT edge cases", "[stack][edge]", TESTS_LIST) {
  std::cout << "Testing type: " << type_name<TestType>() << std::endl;
  TestType test_stack;

  if constexpr (stack_concepts::has_push<TestType> &&
                stack_concepts::has_pop<TestType>) {
    SECTION("Push-Pop Sequence") {
      // Test rapid push-pop sequences
      for (int i = 0; i < 100; ++i) {
        REQUIRE_NOTHROW(test_stack.push(i));
        REQUIRE(test_stack.pop() == i);
        REQUIRE(test_stack.empty());
      }
    }
  }

  if constexpr (std::is_copy_constructible_v<TestType> &&
                std::is_copy_assignable_v<TestType>) {
    SECTION("Copy Operations") {
      // Fill original stack
      for (int i = 0; i < 5; ++i) {
        test_stack.push(i);
      }

      // Test copy constructor
      TestType copy_stack = test_stack;
      REQUIRE(copy_stack.size() == test_stack.size());

      // Verify independent copies
      test_stack.push(42);
      REQUIRE(copy_stack.size() != test_stack.size());
    }
  }
}