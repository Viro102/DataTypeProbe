#pragma once
#include <concepts>

namespace stack_concepts {

template <typename T>
concept has_push = requires(T structure, typename T::value_type value) {
  { structure.push(value) };
};

template <typename T>
concept has_pop = requires(T structure) {
  { structure.pop() } -> std::convertible_to<typename T::value_type>;
};

template <typename T>
concept has_peek = requires(T structure) {
  { structure.peek() } -> std::convertible_to<typename T::value_type>;
};

} // namespace stack_concepts