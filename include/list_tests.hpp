#pragma once
#include <catch2/catch_session.hpp>
#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators_random.hpp>
#include <concepts/generic_concepts.hpp>
#include <concepts/list_concepts.hpp>
#include <iostream>
#include <stdexcept>
#include <utility.hpp>

TEMPLATE_LIST_TEST_CASE("List access operations", "[list][access]",
                        TESTS_LIST) {
  std::cout << "Testing type: " << type_name<TestType>() << std::endl;

  TestType test_type{5, 4, 10, 41};
  REQUIRE(test_type.size() == 4);

  if constexpr (list_concepts::has_at<TestType>) {
    SECTION("Access") {
      REQUIRE(test_type.at(0) == 5);
      REQUIRE(test_type.at(1) == 4);
      REQUIRE(test_type.at(2) == 10);
      REQUIRE(test_type.at(3) == 41);
    }

    SECTION("Access in empty list") {
      TestType empty_list;
      REQUIRE_THROWS_AS(empty_list.at(0), std::out_of_range);
      // TODO: design choice, require these functions to throw an exception or
      // something else?
      if constexpr (list_concepts::has_front<TestType>) {
        INFO("List should throw an out_of_range exception when accessing "
             "front() in empty list");
        REQUIRE_THROWS_AS(empty_list.front(), std::out_of_range);
      }
      if constexpr (list_concepts::has_back<TestType>) {
        INFO("List should throw an out_of_range exception when accessing "
             "back() in empty list");
        REQUIRE_THROWS_AS(empty_list.back(), std::out_of_range);
      }
    }

    if constexpr (generic_concepts::has_size<TestType>) {
      SECTION("Access out of range") {
        REQUIRE_THROWS_AS(test_type.at(test_type.size()), std::out_of_range);
        REQUIRE_THROWS_AS(test_type.at(test_type.size() + 10),
                          std::out_of_range);
      }

      SECTION("Access at negative index") {
        REQUIRE_THROWS_AS(test_type.at(-3), std::out_of_range);
      }
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
  std::cout << "Testing type: " << type_name<TestType>() << std::endl;

  TestType test_type{};

  if constexpr (list_concepts::has_push_back<TestType> &&
                generic_concepts::has_size<TestType>) {
    SECTION("Insert random values") {
      const int size = 10000;
      bool exception_thrown = false;

      try {
        for (int i = 0; i < size; ++i) {
          int random_value = Catch::Generators::random(-100, 100).get();
          test_type.push_back(random_value);
        }
      } catch (...) {
        exception_thrown = true;
      }

      REQUIRE_FALSE(exception_thrown);

      REQUIRE(test_type.size() == size);
    }
  }

  if constexpr (list_concepts::has_insert<TestType>) {
    SECTION("Insert at specific positions") {
      REQUIRE_NOTHROW(test_type.insert(0, 100));
      REQUIRE(test_type.at(0) == 100);

      REQUIRE_NOTHROW(test_type.insert(1, 200));
      REQUIRE(test_type.at(1) == 200);

      // TODO: define behaviour throw exception or something else?
      REQUIRE_THROWS_AS(test_type.insert(6, 300), std::out_of_range);

      REQUIRE_THROWS_AS(test_type.insert(-1, 700), std::out_of_range);
    }
  }
}

TEMPLATE_LIST_TEST_CASE("List deletion operations", "[list][modifier]",
                        TESTS_LIST) {
  std::cout << "Testing type: " << type_name<TestType>() << std::endl;

  TestType test_type{1, 4, 2, 6, 7};

  if constexpr (generic_concepts::has_clear<TestType> &&
                generic_concepts::has_empty<TestType> &&
                generic_concepts::has_size<TestType>) {
    SECTION("Clear") {
      REQUIRE_NOTHROW(test_type.clear());
      REQUIRE(test_type.size() == 0);
      REQUIRE(test_type.empty());
    }
  }

  if constexpr (list_concepts::has_remove<TestType>) {
    SECTION("Remove") {
      test_type.remove(2);
      REQUIRE(test_type.size() == 4);
      REQUIRE(test_type.at(0) == 1);
      REQUIRE(test_type.at(1) == 4);
      REQUIRE(test_type.at(2) == 6);
      REQUIRE(test_type.at(3) == 7);
    }

    SECTION("Remove invalid index") {
      REQUIRE_THROWS_AS(test_type.remove(100), std::out_of_range);
      REQUIRE(test_type.size() == 5);
    }

    SECTION("Remove empty list") {
      TestType empty_list;
      REQUIRE_THROWS_AS(empty_list.remove(1), std::out_of_range);
      REQUIRE(empty_list.size() == 0);
    }
  }
}

TEMPLATE_LIST_TEST_CASE("List capacity operations", "[list][capacity]",
                        TESTS_LIST) {
  std::cout << "Testing type: " << type_name<TestType>() << std::endl;
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

TEMPLATE_LIST_TEST_CASE("List iterators", "[list][iterator]", TESTS_LIST) {
  std::cout << "Testing type: " << type_name<TestType>() << std::endl;

  if constexpr (list_concepts::has_iterator<TestType>) {
    TestType test_type{1, 2, 3, 4, 5};

    SECTION("Iterator traversal") {
      auto it = test_type.begin();
      auto end = test_type.end();
      int expected_value = 1;
      for (; it != end; ++it, ++expected_value) {
        REQUIRE(*it == expected_value);
      }
      REQUIRE(expected_value == 6);
    }

    SECTION("Const_iterator traversal") {
      const TestType &const_test_type = test_type;
      auto it = const_test_type.cbegin();
      auto end = const_test_type.cend();
      int expected_value = 1;
      for (; it != end; ++it, ++expected_value) {
        REQUIRE(*it == expected_value);
      }
      REQUIRE(expected_value == 6);
    }

    SECTION("For-each loop") {
      int sum = 0;
      for (const auto &value : test_type) {
        sum += value;
      }
      REQUIRE(sum == 15);
    }
  }
}

TEMPLATE_LIST_TEST_CASE("List default construction", "[list][construction]",
                        TESTS_LIST) {
  std::cout << "Testing type: " << type_name<TestType>() << std::endl;
  TestType test_type;

  if constexpr (generic_concepts::has_size<TestType> &&
                generic_concepts::has_empty<TestType>) {
    REQUIRE(test_type.size() == 0);
    REQUIRE(test_type.empty());
  }
}

TEMPLATE_LIST_TEST_CASE("List copy and assignment", "[list][copy]",
                        TESTS_LIST) {
  std::cout << "Testing type: " << type_name<TestType>() << std::endl;

  if constexpr (std::is_copy_constructible_v<TestType> &&
                std::is_copy_assignable_v<TestType> &&
                generic_concepts::has_size<TestType> &&
                list_concepts::has_at<TestType>) {
    SECTION("Copy constructor") {
      TestType original{1, 2, 3};
      TestType copy = original;
      REQUIRE(copy.size() == 3);
      REQUIRE(copy.at(0) == 1);
      REQUIRE(copy.at(1) == 2);
      REQUIRE(copy.at(2) == 3);
    }
    SECTION("Copy assignment") {
      TestType original{4, 5, 6};
      TestType assigned;
      assigned = original;
      REQUIRE(assigned.size() == 3);
      REQUIRE(assigned.at(0) == 4);
      REQUIRE(assigned.at(1) == 5);
      REQUIRE(assigned.at(2) == 6);
    }

    SECTION("Self-assignment") {
      TestType list{1, 2, 3};
      list = list;
      REQUIRE(list.size() == 3);
      REQUIRE(list.at(0) == 1);
      REQUIRE(list.at(1) == 2);
      REQUIRE(list.at(2) == 3);
    }

    if constexpr (list_concepts::has_push_back<TestType>) {
      SECTION("Modify original after copy") {
        TestType original{1, 2, 3};
        TestType copy = original;
        original.push_back(4);
        REQUIRE(original.size() == 4);
        REQUIRE(original.at(0) == 1);
        REQUIRE(original.at(1) == 2);
        REQUIRE(original.at(2) == 3);
        REQUIRE(original.at(3) == 4);
        REQUIRE(copy.size() == 3);
        REQUIRE(copy.at(0) == 1);
        REQUIRE(copy.at(1) == 2);
        REQUIRE(copy.at(2) == 3);
      }
    }

    SECTION("Assignment to non-empty list") {
      TestType original{7, 8, 9};
      TestType assigned{1, 2};
      assigned = original;
      REQUIRE(assigned.size() == 3);
      REQUIRE(assigned.at(0) == 7);
      REQUIRE(assigned.at(1) == 8);
      REQUIRE(assigned.at(2) == 9);
    }
  }
}

TEMPLATE_LIST_TEST_CASE("List move semantics", "[list][move]", TESTS_LIST) {
  std::cout << "Testing type: " << type_name<TestType>() << std::endl;

  if constexpr (std::is_move_constructible_v<TestType> &&
                std::is_move_assignable_v<TestType> &&
                generic_concepts::has_size<TestType>) {
    SECTION("Move constructor") {
      TestType original{1, 2, 3};
      TestType moved = std::move(original);
      REQUIRE(moved.size() == 3);
      REQUIRE(original.size() == 0);
    }

    SECTION("Move assignment") {
      TestType original{4, 5, 6};
      TestType moved;
      moved = std::move(original);
      REQUIRE(moved.size() == 3);
      REQUIRE(original.size() == 0);
    }
  }
}