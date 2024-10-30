#pragma once
#include <concepts>

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

} // namespace list_concepts