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

// Has capacity concept - checks if the type has a capacity() method that
// returns a size_t
template <typename T>
concept has_capacity = requires(const T &t) {
  { t.capacity() } -> std::convertible_to<std::size_t>;
};

} // namespace generic_concepts