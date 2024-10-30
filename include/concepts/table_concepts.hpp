#pragma once
#include <concepts>
#include <utility>

namespace table_concepts {

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