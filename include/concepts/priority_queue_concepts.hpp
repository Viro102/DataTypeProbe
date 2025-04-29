#pragma once
#include <concepts>

namespace priority_queue_concepts {

template <typename T>
concept has_push = requires(T structure, typename T::value_type value) {
  { structure.push(value) };
};

template <typename T>
concept has_top = requires(T structure) {
  { structure.top() } -> std::convertible_to<typename T::value_type>;
};

} // namespace priority_queue_concepts