#pragma once
#include <concepts>

namespace generic_concepts {

template <typename T>
concept has_empty = requires(T structure) {
  { structure.empty() } -> std::convertible_to<bool>;
};

template <typename T>
concept has_size = requires(T structure) {
  { structure.size() } -> std::convertible_to<std::size_t>;
};

template <typename T>
concept has_clear = requires(T structure) {
  { structure.clear() };
};

} // namespace generic_concepts