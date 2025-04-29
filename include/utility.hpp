#pragma once
#include "catch2/catch_session.hpp"
#include <string_view>
#include <vector>

template <typename T> constexpr std::string_view type_name() {
#if defined(__clang__)
  std::string_view p = __PRETTY_FUNCTION__;
  constexpr std::string_view prefix = "std::string_view type_name() [T = ";
  constexpr std::string_view suffix = "]";
#elif defined(__GNUC__)
  std::string_view p = __PRETTY_FUNCTION__;
  constexpr std::string_view prefix =
      "constexpr std::string_view type_name() [with T = ";
  constexpr std::string_view suffix = "]";
#elif defined(_MSC_VER)
  std::string_view p = __FUNCSIG__;
  constexpr std::string_view prefix =
      "class std::basic_string_view<char,struct std::char_traits<char> > "
      "__cdecl type_name<";
  constexpr std::string_view suffix = ">(void)";
#else
#error Unsupported compiler
#endif

  const size_t start = p.find(prefix) + prefix.size();
  const size_t end = p.rfind(suffix);
  return p.substr(start, end - start);
}

#ifndef TESTS_LIST
#pragma message(                                                               \
    "TESTS_LIST macro not defined. Using default structure: std::vector<int>")
using DefaultTypes = std::tuple<std::vector<int>>;
#define TESTS_LIST DefaultTypes
#endif

struct data_type_probe {
  static int run_tests(int argc, char *argv[]) {
    Catch::Session session;

    // Apply any command line arguments to Catch2
    int return_code = session.applyCommandLine(argc, argv);

    // Indicates a command line error
    if (return_code != 0) {
      return return_code;
    }

    // Run the Catch2 session, which will run the test cases
    int result = session.run();

    return result;
  }
};