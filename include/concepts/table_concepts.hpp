#pragma once
#include <concepts>
#include <ranges>
#include <utility>

namespace table_concepts {

// Iterator concept helper to check if the type provides the required iterator
// operations
template <typename T>
concept has_iterator = requires(T t) {
  // Check if type has begin() and end()
  { t.begin() } -> std::forward_iterator;
  { t.end() } -> std::forward_iterator;
  // Checking if the iterator returns pairs
  requires requires(typename T::iterator it) {
    {
      *it
    } -> std::convertible_to<
          std::pair<const typename T::key_type, typename T::mapped_type>>;
  };
  // Check if type can be used in range-based for loop
  requires std::ranges::range<T>;
};

// Optional: Additional concepts for more specific iterator requirements
template <typename T>
concept has_const_iterator = requires(const T t) {
  { t.begin() } -> std::forward_iterator;
  { t.end() } -> std::forward_iterator;
  requires requires(typename T::const_iterator it) {
    {
      *it
    } -> std::convertible_to<
          const std::pair<const typename T::key_type, typename T::mapped_type>>;
  };
  requires std::ranges::range<const T>;
};

template <typename T>
concept has_insert_key_val = requires(T structure, typename T::key_type key,
                                      typename T::value_type value) {
  { structure.insert(key, value) };
};

template <typename T>
concept has_insert_pair = requires(T structure) {
  { structure.insert(std::declval<typename T::value_type>()) };
};

template <typename T>
concept has_erase = requires(T structure, typename T::key_type key) {
  { structure.erase(key) };
};

template <typename T>
concept has_find = requires(T structure, typename T::key_type key) {
  { structure.find(key) };
};

template <typename T>
concept has_contains = requires(T structure, typename T::key_type key) {
  { structure.contains(key) } -> std::convertible_to<bool>;
};

template <typename T>
concept has_set = requires(T structure, typename T::key_type key,
                           typename T::value_type value) {
  { structure.set(key, value) };
};

template <typename T>
concept has_keys = requires(T structure) {
  { structure.keys() };
};

template <typename T>
concept has_values = requires(T structure) {
  { structure.values() };
};

} // namespace table_concepts