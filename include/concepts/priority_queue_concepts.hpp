#pragma once
#include <concepts>

namespace priority_queue_concepts {

template <typename T>
concept has_push_priority = requires(T structure, typename T::value_type value,
                                     typename T::priority_type priority) {
  { structure.push(value, priority) };
};

template <typename T>
concept has_top_priority = requires(T structure) {
  { structure.top() } -> std::convertible_to<typename T::value_type>;
};

} // namespace priority_queue_concepts
