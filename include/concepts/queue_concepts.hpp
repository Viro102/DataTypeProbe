#pragma once
#include <concepts>

namespace queue_concepts {

template <typename T>
concept has_push = requires(T structure, typename T::value_type value) {
  { structure.push(value) };
};

template <typename T>
concept has_pop = requires(T structure) {
  { structure.pop() } -> std::convertible_to<typename T::value_type>;
};

template <typename T>
concept has_front_queue = requires(T structure) {
  { structure.front() } -> std::convertible_to<typename T::value_type>;
};

template <typename T>
concept has_back_queue = requires(T structure) {
  { structure.back() } -> std::convertible_to<typename T::value_type>;
};

} // namespace queue_concepts
