#pragma once
#include <concepts>
#include <iterator>

namespace list_concepts {

template <typename T>
concept has_push_back = requires(T structure, typename T::value_type value) {
  { structure.push_back(value) };
};

template <typename T>
concept has_push_front = requires(T structure, typename T::value_type value) {
  { structure.push_front(value) };
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
concept has_at = requires(T structure, std::size_t index) {
  { structure.at(index) } -> std::convertible_to<typename T::value_type>;
};

template <typename T>
concept has_remove = requires(T structure, std::size_t index) {
  { structure.remove(index) };
};

template <typename T>
concept has_insert =
    requires(T structure, std::size_t index, typename T::value_type value) {
      { structure.insert(index, value) };
    };

template <typename T>
concept has_iterator = requires(T structure) {
  typename T::iterator;
  typename T::const_iterator;
  { structure.begin() } -> std::same_as<typename T::iterator>;
  { structure.end() } -> std::same_as<typename T::iterator>;
  { structure.cbegin() } -> std::same_as<typename T::const_iterator>;
  { structure.cend() } -> std::same_as<typename T::const_iterator>;
};

template <typename T>
concept has_input_iterator = has_iterator<T> && requires(T structure) {
  requires std::input_iterator<typename T::iterator>;
  requires std::input_iterator<typename T::const_iterator>;
};
} // namespace list_concepts